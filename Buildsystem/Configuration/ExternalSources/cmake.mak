# Oliver Kullmann, 15.9.2009 (Swansea)
# Copyright 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Cmake

all_extsrc_okl += cmake

cmake_recommended_version_number_okl ?= 2.6.4
cmake_other_versions_okl ?=

cmake_prefix_okl ?= cmake

cmake_targets_prefix_okl := $(cmake_prefix_okl)-
cmake_recommended_okl := $(cmake_targets_prefix_okl)$(cmake_recommended_version_number_okl)
cmake_source_okl := $(ExternalSources)/sources/Cmake/$(cmake_recommended_okl)
cmake_source_ext_okl ?= .tar.gz

cmake_base_build_dir_okl ?= $(ExternalSources_builds)/Cmake
cmake_build_dir_okl ?= $(cmake_base_build_dir_okl)/$(cmake_recommended_okl)

cmake_default_install_okl ?= local
# other possibility: system

cmake_base_installation_dir_okl ?= $(ExternalSources_installations)/Cmake
cmake_installation_dir_okl ?= $(cmake_base_installation_dir_okl)/$(cmake_recommended_version_number_okl)
cmake_system_install_directory_okl ?= /usr/local
ifeq ($(cmake_default_install_okl),local)
  cmake_install_directory_okl ?= $(cmake_installation_dir_okl)
  cmake_install_command_okl ?= make install
  cmake_call_okl ?= $(cmake_installation_dir_okl)/bin/cmake
else
  cmake_install_directory_okl ?= $(cmake_system_install_directory)
  cmake_install_command_okl ?= sudo make install
  cmake_call_okl ?= cmake
endif

cmake_base_doc_dir_okl ?= $(ExternalSources_doc)/Cmake
cmake_doc_dir_okl ?= $(cmake_base_doc_dir_okl)/$(cmake_recommended_version_number_okl)
cmake_html_documentation_index_location_okl ?= $(cmake_doc_dir_okl)/cmake.html
cmake_docu_page_okl ?= $(doc_dir)/doxygen_html/da/d23/docus_2CMake_8hpp.html

cmake_homepage_url_okl := http://www.cmake.org/
cmake_documentation_url_okl := http://www.cmake.org/cmake/help/documentation.html

