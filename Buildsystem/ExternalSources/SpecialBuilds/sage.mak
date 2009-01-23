# Matthew Henderson, 21.7.2007 (Swansea)
# Copyright 2007, 2008, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 


sage_directories_okl := $(sage_base_installation_dir_okl) $(sage_base_doc_dir_okl) $(sage_doc_dir_okl)

.PHONY : sage cleanallsage

$(sage_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main sage targets
# #################################

sage : $(sage_directories_okl)
	$(call unarchive_uncompressed,$(sage_source_okl),$(sage_base_installation_dir_okl))
	cd $(sage_installation_dir_okl); ls; $(postcondition) \
	make; $(postcondition) \
	make test; $(postcondition) \
	sed --in-place --expression='s|^SAGE_ROOT="....."|SAGE_ROOT="$(sage_installation_dir_okl)"|' ./sage; $(postcondition)
	ln -s --force $(sage_call_okl) $(public_bin_dir_okl)/sage

# #################################
# Cleaning
# #################################

cleanallsage : 
	-rm -rf $(sage_base_installation_dir_okl)
