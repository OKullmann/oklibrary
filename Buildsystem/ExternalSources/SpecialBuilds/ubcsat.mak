# Matthew Lewsey, 9.11.2006 (Swansea)
# Copyright 2006-2007, 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ##################################

ubcsat_lib_directory := $(ubcsat_build_dir_okl)/lib
ubcsat_bin_directory := $(ubcsat_build_dir_okl)/bin
ubcsat_src_directory := $(ubcsat_build_dir_okl)/src
ubcsat_tmp_src_directory := $(ubcsat_build_dir_okl)/tmp

ubcsat_directories := $(ubcsat_base_build_dir_okl) $(ubcsat_build_dir_okl) $(ubcsat_lib_directory) $(ubcsat_bin_directory) $(ubcsat_src_directory) $(ubcsat_tmp_src_directory) $(ubcsat_installation_dir_okl)

$(ubcsat_directories) : % : 
	mkdir -p $@


# #################################
# The Targets
# #################################

.PHONY : ubcsat ubcsat-okl

ubcsat : $(ubcsat_directories) ubcsat-okl
	$(call unarchive,$(ubcsat_source_okl),$(ubcsat_build_dir_okl),src)
	cd $(ubcsat_src_directory); $(postcondition) \
	chmod u+w *; $(postcondition) \
	dos2unix --quiet *
	cp $(ubcsat_src_directory)/* $(ubcsat_tmp_src_directory)
	cp -f $(ubcsat_corrected_files_okl) $(ubcsat_tmp_src_directory)
	gcc -Wall -O3 -o $(ubcsat_bin_directory)/ubcsat -DNDEBUG $(ubcsat_tmp_src_directory)/*.c -lm
	cd $(ubcsat_tmp_src_directory); gcc -Wall -O3 -c -DNDEBUG -DALTERNATEMAIN *.c
	cp $(ubcsat_tmp_src_directory)/*.o $(ubcsat_lib_directory)
	$(AR) $(ARFLAGS) $(ubcsat_lib_directory)/libubcsat.a $(ubcsat_lib_directory)/*.o
	cp -r $(ubcsat_lib_directory) $(ubcsat_installation_dir_okl)
	cp -r $(ubcsat_bin_directory) $(ubcsat_installation_dir_okl)
	cp -r $(ubcsat_src_directory) $(ubcsat_installation_dir_okl)
	ln -s --force $(ubcsat_call_okl) $(public_bin_dir_okl)/ubcsat

ubcsat-okl :
	$(preprocessing_call) $(ubcsat_wrapper_okl) > $(public_bin_dir_okl)/ubcsat-okl
	chmod u+x $(public_bin_dir_okl)/ubcsat-okl


# #################################
# Cleaning
# #################################

cleanubcsat :
	-rm -rf $(ubcsat_base_build_dir_okl)

cleanallubcsat : cleanubcsat
	-rm -rf $(ubcsat_base_installation_dir_okl)
