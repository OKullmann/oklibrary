# Oliver Kullmann, 26.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using R

R_recommended_version_number ?= 2.5.0
R_supported_not_recommended_version_numbers ?= 
R_supported_version_numbers ?= $(R_supported_not_recommended_version_numbers) $(R_recommended_version_number)

R_prefix ?= R

R_base_directory ?= $(ExternalSources)/R

R_documentation_dir ?= $(ExternalSources_doc)/R
R_html_documentation_index_location ?= $(R_documentation_dir)/doc/html/index.html
R_docu_page ?= $(doc_dir)//doxygen_html/da/d4d/docus_2R_8hpp.html

R_homepage_url := http://www.r-project.org/
R_documentation_url := http://www.r-project.org/

R_targets_prefix := $(R_prefix)-
R_targets := $(addprefix $(R_targets_prefix), $(R_supported_version_numbers))
R_recommended := $(R_targets_prefix)$(R_recommended_version_number)
