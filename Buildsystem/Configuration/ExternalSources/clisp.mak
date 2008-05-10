# Oliver Kullmann, 19.12.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Clisp

clisp_recommended_version_number_okl ?= 2.44.1
clisp_supported_not_recommended_version_numbers_okl ?= 2.43
clisp_supported_version_numbers_okl ?= $(clisp_supported_not_recommended_version_numbers_okl) $(clisp_recommended_version_number_okl)

clisp_prefix_okl ?= clisp

clisp_recommended_package_name_okl ?= $(clisp_prefix_okl)-$(clisp_recommended_version_number_okl)

# clisp_html_template_okl ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Clisp.html
# clisp_html_output_okl ?= $(local_html_dir)/Clisp.html
# clisp_html_documentation_index_location_okl ?= Clisp.html

clisp_base_installation_dir_okl ?= $(ExternalSources_installations)/CLisp
clisp_installation_dir_okl ?= $(clisp_base_installation_dir_okl)/$(clisp_recommended_version_number_okl)
clisp_base_build_dir_okl ?= $(ExternalSources_builds)/CLisp
clisp_build_dir_okl ?= $(clisp_base_build_dir_okl)/$(clisp_recommended_package_name_okl)
clisp_base_doc_dir_okl ?= $(ExternalSources_doc)/CLisp
clisp_doc_dir_okl ?= $(clisp_base_doc_dir_okl)/$(clisp_recommended_version_number_okl)

clisp_call_okl ?= clisp

clisp_docu_page_okl ?= $(doc_dir)/doxygen_html/df/dfc/CLisp_8hpp.html

clisp_homepage_url_okl := http://www.clisp.org/
clisp_documentation_url_okl := http://clisp.sourceforge.net/resources.html

clisp_targets_prefix_okl := $(clisp_prefix_okl)-
clisp_targets_okl := $(addprefix $(clisp_targets_prefix_okl), $(clisp_supported_version_numbers_okl))
clisp_recommended_okl := $(clisp_targets_prefix_okl)$(clisp_recommended_version_number_okl)

# Tool: libsigsegv

libsigsegv_recommended_version_number_okl ?= 2.5
libsigsegv_supported_not_recommended_version_numbers_okl ?= 
libsigsegv_supported_version_numbers_okl ?= $(libsigsegv_supported_not_recommended_version_numbers_okl) $(libsigsegv_recommended_version_number_okl)

libsigsegv_prefix_okl ?= libsigsegv

libsigsegv_recommended_package_name_okl ?= $(libsigsegv_prefix_okl)-$(libsigsegv_recommended_version_number_okl)

libsigsegv_base_build_dir_okl ?= $(clisp_base_build_dir_okl)/Libsigsegv
libsigsegv_build_dir_okl ?= $(libsigsegv_base_build_dir_okl)/$(libsigsegv_recommended_package_name_okl)

libsigsegv_homepage_url_okl := http://

libsigsegv_targets_prefix_okl := $(libsigsegv_prefix_okl)-
libsigsegv_targets_okl := $(addprefix $(libsigsegv_targets_prefix_okl), $(libsigsegv_supported_version_numbers_okl))
libsigsegv_recommended_okl := $(libsigsegv_targets_prefix_okl)$(libsigsegv_recommended_version_number_okl)
