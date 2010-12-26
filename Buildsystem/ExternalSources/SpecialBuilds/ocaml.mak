# Oliver Kullmann, 23.7.2007 (Swansea)
# Copyright 2007, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE TO NEW SYSTEM

# Building Ocaml

# ##################################
# Directory Structure
# ################################## 


ocaml_directories := $(ocaml_base_directory)

.PHONY : ocaml $(ocaml_targets)

# #################################
# Main ocaml targets
# #################################

$(ocaml_directories) : % : 
	mkdir -p $@

ocaml : $(ocaml_recommended)

$(ocaml_targets) : ocaml-% : $(ocaml_directories)
	$(call unarchive,$(ExternalSources)/sources/Ocaml/$@,$(ocaml_base_directory))
	cd $(ocaml_base_directory)/$@; $(postcondition) \
	./configure -prefix $(ocaml_base_directory)/$*; $(postcondition) \
	make world && make bootstrap && make opt && make opt.opt && make install && make clean

# #################################
# Cleaning
# #################################

cleanallocaml : 
	-rm -rf $(ocaml-base-directory)

