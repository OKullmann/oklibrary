# Oliver Kullmann, 30.8.2008 (Swansea)
# Copyright 2008, 2009, 2010, 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using Ecl

ecl_recommended_version_number_okl ?= 11.1.1.2
ecl_other_versions_okl ?= 12.12.1

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

ecl_recommended_okl := $(ecl_prefix_okl)-$(ecl_recommended_version_number_okl)
ecl_source_okl := $(ExternalSources)/sources/Ecl/$(ecl_recommended_okl)

# Tool: rlwrap

rlwrap_recommended_version_number_okl ?= 0.37
rlwrap_other_versions_okl ?= 0.30

rlwrap_prefix_okl ?= rlwrap

rlwrap_recommended_package_name_okl ?= $(rlwrap_prefix_okl)-$(rlwrap_recommended_version_number_okl)

rlwrap_base_installation_dir_okl ?= $(ExternalSources_installations)/Rlwrap
rlwrap_installation_dir_okl ?= $(rlwrap_base_installation_dir_okl)/$(rlwrap_recommended_version_number_okl)
rlwrap_base_build_dir_okl ?= $(ecl_base_build_dir_okl)/Rlwrap
rlwrap_build_dir_okl ?= $(rlwrap_base_build_dir_okl)/$(rlwrap_recommended_package_name_okl)

rlwrap_call_okl ?= $(rlwrap_installation_dir_okl)/bin/rlwrap

rlwrap_homepage_url_okl := http://utopia.knoware.nl/~hlub/uck/rlwrap/

rlwrap_recommended_okl := $(rlwrap_prefix_okl)-$(rlwrap_recommended_version_number_okl)
rlwrap_source_okl := $(ExternalSources)/sources/Ecl/$(rlwrap_recommended_okl)

# Tool: libffi

libffi_recommended_version_number_okl ?= 3.0.13
libffi_other_versions_okl ?= 3.0.9

libffi_prefix_okl ?= libffi

libffi_recommended_okl := $(libffi_prefix_okl)-$(libffi_recommended_version_number_okl)

libffi_base_installation_dir_okl ?= $(ExternalSources_installations)/Libffi
libffi_installation_dir_okl ?= $(libffi_base_installation_dir_okl)/$(libffi_recommended_version_number_okl)
libffi_base_build_dir_okl ?= $(ecl_base_build_dir_okl)/Libffi
libffi_build_dir_okl ?= $(libffi_base_build_dir_okl)/$(libffi_recommended_okl)

libffi_link_path_okl ?= -Wl,-rpath,$(libffi_installation_dir_okl)/lib
libffi_link_option_okl ?= -L$(libffi_installation_dir_okl)/lib $(libffi_link_path_okl) -lffi

libffi_source_library_okl ?= $(libffi_installation_dir_okl)/include
libffi_include_option_okl ?= -I $(libffi_source_library_okl)


libffi_homepage_url_okl := http://sourceware.org/libffi/

libffi_source_okl := $(ExternalSources)/sources/Ecl/$(libffi_recommended_okl)

