# Oliver Kullmann, 20.5.2009 (Swansea)
# Copyright 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# The bzip2 library (needed by the iostream-library)

bzip2_recommended_version_number_okl ?= 1.0.5
bzip2_other_versions_okl ?=

bzip2_base_build_dir_okl ?= $(ExternalSources_builds)/Bzip2
bzip2_extracted_package_okl ?= $(bzip2_base_build_dir_okl)/bzip2-$(bzip2_recommended_version_number_okl)
bzip2_build_dir_okl ?= $(bzip2_base_build_dir_okl)/$(gcc_recommended_version_number_okl)/$(bzip2_recommended_version_number_okl)
bzip2_base_installation_dir_okl ?= $(ExternalSources_installations)/Bzip2
bzip2_installation_dir_okl ?= $(bzip2_base_installation_dir_okl)/$(gcc_recommended_version_number_okl)/$(bzip2_recommended_version_number_okl)
bzip2_base_doc_dir_okl ?= $(ExternalSources_doc)/Bzip2
bzip2_doc_dir_okl ?= $(bzip2_base_doc_dir_okl)/$(bzip2_recommended_version_number_okl)

bzip2_html_documentation_index_location_okl ?= $(bzip2_doc_dir_okl)/manual.html
bzip2_docu_page_okl ?= $(doc_dir)/doxygen_html/

bzip2_homepage_url_okl := http://www.bzip.org/
bzip2_documentation_url_okl := http://www.bzip.org/docs.html

bzip2_source_dir_okl := $(ExternalSources)/sources/Boost
bzip2_source_package_okl := $(bzip2_source_dir_okl)/bzip2-$(bzip2_recommended_version_number_okl)

bzip2_source_library_okl ?= $(bzip2_installation_dir_okl)/include
bzip2_include_option_okl ?= -I$(bzip2_source_library_okl)
bzip2_link_library_okl ?= $(bzip2_installation_dir_okl)/lib
bzip2_link_option_okl ?= -L$(bzip2_link_library_okl) -Wl,-rpath,$(bzip2_link_library_okl)
bzip2_call_okl ?= $(bzip2_installation_dir_okl)/bin/bzip2

# as long as the OKlibrary is built using gcc-4.1.2 (to be removed after that):
bzip2412_source_library_okl ?= $(bzip2_base_installation_dir_okl)/4.1.2/$(bzip2_recommended_version_number_okl)/include
bzip2412_link_library_okl ?= $(bzip2_base_installation_dir_okl)/4.1.2/$(bzip2_recommended_version_number_okl)/lib
