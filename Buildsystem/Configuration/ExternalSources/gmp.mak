# Oliver Kullmann, 18.8.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the Gmp library

gmp_recommended_version_number_okl ?= 5.0.1
gmp_supported_not_recommended_version_numbers_okl ?= 4.3.1

gmp_prefix_okl ?= gmp

gmp_targets_prefix_okl := $(gmp_prefix_okl)-
gmp_recommended_okl := $(gmp_targets_prefix_okl)$(gmp_recommended_version_number_okl)
gmp_source_dir_okl := $(ExternalSources)/sources/Gmp/$(gmp_recommended_okl)

gmp_base_build_dir_okl ?= $(ExternalSources_builds)/Gmp
gmp_gccbuild_dir_okl ?= $(gmp_base_build_dir_okl)/$(gcc_recommended_version_number_okl)
gmp_build_dir_okl ?= $(gmp_gccbuild_dir_okl)/$(gmp_recommended_okl)

gmp_default_install_okl ?= local
# other possibility: system

gmp_base_installation_dir_okl ?= $(ExternalSources_installations)/Gmp
gmp_installation_dir_okl ?= $(gmp_base_installation_dir_okl)/$(gcc_recommended_version_number_okl)/$(gmp_recommended_version_number_okl)
gmp_system_install_directory_okl ?= /usr/local
ifeq ($(gmp_default_install_okl),local)
  gmp_install_directory_okl ?= $(gmp_installation_dir_okl)
  gmp_link_option_okl ?= -L $(gmp_install_directory_okl)/lib -Wl,-rpath,$(gmp_install_directory_okl)/lib -lgmp
  gmp_install_command_okl ?= make install
else
  gmp_install_directory_okl ?= $(gmp_system_install_directory)
  gmp_link_option_okl ?= -L $(gmp_install_directory_okl)/lib -lgmp
  gmp_install_command_okl ?= sudo make install
endif

gmp_source_library_okl ?= $(gmp_install_directory_okl)/include
gmp_include_option_okl ?= -I $(gmp_source_library_okl)

gmp_base_doc_dir_okl ?= $(ExternalSources_doc)/Gmp
gmp_doc_dir_okl ?= $(gmp_base_doc_dir_okl)/$(gmp_recommended_version_number_okl)
gmp_html_documentation_index_location_okl ?= $(gmp_doc_dir_okl)/gmp.html/index.html
gmp_dvi_documentation_index_location_okl ?= $(gmp_doc_dir_okl)/gmp.dvi
gmp_docu_page_okl ?= $(doc_dir)/doxygen_html/d5/d9b/docus_2Gmp_8hpp.html

gmp_homepage_url_okl := http://gmplib.org/
gmp_documentation_url_okl := http://gmplib.org/\#DOC

