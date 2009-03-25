# Oliver Kullmann, 25.3.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

minisat_directories_okl := $(minisat_base_installation_dir_okl) $(minisat_installation_dir_okl) $(minisat_base_build_dir_okl)

$(minisat_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Minisat targets
# #################################

.PHONY : minisat cleanminisat cleanallminisat

minisat : $(minisat_directories_okl)
	$(call unarchive,$(minisat_source_okl),$(minisat_base_build_dir_okl)) $(postcondition) \
	cd $(minisat_build_dir_okl); $(postcondition) \
	make rs; $(postcondition) \
	cp minisat_static $(minisat_installation_dir_okl); $(postcondition) \
	ln -s --force $(minisat_call_okl) $(public_bin_dir_okl)/minisat1; $(postcondition)


# #################################
# Cleaning
# #################################

cleanminisat : 
	-rm -rf $(minisat_base_build_dir_okl)

cleanallminisat : cleanminisat
	-rm -rf $(minisat_base_installation_dir_okl)
