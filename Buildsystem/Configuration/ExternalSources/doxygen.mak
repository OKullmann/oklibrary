# Oliver Kullmann, 24.7.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using doxygen

doxygen_recommended_version_number_okl ?= 1.7.6.1
doxygen_other_versions_okl ?= 1.7.1

doxygen_prefix_okl ?= doxygen

doxygen_recommended_okl := $(doxygen_prefix_okl)-$(doxygen_recommended_version_number_okl)
doxygen_source_okl := $(ExternalSources)/sources/Doxygen/$(doxygen_recommended_okl).src

doxygen_base_build_dir_okl ?= $(ExternalSources_builds)/Doxygen
doxygen_build_dir_okl ?= $(doxygen_base_build_dir_okl)/$(doxygen_recommended_okl)

doxygen_default_install_okl ?= local
# other possibility: system

doxygen_base_installation_dir_okl ?= $(ExternalSources_installations)/Doxygen
doxygen_installation_dir_okl ?= $(doxygen_base_installation_dir_okl)/$(doxygen_recommended_version_number_okl)
doxygen_system_install_directory_okl ?= /usr/local
ifeq ($(doxygen_default_install_okl),local)
  doxygen_install_directory_okl ?= $(doxygen_installation_dir_okl)
  doxygen_install_command_okl ?= make install
  doxygen_call_okl ?= $(doxygen_installation_dir_okl)/bin/doxygen
  doxytag_call_okl ?= $(doxygen_installation_dir_okl)/bin/doxytag
else
  doxygen_install_directory_okl ?= $(doxygen_system_install_directory_okl)
  doxygen_install_command_okl ?= sudo make install
  doxygen_call_okl ?= doxygen
  doxytag_call_okl ?= doxytag
endif

doxygen_base_doc_dir_okl ?= $(ExternalSources_doc)/Doxygen
doxygen_doc_dir_okl ?= $(doxygen_base_doc_dir_okl)/$(doxygen_recommended_version_number_okl)
doxygen_html_documentation_index_location_okl ?= $(doxygen_doc_dir_okl)/html/index.html
doxygen_docu_page_okl ?= $(doc_dir)/doxygen_html/df/d07/docus_2Doxygen_8hpp.html

doxygen_homepage_url_okl := http://www.stack.nl/~dimitri/doxygen/
doxygen_documentation_url_okl := http://www.stack.nl/~dimitri/doxygen/manual.html







