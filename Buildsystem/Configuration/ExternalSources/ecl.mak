# Oliver Kullmann, 30.8.2008 (Swansea)
# Copyright 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Ecl

ecl_recommended_version_number_okl ?= 10.4.1
ecl_supported_not_recommended_version_numbers_okl ?= 9.12.3

ecl_prefix_okl ?= ecl

ecl_recommended_package_name_okl ?= $(ecl_prefix_okl)-$(ecl_recommended_version_number_okl)

ecl_html_template_okl ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Ecl.html
ecl_html_output_okl ?= $(local_html_dir)/Ecl.html
ecl_html_documentation_index_location_okl ?= Ecl.html

ecl_base_installation_dir_okl ?= $(ExternalSources_installations)/Ecl
ecl_installation_dir_okl ?= $(ecl_base_installation_dir_okl)/$(ecl_recommended_version_number_okl)
ecl_base_build_dir_okl ?= $(ExternalSources_builds)/Ecl
ecl_build_dir_okl ?= $(ecl_base_build_dir_okl)/$(ecl_recommended_package_name_okl)
ecl_base_doc_dir_okl ?= $(ExternalSources_doc)/Ecl
ecl_doc_dir_okl ?= $(ecl_base_doc_dir_okl)/$(ecl_recommended_version_number_okl)

ecl_man_html_okl ?= $(ecl_doc_dir_okl)/ecl.html
ecl_man_pdf_okl ?= $(ecl_doc_dir_okl)/ecl.pdf
ecl_tutorial_okl ?= $(ecl_doc_dir_okl)/LISP-tutorial.txt
ecl_class_guide_okl ?= $(ecl_doc_dir_okl)/CLOS-guide.txt
ecl_impnotes_html_okl ?= $(ecl_doc_dir_okl)/impnotes.html

ecl_call_okl ?= $(ecl_installation_dir_okl)/bin/ecl

ecl_docu_page_okl ?= $(doc_dir)/doxygen_html/d1/d31/docus_2Ecl_8hpp.html

ecl_homepage_url_okl := http://ecls.sourceforge.net/
ecl_documentation_url_okl := http://ecls.sourceforge.net/resources.html

ecl_targets_prefix_okl := $(ecl_prefix_okl)-
ecl_recommended_okl := $(ecl_targets_prefix_okl)$(ecl_recommended_version_number_okl)
ecl_source_okl := $(ExternalSources)/sources/Ecl/$(ecl_recommended_okl)

# Tool: rlwrap

rlwrap_recommended_version_number_okl ?= 0.30
rlwrap_supported_not_recommended_version_numbers_okl ?= 

rlwrap_prefix_okl ?= rlwrap

rlwrap_recommended_package_name_okl ?= $(rlwrap_prefix_okl)-$(rlwrap_recommended_version_number_okl)

rlwrap_base_installation_dir_okl ?= $(ExternalSources_installations)/Rlwrap
rlwrap_installation_dir_okl ?= $(rlwrap_base_installation_dir_okl)/$(rlwrap_recommended_version_number_okl)
rlwrap_base_build_dir_okl ?= $(ecl_base_build_dir_okl)/Rlwrap
rlwrap_build_dir_okl ?= $(rlwrap_base_build_dir_okl)/$(rlwrap_recommended_package_name_okl)

rlwrap_call_okl ?= $(rlwrap_installation_dir_okl)/bin/rlwrap

rlwrap_homepage_url_okl := http://utopia.knoware.nl/~hlub/uck/rlwrap/

rlwrap_targets_prefix_okl := $(rlwrap_prefix_okl)-
rlwrap_recommended_okl := $(rlwrap_targets_prefix_okl)$(rlwrap_recommended_version_number_okl)
rlwrap_source_okl := $(ExternalSources)/sources/Ecl/$(rlwrap_recommended_okl)

