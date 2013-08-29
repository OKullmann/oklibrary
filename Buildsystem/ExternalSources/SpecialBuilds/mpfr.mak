# Matthew Henderson, 25.6.2007 (Swansea)
# Copyright 2007, 2008, 2010, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

mpfr_directories_okl := $(mpfr_base_installation_dir_okl) $(mpfr_base_build_dir_okl) $(mpfr_gccbuild_dir_okl) $(mpfr_base_doc_dir_okl) $(mpfr_doc_dir_okl)

$(mpfr_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main mpfr targets
# #################################

.PHONY : mpfr cleanmpfr cleanallmpfr

mpfr : $(mpfr_directories_okl)
	$(call unarchive,$(mpfr_source_okl),$(mpfr_gccbuild_dir_okl)) $(postcondition) \
	cd $(mpfr_build_dir_okl); $(postcondition) \
	./configure --prefix=$(mpfr_install_directory_okl) --with-gmp=$(gmp_install_directory_okl) CC=$(gcc_call_okl); $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make html; $(postcondition) \
	cp -r doc/* $(mpfr_doc_dir_okl); $(postcondition) \
	$(mpfr_install_command_okl)


# #################################
# Cleaning
# #################################

cleanmpfr : 
	-rm -rf $(mpfr_base_build_dir_okl)

cleanallmpfr : cleanmpfr
	-rm -rf $(mpfr_base_installation_dir_okl) $(mpfr_base_doc_dir_okl)
