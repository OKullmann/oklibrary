# Matthew Henderson, 6.3.2006 (Swansea)
# Copyright 2006-2007, 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

mhash_directories_okl := $(mhash_base_installation_dir_okl) $(mhash_base_build_dir_okl) $(mhash_gccbuild_dir_okl) $(mhash_base_doc_dir_okl) $(mhash_doc_dir_okl)

$(mhash_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main mhash targets
# #################################

.PHONY : mhash cleanmhash cleanallmhash

mhash : $(mhash_directories_okl)
	$(call unarchive,$(mhash_source_dir_okl),$(mhash_gccbuild_dir_okl)) $(postcondition) \
	cd $(mhash_build_dir_okl); $(postcondition) \
	./configure --prefix=$(mhash_installation_dir_okl) --with-CC=$(gcc412_call_okl); $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition) \
	cp $(mhash_source_doc_okl) $(mhash_html_documentation_index_location_okl); $(postcondition)

# #################################
# Cleaning
# #################################

cleanmhash : 
	-rm -rf $(mhash_base_build_dir_okl)

cleanallmhash : cleanmhash
	-rm -rf $(mhash_base_installation_dir_okl) $(mhash_base_doc_dir_okl)

