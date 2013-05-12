# Oliver Kullmann, 9.9.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Here all relative paths, to be used in html pages (except in case of
# links between doxygen pages, which are handled by doxygen itself),
# are defined.

# It is assumed here that all relevant make-variables got their values now.

# From the local home page:
rel_lindex_OKlibrary_html_documentation_index_location := $(shell $(rel_path_tool) $(OKlibrary_html_documentation_index_location) $(local_home_page_output))
rel_lindex_OKlibrary_full_documentation_dvi_location := $(shell $(rel_path_tool) $(OKlibrary_full_documentation_dvi_location) $(local_home_page_output))
rel_lindex_main_docu_page := $(shell $(rel_path_tool) $(main_docu_page) $(local_home_page_output))
rel_lindex_main_demo_page := $(shell $(rel_path_tool) $(main_demo_page) $(local_home_page_output))

# From the Aims-page:
rel_aims_accu2006_talk_location := $(shell $(rel_path_tool) $(accu2006_talk_location) $(aims_page_output))
rel_aims_oklibrary_report2009_location := $(shell $(rel_path_tool) $(oklibrary_report2009_location) $(aims_page_output))


# From the History-page:
rel_history_epsrc_faststream_pdf_location := $(shell $(rel_path_tool) $(epsrc_faststream_pdf_location) $(history_page_output))
rel_history_epsrc_6finalreport_pdf_location := $(shell $(rel_path_tool) $(epsrc_6finalreport_pdf_location) $(history_page_output))
rel_history_epsrc_finalreport_pdf_location := $(shell $(rel_path_tool) $(epsrc_finalreport_pdf_location) $(history_page_output))
rel_history_release_history_output := $(shell $(rel_path_tool) $(release_history_output) $(history_page_output))

# From the Ten-Challenges-page:
rel_tenchallenges_baltimore2008_talk_location := $(shell $(rel_path_tool) $(baltimore2008_talk_location) $(tenchallenges_page_output))
rel_tenchallenges_tenchallenges_doxygen_directory := $(shell $(rel_path_tool) $(tenchallenges_doxygen_directory) $(tenchallenges_page_output))

# From the Ramsey-Theory-page:
rel_greenTao_pcv2010_talk_location := $(shell $(rel_path_tool) $(greentao_pcv2010_talk_location) $(ramseytheory_page_output))
rel_greenTao_sat2010_talk_location := $(shell $(rel_path_tool) $(greentao_sat2010_talk_location) $(ramseytheory_page_output))
rel_sat2011competition_okl := $(shell $(rel_path_tool) $(sat2011competition_okl) $(externalsources_page_output))


# From the external sources page: ("les" for local external sources)
rel_les_boost_docu_page_okl := $(shell $(rel_path_tool) $(boost_docu_page_okl) $(externalsources_page_output))
rel_les_mhash_docu_page_okl := $(shell $(rel_path_tool) $(mhash_docu_page_okl) $(externalsources_page_output))
rel_les_gcc_docu_page_okl := $(shell $(rel_path_tool) $(gcc_docu_page_okl) $(externalsources_page_output))
rel_les_ocaml_docu_page := $(shell $(rel_path_tool) $(ocaml_docu_page) $(externalsources_page_output))
rel_les_valgrind_docu_page_okl := $(shell $(rel_path_tool) $(valgrind_docu_page_okl) $(externalsources_page_output))
rel_les_git_docu_page_okl := $(shell $(rel_path_tool) $(git_docu_page_okl) $(externalsources_page_output))
rel_les_doxygen_docu_page_okl := $(shell $(rel_path_tool) $(doxygen_docu_page_okl) $(externalsources_page_output))
rel_les_postgresql_docu_page := $(shell $(rel_path_tool) $(postgresql_docu_page) $(externalsources_page_output))
rel_les_R_docu_page_okl := $(shell $(rel_path_tool) $(R_docu_page_okl) $(externalsources_page_output))
rel_les_gmp_docu_page_okl := $(shell $(rel_path_tool) $(gmp_docu_page_okl) $(externalsources_page_output))
rel_les_sage_docu_page_okl := $(shell $(rel_path_tool) $(sage_docu_page_okl) $(externalsources_page_output))
rel_les_coq_docu_page := $(shell $(rel_path_tool) $(coq_docu_page) $(externalsources_page_output))
rel_les_ubcsat_docu_page_okl := $(shell $(rel_path_tool) $(ubcsat_docu_page_okl) $(externalsources_page_output))
rel_les_clisp_docu_page_okl := $(shell $(rel_path_tool) $(clisp_docu_page_okl) $(externalsources_page_output))
rel_les_ecl_docu_page_okl := $(shell $(rel_path_tool) $(ecl_docu_page_okl) $(externalsources_page_output))
rel_les_maxima_docu_page_okl := $(shell $(rel_path_tool) $(maxima_docu_page_okl) $(externalsources_page_output))
rel_les_gap_docu_page_okl := $(shell $(rel_path_tool) $(gap_docu_page_okl) $(externalsources_page_output))
rel_les_grasp_docu_page_okl := $(shell $(rel_path_tool) $(grasp_docu_page_okl) $(externalsources_page_output))
rel_les_picosat_docu_page_okl := $(shell $(rel_path_tool) $(picosat_docu_page_okl) $(externalsources_page_output))
rel_les_espresso_docu_page_okl := $(shell $(rel_path_tool) $(espresso_docu_page_okl) $(externalsources_page_output))

ifeq ($(extsrc_relative),)
  rel_les_boost_html_documentation_index_location_okl := $(boost_html_documentation_index_location_okl)
  rel_les_mhash_html_documentation_index_location_okl := $(mhash_html_documentation_index_location_okl)
  rel_les_valgrind_html_documentation_index_location_okl := $(valgrind_html_documentation_index_location_okl)
  rel_les_git_html_documentation_index_location_okl := $(git_html_documentation_index_location_okl)
  rel_les_doxygen_html_documentation_index_location_okl := $(doxygen_html_documentation_index_location_okl)
  rel_les_pgsql_html_documentation_index_location := $(pgsql_html_documentation_index_location)
  rel_les_R_html_documentation_index_location_okl := $(R_html_documentation_index_location_okl)
  rel_les_sage_main_index_okl := $(sage_main_index_okl)
  rel_les_gmp_html_documentation_index_location_okl := $(gmp_html_documentation_index_location_okl)
  rel_les_gmp_dvi_documentation_index_location_okl := $(gmp_dvi_documentation_index_location_okl)
  rel_les_grasp_html_documentation_index_location_okl := $(grasp_man_okl)

else

  rel_les_boost_html_documentation_index_location_okl := $(shell $(rel_path_tool) $(boost_html_documentation_index_location_okl) $(externalsources_page_output))
  rel_les_mhash_html_documentation_index_location_okl := $(shell $(rel_path_tool) $(mhash_html_documentation_index_location_okl) $(externalsources_page_output))
  rel_les_valgrind_html_documentation_index_location_okl := $(shell $(rel_path_tool) $(valgrind_html_documentation_index_location_okl) $(externalsources_page_output))
  rel_les_git_html_documentation_index_location_okl := $(shell $(rel_path_tool) $(git_html_documentation_index_location_okl) $(externalsources_page_output))
  rel_les_doxygen_html_documentation_index_location_okl := $(shell $(rel_path_tool) $(doxygen_html_documentation_index_location_okl) $(externalsources_page_output))
  rel_les_pgsql_html_documentation_index_location := $(shell $(rel_path_tool) $(pgsql_html_documentation_index_location) $(externalsources_page_output))
  rel_les_R_html_documentation_index_location_okl := $(shell $(rel_path_tool) $(R_html_documentation_index_location_okl) $(externalsources_page_output))
  rel_les_sage_main_index_okl := $(shell $(rel_path_tool) $(sage_main_index_okl) $(externalsources_page_output))
  rel_les_gmp_html_documentation_index_location_okl := $(shell $(rel_path_tool) $(gmp_html_documentation_index_location_okl) $(externalsources_page_output))
  rel_les_gmp_dvi_documentation_index_location_okl := $(shell $(rel_path_tool) $(gmp_dvi_documentation_index_location_okl) $(externalsources_page_output))
  rel_les_grasp_html_documentation_index_location_okl := $(shell $(rel_path_tool) $(grasp_man_okl) $(externalsources_page_output))
endif

# to be completed XXX

# From the Internet home page:
rel_ihp_local_home_page_output := $(shell $(rel_path_tool) $(local_home_page_output) $(internet_home_page_output))

# From the licence page:
rel_lic_licence_docu_page := $(shell $(rel_path_tool) $(licence_docu_page) $(licence_page_output))

# From the Maxima page:
rel_maxima_usermaxima_docu_page_okl := $(shell $(rel_path_tool) $(usermaxima_docu_page_okl) $(maxima_html_output_okl))
ifeq ($(extsrc_relative),)
  rel_maxima_maxima_main_index_okl := $(maxima_main_index_okl)
  rel_maxima_maxima_tutorial_doc_okl := $(maxima_tutorial_doc_okl)
  rel_maxima_maxima_book_doc_okl := $(maxima_book_doc_okl)
  rel_maxima_gnuplot_pdf_doc_okl := $(gnuplot_pdf_doc_okl)
  rel_maxima_maxima_gf_manual_okl := $(maxima_gf_manual_okl)
else
  rel_maxima_maxima_main_index_okl := $(shell $(rel_path_tool) $(maxima_main_index_okl) $(maxima_html_output_okl))
  rel_maxima_maxima_tutorial_doc_okl := $(shell $(rel_path_tool) $(maxima_tutorial_doc_okl) $(maxima_html_output_okl))
  rel_maxima_maxima_book_doc_okl := $(shell $(rel_path_tool) $(maxima_book_doc_okl) $(maxima_html_output_okl))
  rel_maxima_gnuplot_pdf_doc_okl := $(shell $(rel_path_tool) $(gnuplot_pdf_doc_okl) $(maxima_html_output_okl))
  rel_maxima_maxima_gf_manual_okl := $(shell $(rel_path_tool) $(maxima_gf_manual_okl) $(maxima_html_output_okl))
endif

# From the Gcc page:
ifeq ($(extsrc_relative),)
   rel_gcc_gcc_main_index_okl := $(gcc_main_index_okl)
   rel_gcc_gcc_main_dvi_okl := $(gcc_main_dvi_okl)
   rel_gcc_gcc_cpp_index_okl := $(gcc_cpp_index_okl)
   rel_gcc_gcc_cpp_dvi_okl := $(gcc_cpp_dvi_okl)
   rel_gcc_gcc_install_index_okl := $(gcc_install_index_okl)
   rel_gcc_gcc_install_dvi_okl := $(gcc_install_dvi_okl)
   rel_gcc_gcc_internals_index_okl := $(gcc_internals_index_okl)
   rel_gcc_gcc_internals_dvi_okl := $(gcc_internals_dvi_okl)
   rel_gcc_gcc_libiberty_index_okl := $(gcc_libiberty_index_okl)
else
  rel_gcc_gcc_main_index_okl := $(shell $(rel_path_tool) $(gcc_main_index_okl) $(gcc_html_output_okl))
  rel_gcc_gcc_main_dvi_okl := $(shell $(rel_path_tool) $(gcc_main_dvi_okl) $(gcc_html_output_okl))
  rel_gcc_gcc_cpp_index_okl := $(shell $(rel_path_tool) $(gcc_cpp_index_okl) $(gcc_html_output_okl))
  rel_gcc_gcc_cpp_dvi_okl := $(shell $(rel_path_tool) $(gcc_cpp_dvi_okl) $(gcc_html_output_okl))
  rel_gcc_gcc_install_index_okl := $(shell $(rel_path_tool) $(gcc_install_index_okl) $(gcc_html_output_okl))
  rel_gcc_gcc_install_dvi_okl := $(shell $(rel_path_tool) $(gcc_install_dvi_okl) $(gcc_html_output_okl))
  rel_gcc_gcc_internals_index_okl := $(shell $(rel_path_tool) $(gcc_internals_index_okl) $(gcc_html_output_okl))
  rel_gcc_gcc_internals_dvi_okl := $(shell $(rel_path_tool) $(gcc_internals_dvi_okl) $(gcc_html_output_okl))
  rel_gcc_gcc_libiberty_index_okl := $(shell $(rel_path_tool) $(gcc_libiberty_index_okl) $(gcc_html_output_okl))
endif

# From the Gap page:
ifeq ($(extsrc_relative),)
  rel_gap_gap_main_index_okl := $(gap_main_index_okl)
  rel_gap_gap_tutorial_pdf_okl := $(gap_tutorial_pdf_okl)
  rel_gap_gap_manual_pdf_okl := $(gap_manual_pdf_okl)
  rel_gap_gap_programming_pdf_okl := $(gap_programming_pdf_okl)
  rel_gap_gap_monoid_pdf_okl := $(gap_monoid_pdf_okl)
  rel_gap_gap_monoid_html_okl := $(gap_monoid_html_okl)
  rel_gap_gap_grape_pdf_okl := $(gap_grape_pdf_okl)
else
  rel_gap_gap_main_index_okl := $(shell $(rel_path_tool) $(gap_main_index_okl) $(gap_html_output_okl))
  rel_gap_gap_tutorial_pdf_okl := $(shell $(rel_path_tool) $(gap_tutorial_pdf_okl) $(gap_html_output_okl))
  rel_gap_gap_manual_pdf_okl := $(shell $(rel_path_tool) $(gap_manual_pdf_okl) $(gap_html_output_okl))
  rel_gap_gap_programming_pdf_okl := $(shell $(rel_path_tool) $(gap_programming_pdf_okl) $(gap_html_output_okl))
  rel_gap_gap_monoid_pdf_okl := $(shell $(rel_path_tool) $(gap_monoid_pdf_okl) $(gap_html_output_okl))
  rel_gap_gap_monoid_html_okl := $(shell $(rel_path_tool) $(gap_monoid_html_okl) $(gap_html_output_okl))
  rel_gap_gap_grape_pdf_okl := $(shell $(rel_path_tool) $(gap_grape_pdf_okl) $(gap_html_output_okl))
endif

