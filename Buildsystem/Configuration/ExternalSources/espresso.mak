# Oliver Kullmann, 30.11.2010 (Swansea)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using Espresso

espresso_recommended_version_number_okl ?= 2.3
espresso_other_versions_okl ?=

espresso_src_base_name_okl := espresso-ab-1.0
espresso_base_name_okl := espresso
espresso_src_recommended_okl := $(espresso_src_base_name_okl)-$(espresso_recommended_version_number_okl)
espresso_recommended_okl := $(espresso_base_name_okl)-$(espresso_recommended_version_number_okl)
espresso_base_source_okl := $(ExternalSources)/sources/Boolean/Espresso
espresso_source_okl := $(espresso_base_source_okl)/$(espresso_src_recommended_okl)

espresso_base_build_dir_okl ?= $(ExternalSources_builds)/Boolean/Espresso
espresso_build_dir_okl ?= $(espresso_base_build_dir_okl)/$(espresso_src_base_name_okl)

espresso_base_installation_dir_okl ?= $(ExternalSources_installations)/Boolean/Espresso
espresso_installation_dir_okl ?= $(espresso_base_installation_dir_okl)/$(espresso_recommended_version_number_okl)

espresso_exec_okl ?= espresso
espresso_call_okl ?= $(espresso_installation_dir_okl)/$(espresso_exec_okl)
espresso_public_call_okl ?= $(espresso_exec_okl)$(espresso_recommended_version_number_okl)

espresso_base_doc_dir_okl ?= $(ExternalSources_doc)/Boolean/Espresso
espresso_doc_dir_okl ?= $(espresso_base_doc_dir_okl)/$(espresso_recommended_okl)
espresso_docu_page_okl ?= $(doc_dir)/doxygen_html/da/dd1/Espresso_8hpp.html

espresso_homepage_url_okl := http://en.wikipedia.org/wiki/Espresso_heuristic_logic_minimizer
espresso_documentation_url_okl := http://bear.cwru.edu/eecs_cad/man_octtools_espresso.html

