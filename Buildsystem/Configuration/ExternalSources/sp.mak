# Oliver Kullmann, 23.4.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the survey propagation solver

sp_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Sp

sp_recommended_version_number_okl ?= 1.4
sp_extracted_archive_okl ?= sp-$(sp_recommended_version_number_okl)

sp_installation_dir_okl ?= $(sp_base_installation_dir_okl)/$(sp_extracted_archive_okl)

sp_call_okl ?= $(sp_installation_dir_okl)/sp

sp_docu_page_okl ?= $(doc_dir)/doxygen_html/

sp_source_okl := $(ExternalSources)/sources/SAT/SurveyPropagation/$(sp_extracted_archive_okl)b

