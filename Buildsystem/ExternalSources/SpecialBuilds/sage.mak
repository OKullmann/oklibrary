# Matthew Henderson, 21.7.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# ##################################
# Directory Structure
# ################################## 


sage_directories := $(sage_base_directory)

.PHONY : sage $(sage_targets)

# #################################
# Main sage targets
# #################################

$(sage_directories) : % : 
	mkdir -p $@

sage : $(sage_recommended)

$(sage_targets) : $(sage_directories)
	$(call unarchive_uncompressed,$(ExternalSources)/sources/Sage/$@,$(sage_base_directory))
	cd $(sage_base_directory)/$@; $(postcondition) \
	make; $(postcondition) \
	make test; $(postcondition) \
	ln -s --force $(sage_call_okl) $(public_bin_dir_okl)/sage

# #################################
# Cleaning
# #################################

cleanallsage : 
	-rm -rf $(sage-base-directory)
