# Oliver Kullmann, 25.3.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

minisat_directories_okl := $(minisat_base_installation_dir_okl) $(minisat_base_build_dir_okl) $(minisat_base_doc_dir_okl) $(minisat_installation_dir_okl) 
minisat2_directories_okl := $(minisat_base_installation_dir_okl) $(minisat_base_build_dir_okl) $(minisat2_installation_dir_okl)
minisatp_directories_okl := $(minisat_base_installation_dir_okl) $(minisat_base_build_dir_okl) $(minisatp_installation_dir_okl)

minisat_all_directories_okl := $(minisat_base_installation_dir_okl) $(minisat_base_build_dir_okl) $(minisat_base_doc_dir_okl) $(minisat2_installation_dir_okl) $(minisatp_installation_dir_okl) $(minisat_installation_dir_okl) 

$(minisat_all_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Minisat targets
# #################################

.PHONY : minisat minisat2 minisat-new minisatp cleanminisat cleanallminisat

minisat : minisat2 minisat-new minisatp

minisat-new : $(minisat_directories_okl)
	$(call unarchive,$(minisat_source_okl),$(minisat_base_build_dir_okl))
	cd $(minisat_build_dir_okl); $(postcondition) \
	patch core/Solver.cc $(minisat_oklib_dir_okl)/Patch_output; $(postcondition) \
	patch core/Solver.cc $(minisat_oklib_dir_okl)/Patch_output2; $(postcondition) \
	export MROOT=$(minisat_build_dir_okl); $(postcondition) \
	cd simp; $(postcondition) \
	make rs CXX=$(gpp_call_okl) LFLAGS="$(gcc_linking_okl) $(zlib_link_option_okl) -lz"; $(postcondition) \
	cp minisat_static $(minisat_call_okl); $(postcondition) \
	ln -s --force $(minisat_call_okl) $(public_bin_dir_okl)/$(minisat_public_call_okl); $(postcondition)

minisat2 : $(minisat2_directories_okl)
	$(call unarchive,$(minisat2_source_okl),$(minisat_base_build_dir_okl)) $(postcondition) \
	cd $(minisat2_build_dir_okl)/simp; $(postcondition) \
	make rs CFLAGS='-I$$(MTL) -I$$(CORE) -Wall -ffloat-store'" -I$(zlib_source_library_okl)" LFLAGS="$(gcc_linking_okl) $(zlib_link_option_okl) -lz" CXX=$(gpp_call_okl); $(postcondition) \
	cp minisat_static $(minisat2_installation_dir_okl); $(postcondition) \
	ln -s --force $(minisat2_call_okl) $(public_bin_dir_okl)/minisat2; $(postcondition)

minisatp : $(minisatp_directories_okl)
	$(call unarchive,$(minisatp_source_okl),$(minisat_base_build_dir_okl)) $(postcondition) \
	cp $(minisatp_extended_makefile_okl) $(minisatp_build_dir_okl); $(postcondition) \
	cp $(minisatp_corrected_okl) $(minisatp_build_dir_okl)/ADTs; $(postcondition) \
	cd $(minisatp_build_dir_okl); $(postcondition) \
	make GMP_INCLUDE_OPTION="${gmp_include_option_okl}" GMP_LINK_OPTION="${gmp_link_option_okl}" rx ZLIB_INCLUDE_OPTION="-I$(zlib_source_library_okl)" LFLAGS="$(zlib_link_option_okl)" CXX=$(gpp_call_okl); $(postcondition) \
	make GMP_INCLUDE_OPTION="${gmp_include_option_okl}" GMP_LINK_OPTION="${gmp_link_option_okl}" rs ZLIB_INCLUDE_OPTION="-I$(zlib_source_library_okl)" LFLAGS="$(gcc_linking_okl) $(zlib_link_option_okl)" CXX=$(gpp_call_okl); $(postcondition) \
	cp minisat+_64-bit_static minisat+_bignum_static $(minisatp_installation_dir_okl); $(postcondition) \
	ln -s --force $(minisatp_call_okl) $(public_bin_dir_okl)/minisat+; $(postcondition) \
	ln -s --force $(minisatpb_call_okl) $(public_bin_dir_okl)/minisat+b; $(postcondition)


# #################################
# Cleaning
# #################################

cleanminisat : 
	-rm -rf $(minisat_base_build_dir_okl)

cleanallminisat : cleanminisat
	-rm -rf $(minisat_base_installation_dir_okl) $(minisat_base_doc_dir_okl)
