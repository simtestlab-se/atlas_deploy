#!/usr/bin/env python3
"""
GitHub Issues Fetcher - Clean Table View Version

This script fetches GitHub issues from repositories and generates
Markdown files in a simple table format for MkDocs documentation.
"""

import argparse
import logging
import os
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime, timezone, timedelta
from pathlib import Path
from typing import Any, Dict, List, Optional

import requests
import yaml
import pandas as pd


class GitHubIssuesFetcher:
    """Main class for fetching GitHub issues and generating documentation."""
    
    def __init__(self, config_path: str, dry_run: bool = False, verbose: bool = False):
        """Initialize the GitHub Issues Fetcher."""
        self.config_path = config_path
        self.dry_run = dry_run
        self.config = self._load_config()
        self.logger = self._setup_logging(verbose)
        
        # GitHub API setup
        self.session = requests.Session()
        if self.config['github']['token']:
            self.session.headers.update({
                'Authorization': f"token {self.config['github']['token']}",
                'Accept': 'application/vnd.github.v3+json'
            })
            self.logger.debug(f"GitHub token configured: {self.config['github']['token'][:8]}...")
        else:
            self.logger.warning("No GitHub token configured - rate limits will be very low (60/hour)")
        
        # Statistics tracking
        self.stats = {
            'repositories_processed': 0,
            'issues_fetched': 0,
            'files_created': 0,
            'errors': []
        }
    
    def _load_config(self) -> Dict[str, Any]:
        """Load configuration from YAML file."""
        try:
            with open(self.config_path, 'r', encoding='utf-8') as f:
                config = yaml.safe_load(f)
            
            # Expand environment variables in token
            token = config['github']['token']
            if token and token.startswith('${') and token.endswith('}'):
                env_var = token[2:-1]  # Remove ${ and }
                config['github']['token'] = os.getenv(env_var, '')
            
            return config
        except Exception as e:
            print(f"Error loading config: {e}")
            sys.exit(1)
    
    def _setup_logging(self, verbose: bool) -> logging.Logger:
        """Set up logging configuration."""
        level = logging.DEBUG if verbose else logging.INFO
        logging.basicConfig(
            format='%(asctime)s - %(levelname)s - %(message)s',
            level=level
        )
        return logging.getLogger(__name__)
    
    def _ensure_directory_exists(self, path: Path) -> None:
        """Ensure directory exists."""
        if not self.dry_run:
            path.mkdir(parents=True, exist_ok=True)
    
    def _format_date(self, date_str: str) -> str:
        """Format ISO date string to human readable format."""
        try:
            dt = datetime.fromisoformat(date_str.replace('Z', '+00:00'))
            return dt.strftime('%Y-%m-%d')
        except:
            return date_str[:10]
    
    def _format_filename(self, issue: Dict[str, Any], repo_name: str) -> str:
        """Format filename for issue markdown file."""
        number = issue['number']
        title = issue['title']
        # Clean title for filename
        clean_title = ''.join(c for c in title if c.isalnum() or c in (' ', '-', '_')).strip()
        clean_title = clean_title.replace(' ', '-')[:50]
        return f"issue-{number}-{clean_title}.md"
    
    def fetch_issues(self) -> Dict[str, List[Dict[str, Any]]]:
        """Fetch issues from all repositories."""
        org = self.config['github']['organization']
        all_issues = {}
        
        # Get repositories
        repositories = self._get_repositories()
        
        if not repositories:
            self.logger.warning("No repositories found")
            return all_issues
        
        # Fetch issues with concurrent processing
        with ThreadPoolExecutor(max_workers=self.config['processing']['max_workers']) as executor:
            future_to_repo = {
                executor.submit(self._fetch_repository_issues, repo): repo
                for repo in repositories
            }
            
            for future in as_completed(future_to_repo):
                repo = future_to_repo[future]
                try:
                    issues = future.result()
                    if issues:
                        all_issues[repo] = issues
                        self.stats['repositories_processed'] += 1
                        self.stats['issues_fetched'] += len(issues)
                        self.logger.info(f"Fetched {len(issues)} issues from {repo}")
                except Exception as e:
                    error_msg = f"Error processing {repo}: {str(e)}"
                    self.stats['errors'].append(error_msg)
                    self.logger.error(error_msg)
        
        return all_issues
    
    def _get_repositories(self) -> List[str]:
        """Get list of repositories to process."""
        repo_config = self.config.get('repositories', {})
        
        # Check for include list
        include_repos = repo_config.get('include', [])
        if include_repos:
            self.logger.info(f"Using configured repositories: {include_repos}")
            return include_repos
        
        # Check for Excel file
        excel_config = repo_config.get('excel', {})
        excel_file = excel_config.get('file_path')
        if excel_file and os.path.exists(excel_file):
            try:
                sheet_name = excel_config.get('sheet_name', 'Sheet1')
                repo_column = excel_config.get('repo_column', 'Repository')
                
                # Detect file type and read accordingly
                if excel_file.lower().endswith('.xlsx') or excel_file.lower().endswith('.xls'):
                    df = pd.read_excel(excel_file, sheet_name=sheet_name)
                elif excel_file.lower().endswith('.csv'):
                    df = pd.read_csv(excel_file)
                else:
                    self.logger.warning(f"Unsupported file format: {excel_file}. Falling back to organization fetch.")
                    df = None
                
                if df is not None and repo_column in df.columns:
                    repositories = df[repo_column].dropna().astype(str).tolist()
                    repositories = [repo.strip() for repo in repositories if repo.strip()]
                    self.logger.info(f"Loaded {len(repositories)} repositories from file: {repositories}")
                    return repositories
                else:
                    self.logger.warning(f"Column '{repo_column}' not found in file. Falling back to organization fetch.")
            except Exception as e:
                self.logger.error(f"Error reading file: {e}. Falling back to organization fetch.")
        
        # If no specific repositories configured, fetch all from organization
        org = self.config['github']['organization']
        url = f"https://api.github.com/orgs/{org}/repos"
        
        repos_data = []
        page = 1
        per_page = 100
        
        try:
            # Fetch all pages of repositories
            while True:
                params = {
                    'type': 'all',  # Include all types: public, private, forks, sources, member
                    'per_page': per_page,
                    'page': page,
                    'sort': 'updated',
                    'direction': 'desc'
                }
                
                try:
                    response = self.session.get(url, params=params)
                    
                    # Check rate limit before proceeding
                    remaining = int(response.headers.get('X-RateLimit-Remaining', 0))
                    reset_time = int(response.headers.get('X-RateLimit-Reset', 0))
                    
                    if response.status_code == 403 and 'rate limit' in response.text.lower():
                        sleep_time = reset_time - int(time.time()) + 1
                        if sleep_time > 0:
                            self.logger.warning(f"Rate limit exceeded. Would need to wait {sleep_time} seconds ({sleep_time/60:.1f} minutes)")
                            self.logger.info("Consider using a GitHub token with higher rate limits or running the script later.")
                            self.logger.info("Press Ctrl+C to cancel or wait for automatic retry...")
                            try:
                                self._interruptible_sleep(sleep_time)
                                continue
                            except KeyboardInterrupt:
                                self.logger.info("Operation cancelled by user")
                                raise
                    
                    response.raise_for_status()
                    page_repos = response.json()
                    
                    if not page_repos:
                        break
                        
                    repos_data.extend(page_repos)
                    self.logger.info(f"Fetched page {page}: {len(page_repos)} repositories (Rate limit: {remaining} remaining)")
                    
                    # Check if we have more pages
                    if len(page_repos) < per_page:
                        break
                        
                    page += 1
                    
                    # Add delay between pages
                    delay = self.config.get('performance', {}).get('request_delay', 1.0)
                    time.sleep(delay)
                    
                except requests.exceptions.RequestException as e:
                    self.logger.error(f"Error fetching page {page}: {e}")
                    break
            
            # Log all repositories found
            all_repos = [repo['name'] for repo in repos_data]
            self.logger.info(f"Found {len(all_repos)} total repositories in {org}: {', '.join(all_repos)}")
            
            # Filter based on archived status
            if not repo_config.get('include_archived', False):
                repos_data = [repo for repo in repos_data if not repo.get('archived', False)]
                self.logger.info(f"After filtering archived: {len(repos_data)} repositories")
            
            # Filter based on private status
            include_private = repo_config.get('include_private', False)
            if not include_private:
                private_count = len([repo for repo in repos_data if repo.get('private', False)])
                repos_data = [repo for repo in repos_data if not repo.get('private', False)]
                self.logger.info(f"Excluded {private_count} private repositories")
            else:
                private_count = len([repo for repo in repos_data if repo.get('private', False)])
                self.logger.info(f"Including {private_count} private repositories")
            
            repositories = [repo['name'] for repo in repos_data]
            self.logger.info(f"Final repository list: {', '.join(repositories)}")
            
            # Apply exclude filter
            exclude_repos = repo_config.get('exclude', [])
            if exclude_repos:
                repositories = [repo for repo in repositories if repo not in exclude_repos]
                self.logger.info(f"After excluding {exclude_repos}: {', '.join(repositories)}")
            
        except Exception as e:
            self.logger.error(f"Error fetching repositories: {e}")
            return []
        
        return repositories
    
    def _fetch_repository_issues(self, repo_name: str) -> List[Dict[str, Any]]:
        """Fetch issues from a single repository."""
        org = self.config['github']['organization']
        issues = []
        page = 1
        per_page = 100
        
        while True:
            url = f"https://api.github.com/repos/{org}/{repo_name}/issues"
            params = {
                'state': 'all',
                'per_page': per_page,
                'page': page,
                'sort': 'updated',
                'direction': 'desc'
            }
            
            try:
                response = self.session.get(url, params=params)
                response.raise_for_status()
                
                page_issues = response.json()
                if not page_issues:
                    break
                
                # Filter out pull requests (GitHub API returns both issues and PRs)
                page_issues = [issue for issue in page_issues if 'pull_request' not in issue]
                
                # Apply filters
                filtered_issues = self._apply_filters(page_issues)
                issues.extend(filtered_issues)
                
                # Check rate limit
                self._check_rate_limit(response)
                
                page += 1
                
            except Exception as e:
                self.logger.error(f"Error fetching issues from {repo_name}: {e}")
                break
        
        return issues
    
    def _apply_filters(self, issues: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
        """Apply configuration filters to issues."""
        filters = self.config.get('filters', {})
        
        # Filter by labels
        if filters.get('include_labels'):
            include_labels = set(filters['include_labels'])
            issues = [
                issue for issue in issues
                if any(label['name'] in include_labels for label in issue.get('labels', []))
            ]
        
        if filters.get('exclude_labels'):
            exclude_labels = set(filters['exclude_labels'])
            issues = [
                issue for issue in issues
                if not any(label['name'] in exclude_labels for label in issue.get('labels', []))
            ]
        
        # Filter by date range
        if filters.get('date_range', {}).get('days'):
            days = filters['date_range']['days']
            cutoff_date = datetime.now(timezone.utc).replace(tzinfo=None)
            cutoff_date = cutoff_date.replace(microsecond=0) - timedelta(days=days)
            
            issues = [
                issue for issue in issues
                if datetime.fromisoformat(issue['updated_at'].replace('Z', '')) >= cutoff_date
            ]
        
        return issues
    
    def _interruptible_sleep(self, total_seconds: int) -> None:
        """Sleep for the specified time but allow interruption every 10 seconds."""
        remaining = total_seconds
        while remaining > 0:
            sleep_chunk = min(10, remaining)  # Sleep in 10-second chunks
            print(f"\rWaiting... {remaining} seconds remaining (Ctrl+C to cancel)", end="", flush=True)
            time.sleep(sleep_chunk)
            remaining -= sleep_chunk
        print()  # New line after completion

    def check_rate_limit_status(self) -> None:
        """Check and display current GitHub API rate limit status."""
        try:
            response = self.session.get(f"{self.config['github']['api_url']}/rate_limit")
            response.raise_for_status()
            
            rate_limit_info = response.json()
            core_limit = rate_limit_info['resources']['core']
            
            remaining = core_limit['remaining']
            limit = core_limit['limit']
            reset_time = core_limit['reset']
            
            # Calculate time until reset
            reset_datetime = datetime.fromtimestamp(reset_time, tz=timezone.utc)
            now = datetime.now(timezone.utc)
            time_until_reset = reset_datetime - now
            
            print("GitHub API Rate Limit Status:")
            print(f"  Limit: {limit} requests per hour")
            print(f"  Remaining: {remaining} requests")
            print(f"  Used: {limit - remaining} requests")
            print(f"  Reset time: {reset_datetime.strftime('%Y-%m-%d %H:%M:%S UTC')}")
            
            if time_until_reset.total_seconds() > 0:
                minutes_until_reset = int(time_until_reset.total_seconds() / 60)
                print(f"  Time until reset: {minutes_until_reset} minutes")
            else:
                print(f"  Reset time has passed")
            
            # Provide recommendations
            print("\nRecommendations:")
            if remaining > 100:
                print("  ✅ Rate limit looks good - safe to run the fetcher")
            elif remaining > 20:
                print("  ⚠️  Rate limit is getting low - proceed with caution")
            else:
                print("  ❌ Rate limit is very low - consider waiting for reset")
                
        except requests.exceptions.RequestException as e:
            self.logger.error(f"Failed to check rate limit: {e}")
        except Exception as e:
            self.logger.error(f"Error checking rate limit: {e}")

    def _check_rate_limit(self, response: requests.Response) -> None:
        """Check and handle GitHub API rate limits."""
        remaining = int(response.headers.get('X-RateLimit-Remaining', 0))
        reset_time = int(response.headers.get('X-RateLimit-Reset', 0))
        
        # Add delay between requests to prevent hitting rate limits
        delay = self.config.get('performance', {}).get('request_delay', 0.5)
        if delay > 0:
            time.sleep(delay)
        
        # Check if we need to wait for rate limit reset
        if remaining < 5:  # More conservative threshold
            sleep_time = reset_time - int(time.time()) + 1
            if sleep_time > 0:
                self.logger.warning(f"Rate limit approaching. Sleeping for {sleep_time} seconds ({sleep_time/60:.1f} minutes)")
                try:
                    self._interruptible_sleep(sleep_time)
                except KeyboardInterrupt:
                    self.logger.info("Sleep interrupted by user")
                    raise
        elif remaining < 20:  # Warning when getting close
            self.logger.info(f"Rate limit status: {remaining} requests remaining")
    
    def generate_documentation(self, all_issues: Dict[str, List[Dict[str, Any]]]) -> None:
        """Generate all documentation files."""
        if not all_issues and not self.dry_run:
            self.logger.warning("No issues found to generate documentation")
            return
        
        # Generate overview files
        self._generate_overview_files(all_issues)
        
        # Generate repository-specific files
        for repo_name, issues in all_issues.items():
            self._generate_repository_files(repo_name, issues)
        
        # Update MkDocs navigation
        self._update_mkdocs_navigation(all_issues)
    
    def _generate_repository_files(self, repo_name: str, issues: List[Dict[str, Any]]) -> None:
        """Generate files for a repository."""
        base_dir = Path('simtestlab') / self.config['output']['base_dir']
        repo_dir = base_dir / 'repositories' / repo_name
        self._ensure_directory_exists(repo_dir)
        
        # Generate repository index
        self._generate_repository_index(repo_name, issues, repo_dir)
        
        # Generate individual issue files
        for issue in issues:
            self._generate_issue_file(issue, repo_name, repo_dir)
    
    def _generate_repository_index(self, repo_name: str, issues: List[Dict[str, Any]], output_dir: Path) -> str:
        """Generate an index file for a repository."""
        index_path = output_dir / 'index.md'
        
        # Group by state and calculate stats
        open_issues = [issue for issue in issues if issue['state'] == 'open']
        closed_issues = [issue for issue in issues if issue['state'] == 'closed']
        total_count = len(issues)
        
        lines = [
            f"# {repo_name}",
            "",
            f"**{total_count}** issues • **{len(open_issues)}** open • **{len(closed_issues)}** closed",
            "",
        ]
        
        if issues:
            # Sort all issues by updated date (most recent first)
            sorted_issues = sorted(issues, key=lambda x: x.get('updated_at', x['created_at']), reverse=True)
            
            lines.extend([
                '<div class="github-issue-table-container">',
                '<table class="github-issue-table">',
                '<thead>',
                '<tr>',
                '<th>Status</th>',
                '<th>Issue</th>',
                '<th>Title</th>',
                '<th>Assignee</th>',
                '<th>Labels</th>',
                '<th>Updated</th>',
                '</tr>',
                '</thead>',
                '<tbody>'
            ])
            
            for issue in sorted_issues:
                # Status badge
                if issue['state'] == 'open':
                    status = "🟢 Open"
                else:
                    status = "🔴 Closed"
                
                # Format title
                title = issue['title']
                if len(title) > 50:
                    title = title[:47] + "..."
                
                # Format assignees
                assignees = []
                if issue.get('assignees'):
                    assignees = [assignee['login'] for assignee in issue['assignees'][:2]]
                elif issue.get('assignee'):
                    assignees = [issue['assignee']['login']]
                
                assignee_str = ", ".join(assignees) if assignees else "-"
                if issue.get('assignees') and len(issue['assignees']) > 2:
                    assignee_str += f" +{len(issue['assignees']) - 2}"
                
                # Format labels
                labels = [label['name'] for label in issue.get('labels', [])[:3]]
                label_str = ", ".join(labels) if labels else "-"
                if issue.get('labels') and len(issue['labels']) > 3:
                    label_str += f" +{len(issue['labels']) - 3}"
                
                # Format updated date
                updated_date = issue.get('updated_at', issue['created_at'])[:10]
                
                # Create GitHub issue link
                github_url = issue.get('html_url', f"https://github.com/{self.config['github']['organization']}/{repo_name}/issues/{issue['number']}")
                
                lines.append(f"<tr><td>{status}</td><td><a href='{github_url}' target='_blank'>#{issue['number']}</a></td><td>{title}</td><td>{assignee_str}</td><td>{label_str}</td><td>{updated_date}</td></tr>")
            
            lines.extend([
                '</tbody>',
                '</table>',
                '</div>'
            ])
        else:
            lines.extend([
                "",
                "No issues found for this repository.",
            ])
        
        lines.extend([
            "",
            "---",
            "",
            f"Last updated: {datetime.now(timezone.utc).strftime('%Y-%m-%d %H:%M UTC')}",
        ])
        
        content = "\n".join(lines)
        
        if self.dry_run:
            self.logger.debug(f"DRY RUN: Would create repository index: {index_path}")
        else:
            with open(index_path, 'w', encoding='utf-8') as f:
                f.write(content)
        
        self.stats['files_created'] += 1
        return str(index_path.relative_to(Path('simtestlab')))
    
    def _generate_overview_files(self, all_issues: Dict[str, List[Dict[str, Any]]]) -> None:
        """Generate overview files for all issues."""
        base_dir = Path('simtestlab') / self.config['output']['base_dir']
        
        # Generate main overview
        overview_dir = base_dir / 'overview'
        self._ensure_directory_exists(overview_dir)
        
        # Collect all issues across repositories for table view
        all_issues_list = []
        total_issues = 0
        total_open = 0
        total_closed = 0
        
        for repo_name, issues in all_issues.items():
            for issue in issues:
                # Extract assignees
                assignees = []
                if issue.get('assignees'):
                    assignees = [assignee['login'] for assignee in issue['assignees']]
                elif issue.get('assignee'):
                    assignees = [issue['assignee']['login']]
                
                # Extract labels
                labels = [label['name'] for label in issue.get('labels', [])]
                
                all_issues_list.append({
                    'repository': repo_name,
                    'number': issue['number'],
                    'title': issue['title'],
                    'state': issue['state'],
                    'assignees': assignees,
                    'labels': labels,
                    'created_at': issue['created_at'],
                    'updated_at': issue['updated_at'],
                    'html_url': issue['html_url']
                })
                
                total_issues += 1
                if issue['state'] == 'open':
                    total_open += 1
                else:
                    total_closed += 1
        
        # Sort issues by updated date (most recent first)
        all_issues_list.sort(key=lambda x: x['updated_at'], reverse=True)
        
        lines = [
            "# GitHub Issues Dashboard",
            "",
            f"**{len(all_issues)}** repositories • **{total_issues}** issues • **{total_open}** open • **{total_closed}** closed",
            "",
            '<div class="github-issue-table-container">',
            '<table class="github-issue-table">',
            '<thead>',
            '<tr>',
            '<th>Status</th>',
            '<th>Repository</th>',
            '<th>Issue</th>',
            '<th>Title</th>',
            '<th>Assignee</th>',
            '<th>Labels</th>',
            '<th>Updated</th>',
            '</tr>',
            '</thead>',
            '<tbody>'
        ]
        
        # Add issue table rows
        for issue in all_issues_list:
            # Status badge
            if issue['state'] == 'open':
                status = "🟢 Open"
            else:
                status = "🔴 Closed"
            
            # Truncate title if too long
            title = issue['title']
            if len(title) > 50:
                title = title[:47] + "..."
            
            # Format assignees
            assignee_str = ", ".join(issue['assignees'][:2]) if issue['assignees'] else "-"
            if len(issue['assignees']) > 2:
                assignee_str += f" +{len(issue['assignees']) - 2}"
            
            # Format labels
            label_str = ", ".join(issue['labels'][:3]) if issue['labels'] else "-"
            if len(issue['labels']) > 3:
                label_str += f" +{len(issue['labels']) - 3}"
            
            # Format updated date
            updated_date = issue['updated_at'][:10]  # YYYY-MM-DD format
            
            # Create repository link to local repository page
            repo_link = f"<a href='../repositories/{issue['repository']}/'>{issue['repository']}</a>"
            
            lines.append(f"<tr><td>{status}</td><td>{repo_link}</td><td><a href='{issue['html_url']}' target='_blank'>#{issue['number']}</a></td><td>{title}</td><td>{assignee_str}</td><td>{label_str}</td><td>{updated_date}</td></tr>")
        
        lines.extend([
            '</tbody>',
            '</table>',
            '</div>',
            "",
            "---",
            "",
            f"Last updated: {datetime.now(timezone.utc).strftime('%Y-%m-%d %H:%M UTC')}",
        ])
        
        content = "\n".join(lines)
        
        if self.dry_run:
            self.logger.debug(f"DRY RUN: Would create overview: {overview_dir / 'index.md'}")
        else:
            overview_path = overview_dir / 'index.md'
            with open(overview_path, 'w', encoding='utf-8') as f:
                f.write(content)
        
        self.stats['files_created'] += 1
        self.logger.info(f"Generated overview file: {overview_dir / 'index.md'}")
    
    def _generate_issue_file(self, issue: Dict[str, Any], repo_name: str, output_dir: Path) -> str:
        """Generate a markdown file for a single issue."""
        filename = self._format_filename(issue, repo_name)
        issue_path = output_dir / filename
        
        lines = [
            f"# Issue #{issue['number']}: {issue['title']}",
            "",
            f"**Repository:** {repo_name}  ",
            f"**Status:** {issue['state'].title()}  ",
            f"**Created:** {self._format_date(issue['created_at'])}  ",
        ]
        
        if issue.get('updated_at'):
            lines.append(f"**Updated:** {self._format_date(issue['updated_at'])}  ")
        
        if issue.get('closed_at'):
            lines.append(f"**Closed:** {self._format_date(issue['closed_at'])}  ")
        
        if issue.get('user'):
            lines.append(f"**Author:** @{issue['user']['login']}  ")
        
        if issue.get('assignees'):
            assignees = [f"@{assignee['login']}" for assignee in issue['assignees']]
            lines.append(f"**Assignees:** {', '.join(assignees)}  ")
        
        if issue.get('labels'):
            labels = [f"`{label['name']}`" for label in issue['labels']]
            lines.append(f"**Labels:** {', '.join(labels)}  ")
        
        lines.extend([
            "",
            f"[View on GitHub]({issue['html_url']})",
            "",
            "## Description",
            "",
        ])
        
        if issue.get('body'):
            lines.append(issue['body'])
        else:
            lines.append("*No description provided.*")
        
        content = "\n".join(lines)
        
        if self.dry_run:
            self.logger.debug(f"DRY RUN: Would create issue file: {issue_path}")
        else:
            with open(issue_path, 'w', encoding='utf-8') as f:
                f.write(content)
        
        self.stats['files_created'] += 1
        return str(issue_path.relative_to(Path('simtestlab')))
    
    def _update_mkdocs_navigation(self, all_issues: Dict[str, List[Dict[str, Any]]]) -> None:
        """Update mkdocs.yml navigation to include GitHub Issues."""
        if not self.config['mkdocs']['auto_update_nav']:
            self.logger.info("MkDocs navigation auto-update is disabled")
            return
        
        mkdocs_path = Path('mkdocs.yml')
        if not mkdocs_path.exists():
            self.logger.warning("mkdocs.yml not found, skipping navigation update")
            return
        
        try:
            nav_title = self.config['mkdocs']['nav_title']
            base_dir = self.config['output']['base_dir']
            
            # Read the current mkdocs.yml as text to preserve formatting and special tags
            with open(mkdocs_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Create the GitHub Issues navigation section as text
            nav_section = f"  - {nav_title}:\n"
            nav_section += f"      - Overview: {base_dir}/overview/index.md\n"
            if all_issues:
                nav_section += "      - Repositories:\n"
                for repo_name in sorted(all_issues.keys()):
                    nav_section += f"          - {repo_name}: {base_dir}/repositories/{repo_name}/index.md\n"
            
            # Remove existing GitHub Issues section if it exists
            lines = content.split('\n')
            new_lines = []
            skip_github_section = False
            github_indent_level = 0
            
            for line in lines:
                # Check if this is the start of a GitHub Issues section
                if f"- {nav_title}:" in line:
                    skip_github_section = True
                    github_indent_level = len(line) - len(line.lstrip())
                    continue
                
                # If we're skipping the GitHub section, check if we've reached the end
                if skip_github_section:
                    current_indent = len(line) - len(line.lstrip()) if line.strip() else github_indent_level + 1
                    # If we find a line at the same or lower indent level (that's not empty), we've left the GitHub section
                    if line.strip() and current_indent <= github_indent_level:
                        skip_github_section = False
                        new_lines.append(line)
                    # Skip lines that are part of the GitHub section
                    continue
                
                new_lines.append(line)
            
            # Find the nav section and add GitHub Issues after Help
            final_lines = []
            nav_found = False
            help_found = False
            github_added = False
            
            for i, line in enumerate(new_lines):
                final_lines.append(line)
                
                # Check if we're in the nav section
                if line.strip() == 'nav:' or line.strip().startswith('nav:'):
                    nav_found = True
                
                # If we're in nav and found Help, add GitHub Issues after it
                if nav_found and not github_added and '- Help:' in line:
                    help_found = True
                    # Add the GitHub Issues section after Help
                    final_lines.append(nav_section.rstrip())
                    github_added = True
                
                # If we reach the end of nav section without finding Help, add GitHub Issues at the start of nav
                elif nav_found and not github_added and line.strip() and not line.startswith('  ') and not line.startswith('nav'):
                    # We've left the nav section, add GitHub Issues before this line
                    final_lines.insert(-1, nav_section.rstrip())
                    github_added = True
            
            # If GitHub Issues wasn't added yet (nav section might be at the end), add it
            if nav_found and not github_added:
                # Find where to insert in the nav section
                for i in range(len(final_lines) - 1, -1, -1):
                    if final_lines[i].strip() == 'nav:' or final_lines[i].strip().startswith('nav:'):
                        final_lines.insert(i + 1, nav_section.rstrip())
                        break
            
            # Write back to mkdocs.yml if not in dry run mode
            if not self.dry_run:
                with open(mkdocs_path, 'w', encoding='utf-8') as f:
                    f.write('\n'.join(final_lines))
                self.logger.info("Successfully updated mkdocs.yml navigation")
            else:
                self.logger.info("Dry run: mkdocs.yml navigation would be updated")
                
        except Exception as e:
            self.logger.error(f"Failed to update mkdocs.yml navigation: {e}")
            self.logger.info("Manual navigation update required for mkdocs.yml")
            self.logger.info(f"Add the following section to your mkdocs.yml nav:")
            
            nav_title = self.config['mkdocs']['nav_title']
            base_dir = self.config['output']['base_dir']
            
            print(f"\n{nav_title}:")
            print(f"  - Overview: {base_dir}/overview/index.md")
            if all_issues:
                print("  - Repositories:")
                for repo_name in sorted(all_issues.keys()):
                    print(f"    - {repo_name}: {base_dir}/repositories/{repo_name}/index.md")
            print()
    
    def run(self) -> None:
        """Main execution method."""
        start_time = time.time()
        
        self.logger.info("Starting GitHub Issues documentation generation")
        self.logger.info(f"Configuration: {self.config_path}")
        self.logger.info(f"Dry run: {self.dry_run}")
        
        try:
            # Fetch all issues
            all_issues = self.fetch_issues()
            
            if not all_issues:
                self.logger.warning("No issues fetched, nothing to generate")
                return
            
            # Generate documentation
            self.generate_documentation(all_issues)
            
            # Print summary
            elapsed_time = time.time() - start_time
            self.logger.info(f"\n{'='*60}")
            self.logger.info("GENERATION COMPLETE")
            self.logger.info(f"{'='*60}")
            self.logger.info(f"Repositories processed: {self.stats['repositories_processed']}")
            self.logger.info(f"Issues fetched: {self.stats['issues_fetched']}")
            self.logger.info(f"Files created: {self.stats['files_created']}")
            self.logger.info(f"Execution time: {elapsed_time:.2f} seconds")
            
            if self.stats['errors']:
                self.logger.warning(f"Errors encountered: {len(self.stats['errors'])}")
                for error in self.stats['errors']:
                    self.logger.warning(f"  - {error}")
            
            self.logger.info(f"{'='*60}")
            
        except Exception as e:
            self.logger.error(f"Fatal error: {e}")
            sys.exit(1)


def main():
    """Main function."""
    parser = argparse.ArgumentParser(
        description="Fetch GitHub issues and generate MkDocs documentation"
    )
    parser.add_argument(
        '--config', '-c',
        default='config/github_issues.yml',
        help='Path to configuration file (default: config/github_issues.yml)'
    )
    parser.add_argument(
        '--dry-run',
        action='store_true',
        help='Perform a dry run without creating files'
    )
    parser.add_argument(
        '--verbose', '-v',
        action='store_true',
        help='Enable verbose logging'
    )
    parser.add_argument(
        '--check-rate-limit',
        action='store_true',
        help='Check current GitHub API rate limit status and exit'
    )
    
    args = parser.parse_args()
    
    # Initialize the fetcher
    fetcher = GitHubIssuesFetcher(
        config_path=args.config,
        dry_run=args.dry_run,
        verbose=args.verbose
    )
    
    # Check rate limit if requested
    if args.check_rate_limit:
        fetcher.check_rate_limit_status()
        return
    
    fetcher.run()


if __name__ == "__main__":
    main()