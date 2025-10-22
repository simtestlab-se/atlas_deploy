# Issue #10: Publish GitHub Organization Issues in MkDocs

**Repository:** atlas  
**Status:** Open  
**Created:** 2025-10-05  
**Updated:** 2025-10-17  
**Author:** @aeroramesh  
**Assignees:** @nallasivamselvaraj  

[View on GitHub](https://github.com/Simtestlab/atlas/issues/10)

## Description

User Story: Publish GitHub Organization Issues in MkDocs

Title: As a team member, I want to automatically publish all GitHub issues from our organization into MkDocs so that the documentation site always reflects the latest issues.

Description:
Currently, issues from our GitHub repositories are scattered and hard to track in our documentation. We want to automate the process of fetching all issues from all repositories in the organization and generating markdown files that can be rendered by MkDocs. The generated markdowns should include issue title, repository, state, creation date, URL, and description.

Acceptance Criteria:

A script/tool fetches all issues from all repositories in the GitHub organization, excluding pull requests.

Each issue is converted into a markdown (.md) file with:

Issue title

Repository name

Issue state (open/closed)

Creation date

URL link to GitHub

Issue description/body

The markdown files are saved in a structured folder (e.g., docs/github_issues/).

mkdocs.yml is updated automatically or can be regenerated to include all issues in the navigation, grouped by repository.

The solution should support periodic updates (e.g., via GitHub Actions) to keep MkDocs synchronized with GitHub issues.

Proper error handling for API limits and pagination.

Notes / Technical Details:

Use GitHub REST API or GitHub CLI.

Use Python for automation (or another preferred language).

Ensure markdown filenames are sanitized for special characters.

Optional: filter issues by labels, state, or date if needed.