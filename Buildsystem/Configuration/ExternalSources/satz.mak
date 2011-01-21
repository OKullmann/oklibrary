# Oliver Kullmann, 10.4.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the various forms of Satz-solvers

satz_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Satz
satz_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Satz

# Satz 215

satz215_recommended_version_number_okl ?= 215
satz215_build_dir_okl ?= $(satz_base_build_dir_okl)/satz215
satz215_installation_dir_okl ?= $(satz_base_installation_dir_okl)/215
satz215_corrected_src_okl ?= $(OKsystem)/OKlib/Satisfiability/Solvers/Satz/satz215.2.c

satz215_call_okl ?= $(satz215_installation_dir_okl)/satz215
satz215_wrapper_okl ?= $(OKlib)/Satisfiability/Solvers/Satz/satz215
satz215_m2pp_okl ?= $(OKlib)/Satisfiability/Solvers/Satz/satz215-m2pp

satz215_docu_page_okl ?= $(doc_dir)/doxygen_html/

satz215_source_okl := $(ExternalSources)/sources/SAT/Satz/satz215.2.c

