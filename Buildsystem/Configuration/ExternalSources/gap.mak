# Oliver Kullmann, 12.7.2008 (Swansea)
# Copyright 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using Gap

gap_recommended_version_number_okl ?= 4r4p12
gap_other_versions_okl ?= 4r4p10
gap_recommended_major_version_number_okl ?= $(shell echo $(gap_recommended_version_number_okl) | sed 's/p.*//')
# e.g., 4r4p10 -> 4r4
gap_extracted_version_number_okl := $(shell echo $(gap_recommended_version_number_okl) | tr "rp" ".")
# e.g., 4r4p10 -> 4.4.10

gap_packages_recommended_version_number_okl ?= 2008_12_18-12_55_UTC
gap_packages_other_versions_okl ?= 2008_06_23-19_20_UTC

gap_prefix_okl ?= gap
gap_packages_prefix_okl ?= packages

gap_recommended_package_name_okl ?= $(gap_prefix_okl)$(gap_recommended_version_number_okl)
gap_source_okl := $(ExternalSources)/sources/Gap/$(gap_recommended_package_name_okl)
gap_packages_recommended_package_name_okl ?= $(gap_packages_prefix_okl)-$(gap_packages_recommended_version_number_okl)
gap_packages_source_okl := $(ExternalSources)/sources/Gap/$(gap_packages_recommended_package_name_okl)
gap_instpackages_name_okl ?= InstPackages.sh
gap_instpackages_source_okl := $(ExternalSources)/sources/Gap/$(gap_instpackages_name_okl)

gap_html_template_okl ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Gap.html
gap_html_output_okl ?= $(local_html_dir)/Gap.html
gap_html_documentation_index_location_okl ?= Gap.html

gap_base_installation_dir_okl ?= $(ExternalSources_installations)/Gap
gap_installation_dir_okl ?= $(gap_base_installation_dir_okl)/$(gap_prefix_okl)$(gap_recommended_major_version_number_okl)
gap_base_doc_dir_okl ?= $(ExternalSources_doc)/Gap
gap_doc_dir_okl ?= $(gap_base_doc_dir_okl)/$(gap_recommended_version_number_okl)

gap_call_okl ?= $(gap_installation_dir_okl)/bin/gap.sh

gap_html_doc_okl ?= $(gap_doc_dir_okl)/htm
gap_main_index_okl ?= $(gap_html_doc_okl)/index.htm
gap_tutorial_pdf_okl ?= $(gap_doc_dir_okl)/tut/manual.pdf
gap_manual_pdf_okl ?= $(gap_doc_dir_okl)/ref/manual.pdf
gap_programming_pdf_okl ?= $(gap_doc_dir_okl)/prg/manual.pdf

gap_monoid_installdocdir_okl ?= $(gap_installation_dir_okl)/pkg/monoid/doc
gap_monoid_docdir_okl ?= $(gap_doc_dir_okl)/monoid
gap_monoid_pdf_okl ?= $(gap_monoid_docdir_okl)/manual.pdf
gap_monoid_html_okl ?= $(gap_monoid_docdir_okl)/chap0.html
gap_grape_installdocdir_okl ?= $(gap_installation_dir_okl)/pkg/grape/doc
gap_grape_docdir_okl ?= $(gap_doc_dir_okl)/grape
gap_grape_pdf_okl ?= $(gap_grape_docdir_okl)/manual.pdf

gap_docu_page_okl ?= $(doc_dir)/doxygen_html/d8/d95/Gap_8hpp.html

gap_homepage_url_okl := http://www-gap.mcs.st-and.ac.uk/
gap_documentation_url_okl := http://www-gap.mcs.st-and.ac.uk/Doc/doc.html

