# Matthew Gwynne, 31.7.2009 (Swansea)
# Copyright 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Picosat

picosat_recommended_version_number_okl ?= 913
picosat_other_versions_okl ?= 535 632 846

picosat_recommended_okl := picosat-$(picosat_recommended_version_number_okl)
picosat_source_okl := $(ExternalSources)/sources/SAT/Picosat/$(picosat_recommended_okl)

picosat_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Picosat
picosat_build_dir_okl ?= $(picosat_base_build_dir_okl)/$(picosat_recommended_okl)

picosat_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Picosat
picosat_installation_dir_okl ?= $(picosat_base_installation_dir_okl)/$(picosat_recommended_okl)

picosat_exec_okl ?= picosat
picosat_call_okl ?= $(picosat_installation_dir_okl)/$(picosat_exec_okl)
picosat_public_call_okl ?= $(picosat_exec_okl)$(picosat_recommended_version_number_okl)

picosat_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/Picosat
picosat_doc_dir_okl ?= $(picosat_base_doc_dir_okl)/$(picosat_recommended_okl)
picosat_docu_page_okl ?= $(doc_dir)/doxygen_html/d8/d96/Picosat_8hpp.html

picosat_homepage_url_okl := http://fmv.jku.at/picosat/
picosat_documentation_url_okl := http://fmv.jku.at/precosat/preicosat-sc09.pdf

