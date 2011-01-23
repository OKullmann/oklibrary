# Oliver Kullmann, 30.11.2010 (Swansea)
# Copyright 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

espresso_directories_okl := $(espresso_base_installation_dir_okl) $(espresso_installation_dir_okl) $(espresso_base_build_dir_okl) $(espresso_base_doc_dir_okl) $(espresso_doc_dir_okl)

$(espresso_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main espresso targets
# #################################

.PHONY : espresso espresso_doc cleanespresso cleanallespresso


espresso : $(espresso_directories_okl)
	$(call unarchive,$(espresso_source_okl),$(espresso_base_build_dir_okl)) $(postcondition) \
	cd $(espresso_build_dir_okl); $(postcondition) \
	CC=$(gcc_call_okl) CXX=$(gpp_call_okl) ./configure; $(postcondition) \
	make all; $(postcondition) \
	cp -f src/$(espresso_exec_okl) $(espresso_call_okl); $(postcondition) \
	cp -f README $(espresso_doc_dir_okl); $(postcondition) \
	cp $(espresso_base_source_okl)/*.html $(espresso_doc_dir_okl); $(postcondition) \
	ln -s --force $(espresso_call_okl) $(public_bin_dir_okl)/$(espresso_public_call_okl); $(postcondition)

espresso_doc :


# #################################
# Cleaning
# #################################

cleanespresso :
	-rm -rf $(espresso_base_build_dir_okl)

cleanallespresso : cleanespresso
	-rm -rf $(espresso_base_installation_dir_okl) $(espresso_base_doc_dir_okl)
