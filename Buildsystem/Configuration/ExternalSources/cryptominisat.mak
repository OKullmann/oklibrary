# Matthew Gwynne, 7.1.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using CryptoMinisat

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
