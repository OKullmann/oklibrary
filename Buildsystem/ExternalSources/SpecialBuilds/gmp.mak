# Matthew Henderson, 25.6.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

gmp_directories_okl := $(gmp_base_installation_dir_okl) $(gmp_base_build_dir_okl) $(gmp_gccbuild_dir_okl) $(gmp_base_doc_dir_okl) $(gmp_doc_dir_okl) $(gmp_base_build_dir_okl)/4.1.2 $(gmp_base_doc_dir_okl)/4.1.2

$(gmp_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main gmp targets
# #################################

.PHONY : gmp cleangmp cleanallgmp gmp412

gmp : $(gmp_directories_okl)
	$(call unarchive,$(gmp_source_okl),$(gmp_gccbuild_dir_okl)) $(postcondition) \
	cd $(gmp_build_dir_okl); $(postcondition) \
	./configure --prefix=$(gmp_install_directory_okl) --libdir=$(gmp_lib_directory_okl) --enable-cxx CC=$(gcc_call_okl) CXX=$(gpp_call_okl); $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make html; $(postcondition) \
	cp -r doc/gmp.html $(gmp_doc_dir_okl); $(postcondition) \
	cd doc && make gmp.dvi; $(postcondition) \
	cp gmp.dvi $(gmp_doc_dir_okl); $(postcondition) \
	cd ..; $(postcondition) \
	$(gmp_install_command_okl)

# as long as the OKlibrary is built using gcc-4.1.2 (to be removed after that):
gmp412 : $(gmp_directories_okl)
	$(call unarchive,$(gmp_source_okl),$(gmp_base_build_dir_okl)/4.1.2) $(postcondition) \
	cd $(gmp_base_build_dir_okl)/4.1.2/$(gmp_recommended_okl); $(postcondition) \
	./configure --prefix=$(gmp_base_installation_dir_okl)/4.1.2/$(gmp_recommended_version_number_okl) --libdir=$(gmp412_lib_directory_okl) --enable-cxx CC=$(gcc412_call_okl) CXX=$(gpp412_call_okl); $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition)


# #################################
# Cleaning
# #################################

cleangmp : 
	-rm -rf $(gmp_base_build_dir_okl)

cleanallgmp : cleangmp
	-rm -rf $(gmp_base_installation_dir_okl) $(gmp_base_doc_dir_okl)
