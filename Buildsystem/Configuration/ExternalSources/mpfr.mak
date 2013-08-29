# Oliver Kullmann, 24.2.2010 (Swansea)
# Copyright 2010, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the Mpfr library

mpfr_recommended_version_number_okl ?= 3.1.2
mpfr_other_versions_okl ?= 3.0.0

mpfr_prefix_okl ?= mpfr

mpfr_recommended_okl := $(mpfr_prefix_okl)-$(mpfr_recommended_version_number_okl)
mpfr_source_okl := $(ExternalSources)/sources/Gmp/$(mpfr_recommended_okl)

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


mpfr_base_doc_dir_okl ?= $(ExternalSources_doc)/Mpfr
mpfr_doc_dir_okl ?= $(mpfr_base_doc_dir_okl)/$(mpfr_recommended_version_number_okl)
mpfr_html_documentation_index_location_okl ?= $(mpfr_doc_dir_okl)/mpfr.html/index.html
mpfr_dvi_documentation_index_location_okl ?= $(mpfr_doc_dir_okl)/mpfr.pdf
mpfr_docu_page_okl ?= $(doc_dir)/doxygen_html/???

mpfr_homepage_url_okl := http://www.mpfr.org/
mpfr_documentation_url_okl := http://www.mpfr.org/mpfr-current/mpfr.html

# Mpc

mpc_recommended_version_number_okl ?= 1.0.1
mpc_other_versions_okl ?= 0.8.2

mpc_prefix_okl ?= mpc

mpc_targets_prefix_okl := $(mpc_prefix_okl)-
mpc_recommended_okl := $(mpc_targets_prefix_okl)$(mpc_recommended_version_number_okl)
mpc_source_okl := $(ExternalSources)/sources/Gmp/$(mpc_recommended_okl)

mpc_homepage_url_okl := http://www.multiprecision.org/
mpc_documentation_url_okl := http://www.multiprecision.org/index.php?prog=mpc&page=documentation
