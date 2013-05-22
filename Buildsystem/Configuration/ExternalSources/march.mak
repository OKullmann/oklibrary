# Oliver Kullmann, 31.3.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using the various forms of March-solvers

march_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/March
march_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/March

# march_pl

marchpl_recommended_version_number_okl ?= pl
marchpl_extracted_archive_okl ?= march_$(marchpl_recommended_version_number_okl)

marchpl_build_dir_okl ?= $(march_base_build_dir_okl)/$(marchpl_extracted_archive_okl)
marchpl_installation_dir_okl ?= $(march_base_installation_dir_okl)/$(marchpl_recommended_version_number_okl)
marchpl_corrected_src_okl ?= $(OKsystem)/OKlib/Satisfiability/Solvers/March/

marchpl_call_okl ?= $(marchpl_installation_dir_okl)/march_pl

marchpl_docu_page_okl ?= $(doc_dir)/doxygen_html/

marchpl_source_okl := $(ExternalSources)/sources/SAT/March/$(marchpl_extracted_archive_okl)

