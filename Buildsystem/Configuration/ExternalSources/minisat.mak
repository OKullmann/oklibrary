# Oliver Kullmann, 25.3.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the various forms of Minisat-solvers

minisat_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Minisat
minisat_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Minisat
minisat_base_doc_dir_okl ?= $(ExternalSources_doc)/SAT/Minisat


# The new Minisat

minisat_recommended_version_number_okl ?= 2.2.0
minisat_other_versions_okl ?= 

minisat_base_name_okl := minisat
minisat_recommended_okl := $(minisat_base_name_okl)-$(minisat_recommended_version_number_okl)
minisat_base_source_okl := $(ExternalSources)/sources/SAT/MiniSat
minisat_source_okl := $(minisat_base_source_okl)/$(minisat_recommended_okl)

minisat_build_dir_okl ?= $(minisat_base_build_dir_okl)/$(minisat_recommended_okl)

minisat_installation_dir_okl ?= $(minisat_base_installation_dir_okl)/$(minisat_recommended_version_number_okl)

minisat_call_okl ?= $(minisat_installation_dir_okl)/minisat
minisat_public_call_okl ?= $(minisat_recommended_okl)

minisat_doc_dir_okl ?= $(minisat_base_doc_dir_okl)/$(minisat_recommended_okl)
minisat_docu_page_okl ?= $(doc_dir)/doxygen_html/

minisat_homepage_url_okl := http://minisat.se/MiniSat.html
minisat_documentation_url_okl := 
minisat_git_url_okl := https://github.com/niklasso/minisat

minisat_oklib_dir_okl ?= $(OKlib)/Satisfiability/Solvers/Minisat/Minisat2

# Minisat version 2.0

minisat2_recommended_version_number_okl ?= 070721
minisat2_other_versions_okl ?= 2.2.0

minisat2_extracted_archive_okl ?= minisat

minisat2_build_dir_okl ?= $(minisat_base_build_dir_okl)/$(minisat2_extracted_archive_okl)
minisat2_installation_dir_okl ?= $(minisat_base_installation_dir_okl)/$(minisat2_recommended_version_number_okl)

minisat2_call_okl ?= $(minisat2_installation_dir_okl)/minisat_static

minisat2_docu_page_okl ?= $(doc_dir)/doxygen_html/

minisat2_source_okl := $(ExternalSources)/sources/SAT/MiniSat/$(minisat2_extracted_archive_okl)2-$(minisat2_recommended_version_number_okl)



# The pseudo-boolean solver

minisatp_recommended_version_number_okl ?= 2007-Jan-05
minisatp_other_versions_okl ?= 
minisatp_supported_version_numbers_okl ?= $(minisatp_recommended_version_number_okl) $(minisatp_supported_but_not_recommended_version_numbers_okl)

minisatp_extracted_archive_okl ?= minisat+

minisatp_build_dir_okl ?= $(minisat_base_build_dir_okl)/$(minisatp_extracted_archive_okl)
minisatp_installation_dir_okl ?= $(minisat_base_installation_dir_okl)/$(minisatp_recommended_version_number_okl)
minisatp_extended_makefile_okl ?= $(OKsystem)/OKlib/Satisfiability/Solvers/Minisat/Minisat+/Makefile
minisatp_corrected_okl ?= $(OKsystem)/OKlib/Satisfiability/Solvers/Minisat/Minisat+/ADTs/Int.h

minisatp_call_okl ?= $(minisatp_installation_dir_okl)/minisat+_64-bit_static
minisatpb_call_okl ?= $(minisatp_installation_dir_okl)/minisat+_bignum_static

minisatp_docu_page_okl ?= $(doc_dir)/doxygen_html/

minisatp_source_okl := $(ExternalSources)/sources/SAT/MiniSat/$(minisatp_extracted_archive_okl)_$(minisatp_recommended_version_number_okl)

minisatp_homepage_url_okl := http://minisat.se/MiniSat+.html
minisatp_documentation_url_okl := 
