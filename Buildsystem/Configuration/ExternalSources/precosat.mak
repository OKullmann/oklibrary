# Oliver Kullmann, 14.8.2009 (Swansea)
# Copyright 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Precosat

precosat_recommended_version_number_okl ?= -570-239dbbe-100801
precosat_other_version_numbers_okl ?= 236

precosat_recommended_okl := precosat$(precosat_recommended_version_number_okl)
precosat_base_source_okl := $(ExternalSources)/sources/SAT/Precosat
precosat_source_okl := $(precosat_base_source_okl)/$(precosat_recommended_okl)

precosat_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Precosat
precosat_build_dir_okl ?= $(precosat_base_build_dir_okl)/$(precosat_recommended_okl)

precosat_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Precosat
precosat_installation_dir_okl ?= $(precosat_base_installation_dir_okl)/$(precosat_recommended_okl)

precosat_exec_okl ?= precosat
precosat_call_okl ?= $(precosat_installation_dir_okl)/$(precosat_exec_okl)
precosat_public_call_okl ?= $(precosat_exec_okl)$(precosat_recommended_version_number_okl)

precosat_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/Precosat
precosat_doc_dir_okl ?= $(precosat_base_doc_dir_okl)/$(precosat_recommended_okl)
precosat_docu_page_okl ?= $(doc_dir)/doxygen_html/d8/d96/Precosat_8hpp.html

precosat_homepage_url_okl := http://fmv.jku.at/precosat/
precosat_documentation_url_okl := http://fmv.jku.at/precosat/preicosat-sc09.pdf

