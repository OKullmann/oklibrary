# Oliver Kullmann, 26.7.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using R

R_recommended_version_number_okl ?= 2.14.1
R_other_versions_okl ?= 2.14.0

R_prefix_okl ?= R

R_recommended_okl := $(R_prefix_okl)-$(R_recommended_version_number_okl)
R_source_dir_okl := $(ExternalSources)/sources/R
R_source_package_okl := $(R_source_dir_okl)/$(R_recommended_okl)

R_base_build_dir_okl ?= $(ExternalSources_builds)/R
R_build_dir_okl ?= $(R_base_build_dir_okl)/$(R_recommended_okl)

R_default_install_okl ?= local
# other possibility: system

R_base_installation_dir_okl ?= $(ExternalSources_installations)/R
R_installation_dir_okl ?= $(R_base_installation_dir_okl)/$(R_recommended_version_number_okl)
R_system_install_directory_okl ?= /usr/local
ifeq ($(R_default_install_okl),local)
  R_install_directory_okl ?= $(R_installation_dir_okl)
  R_install_command_okl ?= make install
  R_call_okl ?= $(R_installation_dir_okl)/bin/R
  R_homedir_okl ?= $(R_base_installation_dir_okl)
else
  R_install_directory_okl ?= $(R_system_install_directory)
  R_install_command_okl ?= sudo make install
  R_call_okl ?= R
  R_homedir_okl ?= $(HOME)
endif
R_site_profile_okl ?= $(R_homedir_okl)/Rprofile_okl
R_call_parameters ?= --save --no-restore-data
Rrestore_call_parameters_okl ?= 

R_base_doc_dir_okl ?= $(ExternalSources_doc)/R
R_doc_dir_okl ?= $(R_base_doc_dir_okl)/$(R_recommended_version_number_okl)
R_html_documentation_index_location_okl ?= $(R_doc_dir_okl)/doc/html/index.html
R_dvi_documentation_index_location_okl ?= 
R_pdf_documentation_index_location_okl ?= 
R_docu_page_okl ?= $(doc_dir)/doxygen_html/da/d4d/docus_2R_8hpp.html

R_homepage_url_okl := http://www.r-project.org/
R_documentation_url_okl := http://www.r-project.org/

R_packages_okl := gtools_2.6.2.tar.gz lpSolve_5.6.6.tar.gz QCA_0.6-5.tar.gz QCA3_0.0-4.tar.gz # Hmisc_3.8-3.tar.gz pastecs_1.3-11.tar.gz gsl_1.9-3.tar.gz Rcmdr_1.5-4.tar.gz
# selected packages can be installed by e.g.
# ExternalSources> oklib R_packages R_packages_okl="pastecs_1.3-11.tar.gz"
# and loaded into R via
# > library("pastecs")
