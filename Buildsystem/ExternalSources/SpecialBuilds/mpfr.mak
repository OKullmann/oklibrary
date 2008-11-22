# Matthew Henderson, 25.6.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# ##################################
# Directory Structure
# ################################## 

mpfr_base_build_dir_okl := $(ExternalSources_builds)/Mpfr
mpfr_directories := $(mpfr_base_build_dir_okl)

.PHONY : mpfr $(mpfr_targets) create_mpfr_dirs

# #################################
# Main mpfr targets
# #################################

$(mpfr_directories) : % : 
	mkdir -p $@

mpfr : $(mpfr_recommended)

$(mpfr_targets) : $(mpfr_directories)
	$(call unarchive,$(ExternalSources)/sources/Gmp/$@,$(mpfr_base_build_dir_okl))
	cd $(mpfr_base_build_dir_okl)/$@; $(postcondition) \
	./configure; $(postcondition) \
	make; $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallmpfr : 
	-rm -rf $(mpfr_base_build_dir_okl)
