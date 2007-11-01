# Oliver Kullmann, 16.8.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Git

git_recommended_version_number ?= 1.5.3.5
git_supported_not_recommended_version_numbers ?= 1.5.3.4
git_supported_version_numbers ?= $(git_recommended_version_number) $(git_supported_not_recommended_version_numbers)

git_prefix ?= git

git_base_directory ?= $(ExternalSources)/Git

git_documentation_dir ?= $(ExternalSources_doc)/Git
git_html_documentation_index_location ?= $(git_documentation_dir)/Documentation/git.html
git_docu_page ?= $(doc_dir)/doxygen_html/d7/d77/docus_2Git_8hpp.html

git_homepage_url := http://git.or.cz/
git_documentation_url := http://git.or.cz/\#documentation

git_targets_prefix := $(git_prefix)-
git_targets := $(addprefix $(git_targets_prefix), $(git_supported_version_numbers))
git_recommended := $(git_targets_prefix)$(git_recommended_version_number)
