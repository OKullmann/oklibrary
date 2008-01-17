# Matthew Henderson, 19.7.2006 (Paderborn)
# Copyright 2006-2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

valgrind_directories_okl := $(valgrind_base_build_dir_okl) $(valgrind_base_doc_dir_okl) $(valgrind_doc_dir_okl)

.PHONY : valgrind $(valgrind_recommended_okl)

# #################################
# Main Valgrind targets
# #################################

$(valgrind_directories_okl) : % : 
	mkdir -p $@

valgrind : $(valgrind_recommended_okl)

$(valgrind_recommended_okl) : $(valgrind_directories_okl)
	$(call unarchive,$(ExternalSources)/sources/Valgrind/$@,$(valgrind_base_build_dir_okl))
	cd $(valgrind_build_dir_okl); $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition) \
	cp -r $(valgrind_doc_dir_build_okl) $(valgrind_doc_dir_okl)
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallvalgrind : 
	-rm -rf $(valgrind_base_build_dir_okl) $(valgrind_base_doc_dir_okl)
