# Oliver Kullmann, 10.5.2010 (Swansea)
# Copyright 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using the ArgoSAT library and solver

argosat_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Argosat
argosat_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Argosat
ecl_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/Argosat

argosat_recommended_version_number_okl ?= 1.0.1
argosat_other_versions_okl ?= 1.0

argosat_prefix_okl ?= argosat-
argosat_extracted_archive_okl ?= $(argosat_prefix_okl)$(argosat_recommended_version_number_okl)

argosat_build_dir_okl ?= $(argosat_base_build_dir_okl)/$(argosat_extracted_archive_okl)
argosat_installation_dir_okl ?= $(argosat_base_installation_dir_okl)/$(argosat_recommended_version_number_okl)
ecl_doc_dir_okl ?= $(argosat_base_doc_dir_okl)/$(argosat_recommended_version_number_okl)

argosat_docu_page_okl ?= $(doc_dir)/doxygen_html/

argosat_homepage_url_okl := http://argo.matf.bg.ac.yu/index.html

argosat_source_okl := $(ExternalSources)/sources/SAT/Argo/$(argosat_extracted_archive_okl)

argosat_call_okl ?= $(argosat_installation_dir_okl)/bin/argosat
argosat_source_library_okl ?= $(argosat_installation_dir_okl)/include
argosat_include_option_okl ?= -I $(argosat_source_library_okl)
argosat_link_option_okl ?= -L $(argosat_installation_dir_okl)/lib -Wl,-rpath,$(argosat_installation_dir_okl)/lib -largosat
