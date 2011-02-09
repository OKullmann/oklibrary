# Oliver Kullmann, 30.8.2008 (Swansea)
# Copyright 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

ecl_directories_okl := $(ecl_base_installation_dir_okl) $(ecl_base_build_dir_okl) $(ecl_base_doc_dir_okl) $(ecl_doc_dir_okl)

$(ecl_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main ecl targets
# #################################

.PHONY : ecl ecl_core libffi cleanecl cleanallecl

ecl : libffi ecl_core rlwrap

ecl_core : $(ecl_directories_okl)
	$(call unarchive,$(ecl_source_okl),$(ecl_base_build_dir_okl))
	cd $(ecl_build_dir_okl); $(postcondition) \
	LDFLAGS="-Wl,-rpath=$(ecl_installation_dir_okl)/lib $(gmp_link_path_okl) $(libffi_link_path_okl)" ./configure --prefix=$(ecl_installation_dir_okl) --with-gmp-prefix=$(gmp_installation_dir_okl) CC=$(gcc_call_okl) CXX=$(gpp_call_okl) CPPFLAGS="$(libffi_include_option_okl)"; $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition)


# #################################
# Cleaning
# #################################

cleanecl : 
	-rm -rf $(ecl_base_build_dir_okl)

cleanallecl : cleanecl
	-rm -rf $(ecl_base_installation_dir_okl) $(ecl_base_doc_dir_okl)


# #################################
# Tool rlwrap
###################################

rlwrap_directories_okl := $(rlwrap_base_build_dir_okl) $(rlwrap_base_installation_dir_okl)

.PHONY : rlwrap cleanallrlwrap

$(rlwrap_directories_okl) : % : 
	mkdir -p $@

rlwrap : $(rlwrap_directories_okl)
	$(call unarchive,$(rlwrap_source_okl),$(rlwrap_base_build_dir_okl))
	cd $(rlwrap_build_dir_okl); $(postcondition) \
	./configure --prefix=$(rlwrap_installation_dir_okl); $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition)
	ln -s --force $(rlwrap_call_okl) $(public_bin_dir_okl)/rlwrap; $(postcondition)

cleanallrlwrap : 
	-rm -rf $(rlwrap_base_build_dir_okl) $(rlwrap_base_installation_dir_okl) 


# #################################
# Tool libffi
###################################

libffi_directories_okl := $(libffi_base_build_dir_okl) $(libffi_base_installation_dir_okl)

.PHONY : libffi cleanlibffi cleanalllibffi

$(libffi_directories_okl) : % : 
	mkdir -p $@

libffi : $(libffi_directories_okl)
	$(call unarchive,$(libffi_source_okl),$(libffi_base_build_dir_okl))
	cd $(libffi_build_dir_okl); $(postcondition) \
	./configure --prefix=$(libffi_installation_dir_okl) CC=$(gcc_call_okl) CXX=$(gpp_call_okl); $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition)

cleanlibffi :
	-rm -rf $(libffi_base_build_dir_okl)

cleanalllibffi : cleanlibffi
	-rm -rf $(libffi_base_installation_dir_okl) 
