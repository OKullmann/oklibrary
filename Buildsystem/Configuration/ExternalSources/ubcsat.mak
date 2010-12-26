# Oliver Kullmann, 6.9.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the Ubcsat package

ubcsat_recommended_version_number_okl ?= 1-0-0
ubcsat_other_versions_okl ?= 1-2-0-beta

ubcsat_prefix_okl ?= ubcsat

ubcsat_recommended_okl := $(ubcsat_prefix_okl)-$(ubcsat_recommended_version_number_okl)
ubcsat_source_okl := $(ExternalSources)/sources/SAT/Ubcsat/$(ubcsat_recommended_okl)

ubcsat_base_installation_dir_okl ?= $(ExternalSources_installations)/SAT/Ubcsat
ubcsat_installation_dir_okl ?= $(ubcsat_base_installation_dir_okl)/$(ubcsat_recommended_version_number_okl)
ubcsat_base_build_dir_okl ?= $(ExternalSources_builds)/SAT/Ubcsat
ubcsat_build_dir_okl ?= $(ubcsat_base_build_dir_okl)/$(ubcsat_recommended_version_number_okl)
# ubcsat_build_dir_okl needs update to the usual setting

ubcsat_installsrc_okl ?= $(ubcsat_installation_dir_okl)/src
ubcsat_include_option_okl ?= -I- -I$(OKsystem)/OKlib/Satisfiability/Algorithms/LocalSearch/Ubcsat/local -I$(OKsystem)/OKlib/Satisfiability/Algorithms/LocalSearch/Ubcsat/corrected -I$(ubcsat_installsrc_okl)
ubcsat_link_okl ?= $(ubcsat_installation_dir_okl)/lib
ubcsat_link_option_okl ?= -L$(ubcsat_link_okl) -lubcsat

ubcsat_call_okl ?= $(ubcsat_installation_dir_okl)/bin/$(ubcsat_recommended_okl)

ubcsat_docu_page_okl ?= $(doc_dir)/doxygen_html/d6/d0c/docus_2Ubcsat_8hpp.html

ubcsat_homepage_url_okl := http://www.satlib.org/ubcsat/
ubcsat_documentation_url_okl := http://www.satlib.org/ubcsat/\#documentation

ubcsat_corrected_dir_okl := $(OKsystem)/OKlib/Satisfiability/Algorithms/LocalSearch/Ubcsat/corrected
ubcsat_corrected_files_okl := $(wildcard $(ubcsat_corrected_dir_okl)/*.h $(ubcsat_corrected_dir_okl)/*.c)

ubcsat_wrapper_okl ?= $(OKlib)/Experimentation/ExperimentSystem/ControllingLocalSearch/ubcsat-okl
ubcsat_defaults_okl ?= -rclean -r out stdout run,found,best,beststep,steps,seed  -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] -runs 10 -cutoff 100000

script_name_ubcsat_okl ?= new-ubcsat-okl
new_ubcsat_wrapper_okl ?= $(OKlib)/Experimentation/ExperimentSystem/ControllingLocalSearch/$(script_name_ubcsat_okl)
new_ubcsat_defaults_okl ?= -rclean -r out stdout run,found,best,beststep,steps,seed  -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] -runs 10 -cutoff 100000 -rflush
