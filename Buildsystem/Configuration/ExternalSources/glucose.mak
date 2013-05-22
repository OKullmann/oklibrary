# Matthew Gwynne, 13.1.2011 (Swansea)
# Copyright 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

glucose_recommended_version_number_okl ?= 2.2
glucose_other_versions_okl ?=

glucose_base_name_okl := glucose
glucose_recommended_okl := $(glucose_base_name_okl)-$(glucose_recommended_version_number_okl)
glucose_base_source_okl := $(ExternalSources)/sources/SAT/Glucose
glucose_source_okl := $(glucose_base_source_okl)/$(glucose_recommended_okl)

glucose_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Glucose
glucose_build_dir_okl ?= $(glucose_base_build_dir_okl)/$(glucose_recommended_okl)

glucose_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Glucose
glucose_installation_dir_okl ?= $(glucose_base_installation_dir_okl)/$(glucose_recommended_version_number_okl)

glucose_exec_okl ?= glucose
glucose_call_okl ?= $(glucose_installation_dir_okl)/$(glucose_exec_okl)
glucose_public_call_okl ?= $(glucose_exec_okl)$(glucose_recommended_version_number_okl)

glucose_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/Glucose
glucose_doc_dir_okl ?= $(glucose_base_doc_dir_okl)/$(glucose_recommended_okl)
glucose_docu_page_okl ?= $(doc_dir)/doxygen_html/da/dd1/Glucose_8hpp.html

glucose_homepage_url_okl := http://www.lri.fr/~simon/glucose/
glucose_documentation_url_okl := http://www.lri.fr/~simon/glucose/downloads/glucose-compet-report.pdf
