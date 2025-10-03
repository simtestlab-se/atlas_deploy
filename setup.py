
with open("README.md", "r") as fh:
    long_description = fh.read()

with open('requirements.txt') as f:
    requirements = f.read().splitlines()

reqs = [req for req in requirements]
name="Stl",
version='0.0.1',
author="Simtestlab Sweden AB",
author_email="info.simtestlab@gmail.com",
description="simtestlab work dirctory",
long_description=long_description,
long_description_content_type="text/markdown",
install_requires=reqs,
include_package_data=True,
