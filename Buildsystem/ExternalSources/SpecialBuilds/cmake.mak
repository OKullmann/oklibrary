# Oliver Kullmann, 5.6.2007 (Swansea)
# Copyright 2007, 2008, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# ##################################
# Directory Structure
# ################################## 

cmake_directories_okl := $(cmake_base_installation_dir_okl) $(cmake_base_build_dir_okl) $(cmake_base_doc_dir_okl) $(cmake_doc_dir_okl)

$(cmake_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Cmake targets
# #################################


.PHONY : cmake cmake_base cmake_links cleancmake cleanallcmake

cmake : cmake_base cmake_links


cmake_base : $(cmake_directories_okl)
	$(call unarchive,$(cmake_source_okl),$(cmake_base_build_dir_okl)) $(postcondition) \
	cd $(cmake_build_dir_okl); $(postcondition) \
	./bootstrap --prefix=$(cmake_install_directory_okl); $(postcondition) \
	make; $(postcondition) \
	$(cmake_install_command_okl); $(postcondition) \
	cp -r $(cmake_install_directory_okl)/doc/cmake-*/* Example $(cmake_doc_dir_okl); $(postcondition)

ifeq ($(cmake_default_install_okl),local)
cmake_links :
	ln -s --force $(cmake_call_okl) $(public_bin_dir_okl)/cmake; $(postcondition)
else
cmake_links :
endif


# #################################
# Cleaning
# #################################

cleancmake : 
	-rm -rf $(cmake_base_build_dir_okl)

cleanallcmake : cleancmake
	-rm -rf $(cmake_base_installation_dir_okl) $(cmake_base_doc_dir_okl)
