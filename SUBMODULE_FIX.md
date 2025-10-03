# Submodule Issues Resolution Guide

## Current Issue
The GitHub Actions workflow is failing because the submodule `simtestlab/bms_software` points to a repository that doesn't exist or isn't accessible:
- Repository: `https://github.com/Simtestlab/bms_master.git`
- Error: `fatal: repository 'https://github.com/Simtestlab/bms_master.git/' not found`

## Solution Options

### Option 1: Fix the Submodule Repository (Recommended)

If you have the BMS software code but the repository is missing:

1. **Create the missing repository:**
   ```bash
   # Create a new repository on GitHub named 'bms_master'
   # Or rename an existing repository to match the URL
   ```

2. **Update submodule URL if repository has different name:**
   ```bash
   # If your repository is named differently, update .gitmodules
   git config submodule.simtestlab/bms_master.url https://github.com/Simtestlab/correct-repo-name.git
   git submodule sync
   git add .gitmodules
   git commit -m "Update submodule URL"
   ```

### Option 2: Remove the Submodule (If not needed)

If you don't need the BMS software as a submodule:

1. **Remove the submodule:**
   ```bash
   git submodule deinit simtestlab/bms_software
   git rm simtestlab/bms_software
   rm -rf .git/modules/simtestlab/bms_software
   git commit -m "Remove bms_software submodule"
   ```

2. **Clean up .gitmodules:**
   ```bash
   # The .gitmodules file will be updated automatically
   ```

### Option 3: Convert Submodule to Regular Directory

If you want to include the BMS software directly in this repository:

1. **Remove submodule and copy files:**
   ```bash
   git submodule deinit simtestlab/bms_software
   git rm --cached simtestlab/bms_software
   rm -rf .git/modules/simtestlab/bms_software
   
   # Copy your BMS software files directly into simtestlab/bms_software/
   # Then commit as regular files
   git add simtestlab/bms_software/
   git commit -m "Convert BMS software from submodule to direct inclusion"
   ```

### Option 4: Update Submodule to Point to Existing Repository

If you have the BMS code in a different repository:

1. **Update .gitmodules file:**
   ```bash
   # Edit .gitmodules and change the URL to the correct repository
   git config submodule.simtestlab/bms_master.url https://github.com/Simtestlab/actual-repo-name.git
   git submodule sync
   git submodule update --init --recursive
   git add .gitmodules
   git commit -m "Update submodule to correct repository"
   ```

## Immediate Workaround (Already Implemented)

I've updated the GitHub Actions workflow to handle this gracefully:
- The workflow now continues even if submodules fail to initialize
- Creates a placeholder documentation file if the BMS software is missing
- This allows your deployment to proceed while you fix the submodule issue

## Current Workflow Changes

The workflow now includes error handling for submodules:

```yaml
- name: Initialize submodules (with error handling)
  run: |
    git submodule sync --recursive || true
    git submodule update --init --recursive || echo "Warning: Some submodules failed to initialize"
    
    if [ ! -d "simtestlab/bms_software" ] || [ ! "$(ls -A simtestlab/bms_software 2>/dev/null)" ]; then
      echo "Creating placeholder for missing bms_software submodule"
      mkdir -p simtestlab/bms_software/docs
      echo "# BMS Software Documentation" > simtestlab/bms_software/docs/index.md
      echo "This section is under development." >> simtestlab/bms_software/docs/index.md
    fi
```

## Recommended Action Plan

1. **Immediate:** The updated workflow should now allow deployment to proceed
2. **Short-term:** Choose one of the options above based on your needs
3. **Long-term:** Ensure all submodules point to accessible repositories

## Testing the Fix

After implementing any of the above solutions, test by:

1. Pushing changes to trigger the GitHub Actions
2. Verifying the workflow completes successfully
3. Checking that your site deploys correctly

Choose the option that best fits your project structure and requirements!