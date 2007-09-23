# Matthew Henderson, 25.6.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

R-base-directory := $(prefix)/R
R-directories := $(R-base-directory)

.PHONY : R $(R_targets) create_R_dirs

# #################################
# Main R targets
# #################################

$(R-directories) : % : 
	mkdir -p $@

R : $(R_recommended)

$(R_targets) : $(R-directories)
	$(call unarchive,sources/R/$@,$(R-base-directory))
	cd $(R-base-directory)/$@; $(postcondition) \
	./configure; $(postcondition) \
	make; $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallR : 
	-rm -rf $(R-base-directory)
