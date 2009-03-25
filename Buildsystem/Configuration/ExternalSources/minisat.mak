# Oliver Kullmann, 25.3.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the various forms of Minisat-solvers

# The "old" Minisat

minisat_recommended_version_number_okl ?= v1.14
minisat_extracted_archive_okl ?= MiniSat_$(minisat_recommended_version_number_okl)

minisat_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Minisat
minisat_build_dir_okl ?= $(minisat_base_build_dir_okl)/$(minisat_extracted_archive_okl)
minisat_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Minisat
minisat_installation_dir_okl ?= $(minisat_base_installation_dir_okl)/$(minisat_recommended_version_number_okl)

minisat_call_okl ?= $(minisat_installation_dir_okl)/minisat_static

minisat_docu_page_okl ?= $(doc_dir)/doxygen_html/

minisat_source_okl := $(ExternalSources)/sources/SAT/MiniSat/$(minisat_extracted_archive_okl).2006-Aug-29



# The "new" Minisat

minisat2_recommended_version_number_okl ?= 070721
minisat2_supported_but_not_recommended_version_numbers_okl ?= 
minisat2_supported_version_numbers_okl ?= $(minisat2_recommended_version_number_okl) $(minisat2_supported_but_not_recommended_version_numbers_okl)


# The pseudo-boolean solver

minisatp_recommended_version_number_okl ?= 2007-Jan-05
minisatp_supported_but_not_recommended_version_numbers_okl ?= 
minisatp_supported_version_numbers_okl ?= $(minisatp_recommended_version_number_okl) $(minisatp_supported_but_not_recommended_version_numbers_okl)

