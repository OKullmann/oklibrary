# Oliver Kullmann, 18.7.2008 (Swansea)
# Copyright 2008, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

grasp_directories_okl := $(grasp_base_installation_dir_okl) $(grasp_installation_dir_okl) $(grasp_base_build_dir_okl) $(grasp_base_doc_dir_okl) $(grasp_doc_dir_okl)

$(grasp_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Grasp targets
# #################################

.PHONY : grasp cleangrasp cleanallgrasp

grasp : $(grasp_directories_okl)
	$(call unarchive,$(grasp_source_okl),$(grasp_base_build_dir_okl)) $(postcondition) \
	cd $(grasp_build_dir_okl); $(postcondition) \
	make CC=$(gcc412_call_okl) CXX=$(gpp412_call_okl) all; $(postcondition) \
	cp -f $(grasp_exec_okl) $(grasp_call_okl); $(postcondition) \
	cp -f README COPYRIGHT LICENSE GPL-3.0.txt $(grasp_exec_okl).doc $(grasp_exec_okl).html $(grasp_doc_dir_okl); $(postcondition) \
	ln -s --force $(grasp_call_okl) $(public_bin_dir_okl)/$(grasp_exec_okl); $(postcondition)

# #################################
# Cleaning
# #################################

cleangrasp :
	-rm -rf $(grasp_base_build_dir_okl)

cleanallgrasp : cleangrasp
	-rm -rf $(grasp_base_installation_dir_okl) $(grasp_base_doc_dir_okl)
