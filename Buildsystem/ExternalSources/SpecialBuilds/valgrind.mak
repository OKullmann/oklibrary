# Matthew Henderson, 19.7.2006 (Paderborn)
# Copyright 2006-2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# ##################################
# Directory Structure
# ################################## 

valgrind_directories_okl := $(valgrind_base_build_dir_okl) $(valgrind_base_doc_dir_okl) $(valgrind_doc_dir_okl)

.PHONY : valgrind cleanvalgrind cleanallvalgrind

# #################################
# Main Valgrind targets
# #################################

$(valgrind_directories_okl) : % : 
	mkdir -p $@

valgrind : $(valgrind_directories_okl)
	$(call unarchive,$(valgrind_source_okl),$(valgrind_base_build_dir_okl))
	cd $(valgrind_build_dir_okl); $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition) \
	cp -r $(valgrind_doc_dir_build_okl) $(valgrind_doc_dir_okl); $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanvalgrind : 
	-rm -rf $(valgrind_base_build_dir_okl)

cleanallvalgrind : cleanvalgrind
	-rm -rf $(valgrind_base_doc_dir_okl)
