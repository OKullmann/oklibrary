# Oliver Kullmann, 18.12.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Maxima

maxima_recommended_version_number_okl ?= 5.15.0
# ATTENTION: special repair-build for 5.15.0 regarding module graphs
# 5.16.3 usable, but gf-package unacceptably slow
maxima_supported_not_recommended_version_numbers_okl ?= 5.16.3
maxima_supported_version_numbers_okl ?= $(maxima_supported_not_recommended_version_numbers_okl) $(maxima_recommended_version_number_okl)

maxima_prefix_okl ?= maxima

maxima_recommended_package_name_okl ?= $(maxima_prefix_okl)-$(maxima_recommended_version_number_okl)

maxima_html_template_okl ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Maxima.html
maxima_html_output_okl ?= $(local_html_dir)/Maxima.html
maxima_html_documentation_index_location_okl ?= Maxima.html

# Temporary:
ecl_call_okl ?= $(ExternalSources_installations)/Ecl/bin/ecl
# Possibilities for the underlying Lisp: clisp, ecl (only from 5.16.3 on)
maxima_lisp_name_okl ?= clisp
ifeq ($(maxima_lisp_name_okl),clisp)
  maxima_lisp_configuration_okl ?= --with-clisp=$(clisp_call_okl) --with-clisp-runtime=$(clisp_lib_okl)
else
  maxima_lisp_configuration_okl ?= --with-ecl=$(ecl_call_okl)
endif


maxima_base_installation_dir_okl ?= $(ExternalSources_installations)/Maxima/$(maxima_lisp_name_okl)
maxima_installation_dir_okl ?= $(maxima_base_installation_dir_okl)/$(maxima_recommended_version_number_okl)
maxima_base_build_dir_okl ?= $(ExternalSources_builds)/Maxima/$(maxima_lisp_name_okl)
maxima_build_dir_okl ?= $(maxima_base_build_dir_okl)/$(maxima_recommended_package_name_okl)
maxima_base_doc_dir_okl ?= $(ExternalSources_doc)/Maxima
maxima_doc_dir_okl ?= $(maxima_base_doc_dir_okl)/$(maxima_recommended_version_number_okl)

maxima_call_okl ?= $(maxima_installation_dir_okl)/bin/maxima

maxima_share_dir_okl ?= $(maxima_installation_dir_okl)/share/maxima/$(maxima_recommended_version_number_okl)/share

maxima_book_name_okl ?= maximabook-19-Sept-2004.pdf
maxima_book_source_okl ?= $(ExternalSources_sources)/Maxima/$(maxima_book_name_okl)
maxima_book_doc_okl ?= $(maxima_base_doc_dir_okl)/$(maxima_book_name_okl)
maxima_woollettbook_name_okl ?= BookWoollett
maxima_source_woollettbook_okl ?= $(ExternalSources_sources)/Maxima/$(maxima_woollettbook_name_okl).tar.gz
maxima_woollettbook_okl ?= $(maxima_base_doc_dir_okl)/$(maxima_woollettbook_name_okl)

maxima_html_doc_okl ?= $(maxima_doc_dir_okl)/html
maxima_main_index_okl ?= $(maxima_html_doc_okl)/maxima.html
maxima_tutorial_doc_okl ?= $(maxima_html_doc_okl)/intromax.html
maxima_gf_manual_okl ?= $(maxima_doc_dir_okl)/gf_manual.pdf

maxima_docu_page_okl ?= $(doc_dir)/doxygen_html/df/d26/Buildsystem_2ExternalSources_2SpecialBuilds_2docus_2Maxima_8hpp.html

maxima_homepage_url_okl := http://maxima.sourceforge.net/
maxima_documentation_url_okl := http://maxima.sourceforge.net/docs.shtml

maxima_targets_prefix_okl := $(maxima_prefix_okl)-
maxima_recommended_okl := $(maxima_targets_prefix_okl)$(maxima_recommended_version_number_okl)
maxima_source_okl := $(ExternalSources)/sources/Maxima/$(maxima_recommended_okl)

# Tool : gnuplot

gnuplot_recommended_version_number_okl ?= 4.2.3
gnuplot_supported_not_recommended_version_numbers_okl ?= 
gnuplot_supported_version_numbers_okl ?= $(gnuplot_supported_not_recommended_version_numbers_okl) $(gnuplot_recommended_version_number_okl)

gnuplot_prefix_okl ?= gnuplot

gnuplot_recommended_package_name_okl ?= $(gnuplot_prefix_okl)-$(gnuplot_recommended_version_number_okl)

gnuplot_base_installation_dir_okl ?= $(ExternalSources_installations)/Gnuplot
gnuplot_installation_dir_okl ?= $(gnuplot_base_installation_dir_okl)/$(gnuplot_recommended_version_number_okl)
gnuplot_base_build_dir_okl ?= $(ExternalSources_builds)/Gnuplot
gnuplot_build_dir_okl ?= $(gnuplot_base_build_dir_okl)/$(gnuplot_recommended_package_name_okl)

gnuplot_homepage_url_okl := http://gnuplot.sourceforge.net

gnuplot_targets_prefix_okl := $(gnuplot_prefix_okl)-
gnuplot_recommended_okl := $(gnuplot_targets_prefix_okl)$(gnuplot_recommended_version_number_okl)
gnuplot_source_okl := $(ExternalSources)/sources/Maxima/$(gnuplot_recommended_okl)

gnuplot_call_okl ?= $(gnuplot_installation_dir_okl)/bin/gnuplot

gnuplot_pdf_name_okl ?= $(gnuplot_recommended_okl).pdf
gnuplot_pdf_source_okl ?= $(ExternalSources_sources)/Maxima/$(gnuplot_pdf_name_okl)
gnuplot_pdf_doc_okl ?= $(maxima_base_doc_dir_okl)/$(gnuplot_pdf_name_okl)
