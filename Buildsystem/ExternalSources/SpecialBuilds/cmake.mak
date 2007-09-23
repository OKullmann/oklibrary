# Oliver Kullmann, 5.6.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

cmake-base-directory := $(prefix)/Cmake
cmake_doc_dir := $(external_sources_doc_base_dir)/Cmake
cmake-directories := $(cmake-base-directory) $(cmake_doc_dir)

.PHONY : cmake $(cmake_targets) create_cmake_dirs

# #################################
# Main Cmake targets
# #################################

$(cmake-directories) : % : 
	mkdir -p $@

create_cmake_dirs : $(cmake-directories)

cmake : $(cmake_recommended)

$(cmake_targets) : create_cmake_dirs
	$(call unarchive,sources/Cmake/$@,$(cmake-base-directory)) $(postcondition) \
	cd $(cmake-base-directory)/$@; $(postcondition) \
	./bootstrap; $(postcondition) \
	make; $(postcondition) \
	cp -r Docs Example $(cmake_doc_dir); $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallcmake : 
	-rm -rf $(cmake-base-directory)
