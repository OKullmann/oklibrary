# Oliver Kullmann, 14.8.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Precosat

precosat_recommended_version_number_okl ?= -570.1
precosat_other_versions_okl ?= 236

precosat_recommended_okl := precosat$(precosat_recommended_version_number_okl)
precosat_base_source_okl := $(ExternalSources)/sources/SAT/Precosat
precosat_source_okl := $(precosat_base_source_okl)/$(precosat_recommended_okl)

precosat_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Precosat
precosat_build_dir_okl ?= $(precosat_base_build_dir_okl)/$(precosat_recommended_okl)

precosat_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Precosat
precosat_installation_dir_okl ?= $(precosat_base_installation_dir_okl)/$(precosat_recommended_version_number_okl)

precosat_exec_okl ?= precosat
precosat_call_okl ?= $(precosat_installation_dir_okl)/$(precosat_exec_okl)
precosat_public_call_okl ?= $(precosat_exec_okl)$(precosat_recommended_version_number_okl)

precosat_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/Precosat
precosat_doc_dir_okl ?= $(precosat_base_doc_dir_okl)/$(precosat_recommended_okl)
precosat_docu_page_okl ?= $(doc_dir)/doxygen_html/d8/d96/Precosat_8hpp.html

precosat_homepage_url_okl := http://fmv.jku.at/precosat/
precosat_documentation_url_okl := http://fmv.jku.at/precosat/preicosat-sc09.pdf


# Settings for building and using Lingeling and Plingeling

lingeling_recommended_version_number_okl ?= 587f-4882048-110513
lingeling_other_versions_okl ?= 276-6264d55-100731

lingeling_recommended_okl := lingeling-$(lingeling_recommended_version_number_okl)
lingeling_base_source_okl := $(ExternalSources)/sources/SAT/Precosat
lingeling_source_okl := $(lingeling_base_source_okl)/$(lingeling_recommended_okl)

lingeling_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Lingeling
lingeling_build_dir_okl ?= $(lingeling_base_build_dir_okl)/$(lingeling_recommended_okl)

lingeling_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Lingeling
lingeling_installation_dir_okl ?= $(lingeling_base_installation_dir_okl)/$(lingeling_recommended_version_number_okl)

lingeling_exec_okl ?= lingeling
lingeling_call_okl ?= $(lingeling_installation_dir_okl)/$(lingeling_exec_okl)
lingeling_public_call_okl ?= $(lingeling_exec_okl)$(lingeling_recommended_version_number_okl)
plingeling_exec_okl ?= plingeling
plingeling_call_okl ?= $(lingeling_installation_dir_okl)/$(plingeling_exec_okl)
plingeling_public_call_okl ?= $(plingeling_exec_okl)$(lingeling_recommended_version_number_okl)


lingeling_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/Lingeling
lingeling_doc_dir_okl ?= $(lingeling_base_doc_dir_okl)/$(lingeling_recommended_okl)
lingeling_docu_page_okl ?= $(doc_dir)/

lingeling_homepage_url_okl := 
lingeling_documentation_url_okl := 

