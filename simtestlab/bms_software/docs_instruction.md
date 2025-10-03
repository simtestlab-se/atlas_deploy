# MkDocs Project Setup and Documentation Guide

## Quick Start: Run Development Environment and MkDocs Server

1. **Activate the Development Environment**:
   Run the development environment file:
   ```cmd
   DevEnvironment
   ```
2. **Start the MkDocs Server**
    ```cmd
    mkdocs serve
    ```

    (or[text](Specifications))



## 1. Environment Setup

Run the setup script to configure your environment:

```cmd
setup.bat
```

This script will:
- Configure the Python environment
- Set up virtual environment paths
- Configure necessary environment variables

## 2. Install Dependencies

Install the required Python packages:

```cmd
pip install -r requirements.txt
```

This will install:
- `MkDocs` (>=1.6.1)
- `MkDocs Material` theme (>=9.5.0)

## 3. Serving Documentation Locally

To preview the documentation locally, run:

```cmd
mkdocs serve
```

Open a web browser and navigate to `http://127.0.0.1:8000` to view your documentation.

## 4. Writing Documentation

All documentation files should be placed in the `docs/` directory:

```
docs/
├── index.md           # Homepage (required)
├── Folder 1/            
│   └── page 1
├── Folder 2/            
│   └── page 1   
├── assets/            # Images
│   └── Images/
└── Specifications/          # Organized content sections
    ├── ContactorControl.md
    └── LFP_Soc.md
```

### Tips for Writing:
- Use Markdown syntax for formatting content.

- Treat each folder as a page, and consider each file within the folder as a topic of that - page.

- Store all images in docs/assets/images/.

- For longer content, create subfolders under sections/ to keep it organized.


## 5. Building the Static Site

When ready to deploy, generate the static site:

```cmd
mkdocs build
```

The static HTML site will be available in the `site/` directory for hosting.

## References
- [MkDocs Documentation](https://www.mkdocs.org/)
- [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/)
