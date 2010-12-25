# Matthew Henderson, 25.6.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

gmp_directories_okl := $(gmp_base_installation_dir_okl) $(gmp_base_build_dir_okl) $(gmp_gccbuild_dir_okl) $(gmp_locsys_base_build_dir_okl) $(gmp_base_doc_dir_okl) $(gmp_doc_dir_okl)

$(gmp_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main gmp targets
# #################################

.PHONY : gmp cleangmp cleanallgmp gmplocsys

gmp : $(gmp_directories_okl)
	$(call unarchive,$(gmp_source_dir_okl),$(gmp_gccbuild_dir_okl)) $(postcondition) \
	cd $(gmp_build_dir_okl); $(postcondition) \
	./configure --prefix=$(gmp_install_directory_okl) --enable-cxx CC=$(gcc_call_okl) CXX=$(gpp_call_okl); $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make html; $(postcondition) \
	cp -r doc/gmp.html $(gmp_doc_dir_okl); $(postcondition) \
	cd doc && make gmp.dvi; $(postcondition) \
	cp gmp.dvi $(gmp_doc_dir_okl); $(postcondition) \
	cd ..; $(postcondition) \
	$(gmp_install_command_okl)

gmplocsys : $(gmp_directories_okl)
	$(call unarchive,$(gmp_source_dir_okl),$(gmp_locsys_base_build_dir_okl)) $(postcondition) \
	cd $(gmp_locsys_build_dir_okl); $(postcondition) \
	./configure --prefix=$(gmp_locsys_install_directory_okl) --enable-cxx; $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make install


# #################################
# Cleaning
# #################################

cleangmp : 
	-rm -rf $(gmp_base_build_dir_okl)

cleanallgmp : cleangmp
	-rm -rf $(gmp_base_installation_dir_okl) $(gmp_base_doc_dir_okl)
