# Matthew Henderson, 25.6.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

mpfr-base-directory := $(prefix)/mpfr
mpfr-directories := $(mpfr-base-directory)

.PHONY : mpfr $(mpfr_targets) create_mpfr_dirs

# #################################
# Main mpfr targets
# #################################

$(mpfr-directories) : % : 
	mkdir -p $@

mpfr : $(mpfr_recommended)

$(mpfr_targets) : $(mpfr-directories)
	$(call unarchive,$@,$(mpfr-base-directory))
	cd $(mpfr-base-directory)/$@; $(postcondition) \
	./configure; $(postcondition) \
	make; $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallmpfr : 
	-rm -rf $(mpfr-base-directory)
