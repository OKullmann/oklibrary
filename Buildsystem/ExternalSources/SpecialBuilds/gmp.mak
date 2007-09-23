# Matthew Henderson, 25.6.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

gmp_directories := $(gmp_base_directory) $(gmp_documentation_dir)

.PHONY : gmp $(gmp_targets)

# #################################
# Main gmp targets
# #################################

$(gmp_directories) : % : 
	mkdir -p $@

gmp : $(gmp_recommended)

$(gmp_targets) : $(gmp_directories)
	$(call unarchive,sources/Gmp/$@,$(gmp_base_directory)) $(postcondition) \
	cd $(gmp_base_directory)/$@; $(postcondition) \
	./configure --prefix=$(gmp_install_directory); $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make html; $(postcondition) \
	cp -r doc/gmp.html $(gmp_documentation_dir); $(postcondition) \
	cd doc && make gmp.dvi; $(postcondition) \
	cp gmp.dvi $(gmp_documentation_dir); $(postcondition) \
	cd ..; $(postcondition) \
	$(gmp_install_command)

# #################################
# Cleaning
# #################################

cleanallgmp : 
	-rm -rf $(gmp_base_directory)
