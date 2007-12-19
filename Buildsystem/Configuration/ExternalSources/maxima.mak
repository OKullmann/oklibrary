# Oliver Kullmann, 18.12.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Maxima

maxima_recommended_version_number ?= 5.13
maxima_supported_not_recommended_version_numbers ?= 5.12
maxima_supported_version_numbers ?= $(maxima_supported_not_recommended_version_numbers) $(maxima_recommended_version_number)

maxima_prefix ?= maxima

maxima_recommended_package_name ?= $(maxima_prefix)-$(maxima_recommended_version_number)

maxima_html_template ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Maxima.html
maxima_html_output ?= $(local_html_dir)/Maxima.html
maxima_html_documentation_index_location ?= Maxima.html

maxima_base_directory ?= $(ExternalSources_installations)/Maxima
maxima_installation_dir ?= $(maxima_base_directory)/$(maxima_recommended_version_number)
maxima_build_dir ?= $(ExternalSources_builds)/Maxima/$(maxima_recommended_package_name)
maxima_doc_dir ?= $(ExternalSources_doc)/Maxima/$(maxima_recommended_version_number)

maxima_call ?= $(maxima_installation_dir)/bin/maxima
maxima_main_index ?= $(maxima_installation_dir)/doc/index.html

maxima_docu_page ?= $(doc_dir)/doxygen_html/

maxima_homepage_url := http://
maxima_documentation_url := http://

maxima_targets_prefix := $(maxima_prefix)-
maxima_targets := $(addprefix $(maxima_targets_prefix), $(maxima_supported_version_numbers))
maxima_recommended := $(maxima_targets_prefix)$(maxima_recommended_version_number)

