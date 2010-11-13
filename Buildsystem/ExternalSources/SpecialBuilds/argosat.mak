# Oliver Kullmann, 10.5.2010 (Swansea)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

argosat_directories_okl := $(argosat_base_installation_dir_okl) $(argosat_base_build_dir_okl) $(argosat_base_doc_dir_okl) $(argosat_doc_dir_okl)

$(argosat_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main argosat targets
# #################################

.PHONY : argosat cleanargosat cleanallargosat

argosat : $(argosat_directories_okl)
	$(call unarchive,$(argosat_source_okl),$(argosat_base_build_dir_okl))
	cd $(argosat_build_dir_okl); $(postcondition) \
	./configure --prefix=$(argosat_installation_dir_okl) CXX=$(gpp_call_okl); $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition) \
	ln -s --force $(argosat_call_okl) $(public_bin_dir_okl)/argosat; $(postcondition)

# #################################
# Cleaning
# #################################

cleanargosat : 
	-rm -rf $(argosat_base_build_dir_okl)

cleanallargosat : cleanargosat
	-rm -rf $(argosat_base_installation_dir_okl) $(argosat_base_doc_dir_okl)

