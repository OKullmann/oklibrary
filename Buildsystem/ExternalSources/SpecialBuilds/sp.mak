# Oliver Kullmann, 23.4.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

sp_directories_okl := $(sp_base_installation_dir_okl)

$(sp_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Sp targets
# #################################

.PHONY : sp cleanallsp

sp : $(sp_directories_okl)
	$(call unarchive,$(sp_source_okl),$(sp_base_installation_dir_okl)) $(postcondition) \
	cd $(sp_installation_dir_okl); $(postcondition) \
	make; $(postcondition) \
	ln -s --force $(sp_call_okl) $(public_bin_dir_okl)/survey_propagation; $(postcondition)


# #################################
# Cleaning
# #################################


cleanallsp :
	-rm -rf $(sp_base_installation_dir_okl)
