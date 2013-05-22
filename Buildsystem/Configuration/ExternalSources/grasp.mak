# Oliver Kullmann, 18.7.2007 (Swansea)
# Copyright 2008, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using Grasp
# (cannot be downloaded)

grasp_recommended_version_number_okl ?= 20080718
grasp_other_versions_okl ?=

grasp_recommended_okl := grasp-$(grasp_recommended_version_number_okl)
grasp_source_okl := $(ExternalSources)/sources/SAT/GRASP/$(grasp_recommended_okl)

grasp_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Grasp
grasp_build_dir_okl ?= $(grasp_base_build_dir_okl)/$(grasp_recommended_okl)

grasp_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Grasp
grasp_installation_dir_okl ?= $(grasp_base_installation_dir_okl)/$(grasp_recommended_version_number_okl)

grasp_exec_okl ?= sat-grasp
grasp_call_okl ?= $(grasp_installation_dir_okl)/$(grasp_exec_okl)

grasp_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/Grasp
grasp_doc_dir_okl ?= $(grasp_base_doc_dir_okl)/$(grasp_recommended_okl)
grasp_man_okl ?= $(grasp_doc_dir_okl)/$(grasp_exec_okl).html
grasp_docu_page_okl ?= $(doc_dir)/doxygen_html/d5/d83/Grasp_8hpp.html

grasp_homepage_url_okl := http://users.ecs.soton.ac.uk/jpms/soft/
grasp_documentation_url_okl := ???

