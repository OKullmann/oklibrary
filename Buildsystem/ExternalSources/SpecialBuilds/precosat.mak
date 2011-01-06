# Oliver Kullmann, 14.8.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

precosat_directories_okl := $(precosat_base_installation_dir_okl) $(precosat_installation_dir_okl) $(precosat_base_build_dir_okl) $(precosat_base_doc_dir_okl) $(precosat_doc_dir_okl)

$(precosat_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main precosat targets
# #################################

.PHONY : precosat precosat236 precosat570 precosat_gen precosat_doc cleanprecosat cleanallprecosat

precosat : precosat236 precosat570 precosat_doc

precosat236 :
	env -i PATH=$${PATH} gcc_recommended_version_number_okl=$(gcc_recommended_version_number_okl) make -f $(OKbuildsystem)/ExternalSources/Makefile OKplatform=$(OKplatform) precosat_recommended_version_number_okl="236" precosat_gen

precosat570 :
	env -i PATH=$${PATH} gcc_recommended_version_number_okl=$(gcc_recommended_version_number_okl) make -f $(OKbuildsystem)/ExternalSources/Makefile OKplatform=$(OKplatform) precosat_recommended_version_number_okl="-570-239dbbe-100801" precosat_gen

precosat_gen : $(precosat_directories_okl)
	$(call unarchive,$(precosat_source_okl),$(precosat_base_build_dir_okl)) $(postcondition) \
	cd $(precosat_build_dir_okl); $(postcondition) \
	./configure; $(postcondition) \
	make CC=$(gcc_call_okl) CXX=$(gpp_call_okl) all; $(postcondition) \
	cp -f $(precosat_exec_okl) $(precosat_call_okl); $(postcondition) \
	cp -f LICENSE $(precosat_doc_dir_okl); $(postcondition) \
	ln -s --force $(precosat_call_okl) $(public_bin_dir_okl)/$(precosat_public_call_okl); $(postcondition)

precosat_doc :
	cp $(precosat_base_source_okl)/*.pdf $(precosat_base_doc_dir_okl)

# #################################
# Cleaning
# #################################

cleanprecosat :
	-rm -rf $(precosat_base_build_dir_okl)

cleanallprecosat : cleanprecosat
	-rm -rf $(precosat_base_installation_dir_okl) $(precosat_base_doc_dir_okl)
