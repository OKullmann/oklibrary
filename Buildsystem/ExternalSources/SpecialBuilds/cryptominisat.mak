# Matthew Gwynne, 7.1.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

cryptominisat_directories_okl := $(cryptominisat_base_installation_dir_okl) $(cryptominisat_installation_dir_okl) $(cryptominisat_base_build_dir_okl) $(cryptominisat_base_doc_dir_okl) $(cryptominisat_doc_dir_okl)

$(cryptominisat_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main cryptominisat targets
# #################################

.PHONY : cryptominisat cryptominisat_doc cleancryptominisat cleanallcryptominisat cryptominisat_git


cryptominisat : $(cryptominisat_directories_okl)
	$(call unarchive,$(cryptominisat_source_okl),$(cryptominisat_base_build_dir_okl)) $(postcondition) \
	cd $(cryptominisat_build_dir_okl); $(postcondition) \
	./configure; $(postcondition) \
	make CC=$(gcc_call_okl) CXX=$(gpp_call_okl) all; $(postcondition) \
	cp -f $(cryptominisat_exec_okl) $(cryptominisat_call_okl); $(postcondition) \
	ln -s --force $(cryptominisat_call_okl) $(public_bin_dir_okl)/$(cryptominisat_public_call_okl); $(postcondition)


cryptominisat_git : $(cryptominisat_directories_okl)
	$(call unarchive,$(cryptominisat_source_okl),$(cryptominisat_base_build_dir_okl)) $(postcondition) \
	cd $(cryptominisat_build_dir_okl); $(postcondition) \
	make CC=$(gcc_call_okl) CXX=$(gpp_call_okl) -f Makefile.cvs; $(postcondition) \
	cp -f $(cryptominisat_exec_okl) $(cryptominisat_call_okl); $(postcondition) \
	ln -s --force $(cryptominisat_call_okl) $(public_bin_dir_okl)/$(cryptominisat_public_call_okl); $(postcondition)


cryptominisat_doc :


# #################################
# Cleaning
# #################################

cleancryptominisat :
	-rm -rf $(cryptominisat_base_build_dir_okl)

cleanallcryptominisat : cleancryptominisat
	-rm -rf $(cryptominisat_base_installation_dir_okl) $(cryptominisat_base_doc_dir_okl)
