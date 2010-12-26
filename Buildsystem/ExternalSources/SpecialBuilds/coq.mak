# Oliver Kullmann, 23.7.2007 (Swansea)
# Copyright 2007, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE TO NEW SYSTEM

# ##################################
# Directory Structure
# ################################## 


coq_directories := $(coq_base_directory)

.PHONY : coq $(coq_targets)

# #################################
# Main coq targets
# #################################

$(coq_directories) : % : 
	mkdir -p $@

coq : $(coq_recommended)

$(coq_targets) : coq-% : $(coq_directories)
	$(call unarchive,$(ExternalSources)/sources/Coq/$@,$(coq_base_directory))
	cd $(coq_base_directory)/$@; $(postcondition) \
	export PATH=$(ocaml_bin_dir):${PATH}; $(postcondition) \
	./configure -opt --prefix $(coq_base_directory)/$* -fsets all -reals all; $(postcondition) \
	make world && make install; $(postcondition) \

# #################################
# Cleaning
# #################################

cleanallcoq : 
	-rm -rf $(coq-base-directory)

