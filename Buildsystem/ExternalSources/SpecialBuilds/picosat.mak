# Matthew Gwynne, 31.7.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

picosat_directories_okl := $(picosat_base_installation_dir_okl) $(picosat_installation_dir_okl) $(picosat_base_build_dir_okl) $(picosat_base_doc_dir_okl) $(picosat_doc_dir_okl)

$(picosat_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main picosat targets
# #################################

.PHONY : picosat cleanpicosat cleanallpicosat

picosat : $(picosat_directories_okl)
	$(call unarchive,$(picosat_source_okl),$(picosat_base_build_dir_okl)) $(postcondition) \
	cd $(picosat_build_dir_okl); $(postcondition) \
	./configure; $(postcondition) \
	make CC=$(gcc_call_okl) CXX=$(gpp_call_okl) all; $(postcondition) \
	cp -f $(picosat_exec_okl) $(picosat_call_okl); $(postcondition) \
	cp -f README LICENSE $(picosat_doc_dir_okl); $(postcondition) \
	ln -s --force $(picosat_call_okl) $(public_bin_dir_okl)/$(picosat_exec_okl); $(postcondition)

# #################################
# Cleaning
# #################################

cleanpicosat :
	-rm -rf $(picosat_base_build_dir_okl)

cleanallpicosat : cleanpicosat
	-rm -rf $(picosat_base_installation_dir_okl) $(picosat_base_doc_dir_okl)
