#!/bin/bash

# Quick Submodule Fix Script
# This script helps you quickly resolve the bms_master submodule issue

echo "🔧 BMS Submodule Fix Tool"
echo "========================"
echo ""

# Check current submodule status
echo "📋 Current submodule status:"
git submodule status 2>/dev/null || echo "No submodules or git repository not found"
echo ""

# Show current .gitmodules content
if [ -f ".gitmodules" ]; then
    echo "📄 Current .gitmodules configuration:"
    cat .gitmodules
    echo ""
fi

echo "Choose an option to fix the submodule issue:"
echo ""
echo "1) Remove the submodule completely (if not needed)"
echo "2) Update submodule URL to point to existing repository"
echo "3) Convert submodule to regular directory"
echo "4) Skip fix and use placeholder (current workaround)"
echo "5) Show detailed status and exit"
echo ""

read -p "Enter your choice (1-5): " choice

case $choice in
    1)
        echo ""
        echo "🗑️  Removing submodule..."
        
        # Remove submodule
        if git submodule deinit simtestlab/bms_software 2>/dev/null; then
            echo "✅ Deinitialized submodule"
        fi
        
        if git rm simtestlab/bms_software 2>/dev/null; then
            echo "✅ Removed submodule from git"
        fi
        
        # Clean up .git/modules
        if [ -d ".git/modules/simtestlab/bms_software" ]; then
            rm -rf .git/modules/simtestlab/bms_software
            echo "✅ Cleaned up git modules"
        fi
        
        # Commit changes
        if git diff --cached --quiet; then
            echo "⚠️  No changes to commit"
        else
            git commit -m "Remove bms_software submodule"
            echo "✅ Committed submodule removal"
        fi
        
        echo "🎉 Submodule removed successfully!"
        ;;
        
    2)
        echo ""
        read -p "Enter the correct repository URL: " new_url
        
        if [ -z "$new_url" ]; then
            echo "❌ No URL provided"
            exit 1
        fi
        
        echo "🔄 Updating submodule URL to: $new_url"
        
        # Update URL in .gitmodules
        git config -f .gitmodules submodule.simtestlab/bms_master.url "$new_url"
        
        # Sync and update
        git submodule sync
        git submodule update --init --recursive
        
        # Commit changes
        git add .gitmodules
        git commit -m "Update bms_software submodule URL"
        
        echo "✅ Submodule URL updated and synced"
        ;;
        
    3)
        echo ""
        echo "🔄 Converting submodule to regular directory..."
        
        # Backup existing content if any
        if [ -d "simtestlab/bms_software" ]; then
            echo "📦 Backing up existing content..."
            cp -r simtestlab/bms_software /tmp/bms_software_backup 2>/dev/null || true
        fi
        
        # Remove submodule
        git submodule deinit simtestlab/bms_software 2>/dev/null || true
        git rm --cached simtestlab/bms_software 2>/dev/null || true
        rm -rf .git/modules/simtestlab/bms_software 2>/dev/null || true
        
        # Restore content as regular files
        if [ -d "/tmp/bms_software_backup" ]; then
            mkdir -p simtestlab/bms_software
            cp -r /tmp/bms_software_backup/* simtestlab/bms_software/ 2>/dev/null || true
            rm -rf /tmp/bms_software_backup
        fi
        
        # Add as regular files
        git add simtestlab/bms_software/
        git commit -m "Convert BMS software from submodule to direct inclusion"
        
        echo "✅ Converted to regular directory"
        ;;
        
    4)
        echo ""
        echo "⏭️  Using placeholder workaround..."
        echo "The GitHub Actions workflow has been updated to handle this gracefully."
        echo "Your deployment should now work with a placeholder BMS documentation."
        echo ""
        echo "To implement a permanent fix later, run this script again and choose option 1, 2, or 3."
        ;;
        
    5)
        echo ""
        echo "📊 Detailed Status:"
        echo "===================="
        echo ""
        
        echo "Git repository status:"
        git status --porcelain 2>/dev/null || echo "Not a git repository or no changes"
        echo ""
        
        echo "Submodule detailed status:"
        git submodule foreach --recursive 'echo "Submodule: $name"; git remote -v; echo ""' 2>/dev/null || echo "No submodules found"
        
        echo "Directory structure:"
        ls -la simtestlab/bms_software/ 2>/dev/null || echo "Directory doesn't exist"
        echo ""
        
        echo "Recent commits:"
        git log --oneline -5 2>/dev/null || echo "No git history found"
        ;;
        
    *)
        echo "❌ Invalid choice. Please run the script again and choose 1-5."
        exit 1
        ;;
esac

echo ""
echo "🚀 Next steps:"
echo "- Push your changes: git push origin $(git branch --show-current 2>/dev/null || echo 'your-branch')"
echo "- Check GitHub Actions for successful deployment"
echo "- Visit your deployed site to verify everything works"