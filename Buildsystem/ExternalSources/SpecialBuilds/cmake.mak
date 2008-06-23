# Oliver Kullmann, 5.6.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# ##################################
# Directory Structure
# ################################## 

cmake_base_build_dir_okl := $(ExternalSources_builds)/Cmake
cmake_base_doc_dir_okl := $(ExternalSources_doc)/Cmake
cmake-directories := $(cmake_base_build_dir_okl) $(cmake_base_doc_dir_okl)

.PHONY : cmake $(cmake_targets) create_cmake_dirs

# #################################
# Main Cmake targets
# #################################

$(cmake-directories) : % : 
	mkdir -p $@

create_cmake_dirs : $(cmake-directories)

cmake : $(cmake_recommended)

$(cmake_targets) : create_cmake_dirs
	$(call unarchive,$(ExternalSources)/sources/Cmake/$@,$(cmake_base_build_dir_okl)) $(postcondition) \
	cd $(cmake_base_build_dir_okl)/$@; $(postcondition) \
	./bootstrap; $(postcondition) \
	make; $(postcondition) \
	cp -r Docs Example $(cmake_base_doc_dir_okl); $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallcmake : 
	-rm -rf $(cmake_base_build_dir_okl)
