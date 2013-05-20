# Matthew Gwynne, 7.1.2011 (Swansea)
# Copyright 2011, 2012, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using CryptoMinisat2

cryptominisat_recommended_version_number_okl ?= 2.9.6
cryptominisat_other_versions_okl ?= 2.9.0

cryptominisat_base_name_okl := cmsat
cryptominisat_recommended_okl := $(cryptominisat_base_name_okl)-$(cryptominisat_recommended_version_number_okl)
cryptominisat_base_source_okl := $(ExternalSources)/sources/SAT/CryptoMiniSat
cryptominisat_source_okl := $(cryptominisat_base_source_okl)/$(cryptominisat_recommended_okl)

cryptominisat_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/CryptoMiniSat
cryptominisat_build_dir_okl ?= $(cryptominisat_base_build_dir_okl)/$(cryptominisat_recommended_okl)

cryptominisat_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/CryptoMiniSat
cryptominisat_installation_dir_okl ?= $(cryptominisat_base_installation_dir_okl)/$(cryptominisat_recommended_version_number_okl)

cryptominisat_exec_okl ?= cryptominisat
cryptominisat_call_okl ?= $(cryptominisat_installation_dir_okl)/$(cryptominisat_exec_okl)
cryptominisat_public_call_okl ?= $(cryptominisat_exec_okl)

cryptominisat_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/CryptoMiniSat
cryptominisat_doc_dir_okl ?= $(cryptominisat_base_doc_dir_okl)/$(cryptominisat_recommended_okl)
cryptominisat_docu_page_okl ?= $(doc_dir)/doxygen_html/

cryptominisat_homepage_url_okl := http://www.msoos.org/cryptominisat2
cryptominisat_documentation_url_okl := http://www.msoos.org/documentation/cryptominisat/
cryptominisat_git_url_okl := git://gitorious.org/cryptominisat/cryptominisat.git


# Settings for building and using CryptoMinisat3

cryptominisat3_recommended_version_number_okl ?= 3.2
cryptominisat3_other_versions_okl ?=

cryptominisat3_base_name_okl := cryptominisat
cryptominisat3_recommended_okl := $(cryptominisat3_base_name_okl)-$(cryptominisat3_recommended_version_number_okl)
cryptominisat3_base_source_okl := $(cryptominisat_base_source_okl)
cryptominisat3_source_okl := $(cryptominisat3_base_source_okl)/$(cryptominisat3_recommended_okl)

cryptominisat3_base_build_dir_okl ?= $(cryptominisat_base_build_dir_okl)
cryptominisat3_build_dir_okl ?= $(cryptominisat3_base_build_dir_okl)/$(cryptominisat3_recommended_okl)

cryptominisat3_base_installation_dir_okl ?= $(cryptominisat_base_installation_dir_okl)
cryptominisat3_installation_dir_okl ?= $(cryptominisat3_base_installation_dir_okl)/$(cryptominisat3_recommended_version_number_okl)

cryptominisat3_exec_okl ?= cryptominisat3
cryptominisat3_call_okl ?= $(cryptominisat3_installation_dir_okl)/$(cryptominisat3_exec_okl)
cryptominisat3_public_call_okl ?= $(cryptominisat3_exec_okl)

cryptominisat3_base_doc_dir_okl ?= $(cryptominisat_base_doc_dir_okl)
cryptominisat3_doc_dir_okl ?= $(cryptominisat3_base_doc_dir_okl)/$(cryptominisat3_recommended_okl)
cryptominisat3_docu_page_okl ?= $(doc_dir)/doxygen_html/

cryptominisat3_homepage_url_okl := http://www.msoos.org/cryptominisat2
cryptominisat3_documentation_url_okl := http://www.msoos.org/documentation/cryptominisat/


# Settings for building and using tool M4RI (for CryptoMinisat3)

m4ri_recommended_version_number_okl ?= 20130416
m4ri_other_versions_okl ?=

m4ri_base_name_okl := m4ri
m4ri_recommended_okl := $(m4ri_base_name_okl)-$(m4ri_recommended_version_number_okl)
m4ri_base_source_okl := $(cryptominisat3_base_source_okl)
m4ri_source_okl := $(m4ri_base_source_okl)/$(m4ri_recommended_okl)

m4ri_base_build_dir_okl ?= $(cryptominisat3_base_build_dir_okl)
m4ri_build_dir_okl ?= $(m4ri_base_build_dir_okl)/$(m4ri_recommended_okl)

m4ri_base_installation_dir_okl ?= $(cryptominisat3_base_installation_dir_okl)
m4ri_installation_dir_okl ?= $(m4ri_base_installation_dir_okl)/m4ri-$(m4ri_recommended_version_number_okl)

m4ri_base_doc_dir_okl ?= $(cryptominisat3_base_doc_dir_okl)
m4ri_doc_dir_okl ?= $(m4ri_base_doc_dir_okl)/$(m4ri_recommended_okl)
m4ri_docu_page_okl ?= $(doc_dir)/doxygen_html/

m4ri_homepage_url_okl := http://m4ri.sagemath.org/
m4ri_documentation_url_okl := http://m4ri.sagemath.org/m4ri-doxygen/index.html

m4ri_source_library_okl ?= $(m4ri_installation_dir_okl)/include
m4ri_include_option_okl ?= -I$(m4ri_source_library_okl)
m4ri_link_library_okl ?= $(m4ri_installation_dir_okl)/lib
m4ri_link_option_okl ?= -L$(m4ri_link_library_okl) -Wl,-rpath,$(m4ri_link_library_okl)
