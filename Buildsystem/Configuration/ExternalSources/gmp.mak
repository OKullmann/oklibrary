# Oliver Kullmann, 18.8.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the Gmp library

gmp_recommended_version_number ?= 4.2.2
gmp_supported_not_recommended_version_numbers ?= 
gmp_supported_version_numbers ?= $(gmp_recommended_version_number) $(gmp_supported_not_recommended_version_numbers)

gmp_prefix ?= gmp

gmp_base_directory ?= $(ExternalSources)/Gmp

gmp_default_install ?= local
# other possibility: system

gmp_local_install_directory ?= $(gmp_base_directory)/$(gmp_recommended_version_number)
gmp_system_install_directory ?= /usr/local
ifeq ($(gmp_default_install),local)
  gmp_install_directory ?= $(gmp_local_install_directory)
  gmp_link_option ?= -L $(gmp_install_directory)/lib -Wl,-rpath,$(gmp_install_directory)/lib -lgmp
  gmp_install_command ?= make install
else
  gmp_install_directory ?= $(gmp_system_install_directory)
  gmp_link_option ?= -L $(gmp_install_directory)/lib -lgmp
  gmp_install_command ?= sudo make install
endif

gmp_source_library ?= $(gmp_install_directory)/include
gmp_include_option ?= -I $(gmp_source_library)

gmp_documentation_base_dir ?= $(ExternalSources_doc)/Gmp
gmp_documentation_dir ?= $(gmp_documentation_base_dir)/$(gmp_recommended_version_number)
gmp_html_documentation_index_location ?= $(gmp_documentation_dir)/gmp.html/index.html
gmp_dvi_documentation_index_location ?= $(gmp_documentation_dir)/gmp.dvi
gmp_docu_page ?= $(doc_dir)/doxygen_html/d5/d9b/docus_2Gmp_8hpp.html

gmp_homepage_url := http://gmplib.org/
gmp_documentation_url := http://gmplib.org/\#DOC

gmp_targets_prefix := $(gmp_prefix)-
gmp_targets := $(addprefix $(gmp_targets_prefix), $(gmp_supported_version_numbers))
gmp_recommended := $(gmp_targets_prefix)$(gmp_recommended_version_number)

