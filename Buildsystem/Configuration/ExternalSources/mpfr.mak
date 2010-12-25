# Oliver Kullmann, 24.2.2010 (Swansea)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the Mpfr library

mpfr_recommended_version_number_okl ?= 2.4.2
mpfr_supported_not_recommended_version_numbers_okl ?= 2.3.0

mpfr_prefix_okl ?= mpfr

mpfr_targets_prefix_okl := $(mpfr_prefix_okl)-
mpfr_recommended_okl := $(mpfr_targets_prefix_okl)$(mpfr_recommended_version_number_okl)
mpfr_source_dir_okl := $(ExternalSources)/sources/Gmp/$(mpfr_recommended_okl)

mpfr_base_build_dir_okl ?= $(ExternalSources_builds)/Mpfr
mpfr_gccbuild_dir_okl ?= $(mpfr_base_build_dir_okl)/$(gcc_recommended_version_number_okl)
mpfr_build_dir_okl ?= $(mpfr_gccbuild_dir_okl)/$(mpfr_recommended_okl)

mpfr_default_install_okl ?= local
# other possibility: system

mpfr_base_installation_dir_okl ?= $(ExternalSources_installations)/Mpfr
mpfr_installation_dir_okl ?= $(mpfr_base_installation_dir_okl)/$(gcc_recommended_version_number_okl)/$(mpfr_recommended_version_number_okl)
mpfr_system_install_directory_okl ?= /usr/local
ifeq ($(mpfr_default_install_okl),local)
  mpfr_install_directory_okl ?= $(mpfr_installation_dir_okl)
  mpfr_link_option_okl ?= -L $(mpfr_install_directory_okl)/lib -Wl,-rpath,$(mpfr_install_directory_okl)/lib -lmpfr $(gmp_link_option_okl)
  mpfr_install_command_okl ?= make install
else
  mpfr_install_directory_okl ?= $(mpfr_system_install_directory)
  mpfr_link_option_okl ?= -L $(mpfr_install_directory_okl)/lib -lmpfr $(gmp_link_option_okl)
  mpfr_install_command_okl ?= sudo make install
endif

mpfr_source_library_okl ?= $(mpfr_install_directory_okl)/include
mpfr_include_option_okl ?= -I $(mpfr_source_library_okl)


# the local installation with system-compiler:
mpfr_locsys_base_build_dir_okl ?= $(mpfr_gccbuild_dir_okl)/system
mpfr_locsys_build_dir_okl ?= $(mpfr_locsys_base_build_dir_okl)/$(mpfr_recommended_okl)
mpfr_locsys_install_directory_okl ?= $(mpfr_installation_dir_okl)/system
mpfr_locsys_link_path_okl ?= -Wl,-rpath,$(mpfr_locsys_install_directory_okl)/lib
mpfr_locsys_link_option_okl ?= -L $(mpfr_locsys_install_directory_okl)/lib $(mpfr_locsys_link_path_okl) -lmpfr
mpfr_locsys_source_library_okl ?= $(mpfr_locsys_install_directory_okl)/include
mpfr_locsys_include_option_okl ?= -I $(mpfr_locsys_source_library_okl)


mpfr_base_doc_dir_okl ?= $(ExternalSources_doc)/Mpfr
mpfr_doc_dir_okl ?= $(mpfr_base_doc_dir_okl)/$(mpfr_recommended_version_number_okl)
mpfr_html_documentation_index_location_okl ?= $(mpfr_doc_dir_okl)/mpfr.html/index.html
mpfr_dvi_documentation_index_location_okl ?= $(mpfr_doc_dir_okl)/mpfr.pdf
mpfr_docu_page_okl ?= $(doc_dir)/doxygen_html/???

mpfr_homepage_url_okl := http://www.mpfr.org/
mpfr_documentation_url_okl := http://www.mpfr.org/mpfr-current/mpfr.html

