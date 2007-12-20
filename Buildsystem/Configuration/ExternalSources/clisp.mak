# Oliver Kullmann, 19.12.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Clisp

clisp_recommended_version_number ?= 2.43
clisp_supported_not_recommended_version_numbers ?= 
clisp_supported_version_numbers ?= $(clisp_supported_not_recommended_version_numbers) $(clisp_recommended_version_number)

clisp_prefix ?= clisp

clisp_recommended_package_name ?= $(clisp_prefix)-$(clisp_recommended_version_number)

clisp_html_template ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Clisp.html
clisp_html_output ?= $(local_html_dir)/Clisp.html
clisp_html_documentation_index_location ?= Clisp.html

clisp_base_directory ?= $(ExternalSources_installations)/Clisp
clisp_installation_dir ?= $(clisp_base_directory)/$(clisp_recommended_version_number)
clisp_build_dir ?= $(ExternalSources_builds)/Clisp/$(clisp_recommended_package_name)
clisp_doc_dir ?= $(ExternalSources_doc)/Clisp/$(clisp_recommended_version_number)

clisp_call ?= $(clisp_installation_dir)/bin/clisp
clisp_main_index ?= $(clisp_installation_dir)/doc/index.html

clisp_docu_page ?= $(doc_dir)/doxygen_html/

clisp_homepage_url := http://www.clisp.org/
clisp_documentation_url := http://clisp.sourceforge.net/resources.html

clisp_targets_prefix := $(clisp_prefix)-
clisp_targets := $(addprefix $(clisp_targets_prefix), $(clisp_supported_version_numbers))
clisp_recommended := $(clisp_targets_prefix)$(clisp_recommended_version_number)

