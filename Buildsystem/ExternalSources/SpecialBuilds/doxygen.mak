# Matthew Henderson, 19.7.2006 (Paderborn)
# Copyright 2006-2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ##################################

#Root directory ".", where this makefile is located, contains also all software source archives and sub-directories.

#In the following text, ? denotes the Doxygen version number.
#./Doxygen : Contain unarchived various versions of Doxygen.
#./Doxygen/doxygen-? : This is the original unarchived source directory which is also used for configuration and building. Doxygen is only installed system-wide.

doxygen-base-directory := $(prefix)/Doxygen
doxygen_doc_dir := $(external_sources_doc_base_dir)/Doxygen
doxygen-directories := $(doxygen-base-directory) $(doxygen_doc_dir)


# ##################################
# The main targets for doxygen
# ##################################

.PHONY : doxygen $(doxygen_targets) create_doxygen_dirs

$(doxygen-directories) : % : 
	mkdir -p $@

create_doxygen_dirs : $(doxygen-directories)

doxygen : $(doxygen_recommended)

$(doxygen_targets) : create_doxygen_dirs
	$(call unarchive,sources/Doxygen/$@.src,$(doxygen-base-directory)) \
	cd $(doxygen-base-directory)/$@; $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition) \
	make docs; $(postcondition) \
	make pdf; $(postcondition) \
	sudo make install; $(postcondition) \
	sudo make install_docs; $(postcondition) \
	cp -r $(doxygen-base-directory)/$@ $(doxygen_doc_dir)/$@; $(postcondition)


# ####################################
# Cleaning
# ####################################

cleanalldoxygen :
	-rm -rf $(doxygen-base-directory)
