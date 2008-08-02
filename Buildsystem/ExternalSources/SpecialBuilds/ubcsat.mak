# Matthew Lewsey, 9.11.2006 (Swansea)
# Copyright 2006-2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ##################################

ubcsat-lib-directory := $(ubcsat_build_dir_okl)/lib
ubcsat-bin-directory := $(ubcsat_build_dir_okl)/bin
ubcsat-src-directory := $(ubcsat_build_dir_okl)/src
ubcsat-tmp-src-directory := $(ubcsat_build_dir_okl)/tmp

ubcsat-directories := $(ubcsat_base_build_dir_okl) $(ubcsat_build_dir_okl) $(ubcsat-lib-directory) $(ubcsat-bin-directory) $(ubcsat-src-directory) $(ubcsat-tmp-src-directory) $(ubcsat_installation_dir_okl)

$(ubcsat-directories) : % : 
	mkdir -p $@


# #################################
# The Targets
# #################################

.PHONY : ubcsat ubcsat-okl

ubcsat : $(ubcsat-directories) ubcsat-okl
	$(call unarchive,$(ubcsat_source_okl),$(ubcsat_build_dir_okl),src)
	dos2unix $(ubcsat-src-directory)/*.c $(ubcsat-src-directory)/*.h
	cp $(ubcsat-src-directory)/* $(ubcsat-tmp-src-directory)
	cp -f $(ubcsat_corrected_files_okl) $(ubcsat-tmp-src-directory)
	gcc -Wall -O3 -o $(ubcsat-bin-directory)/ubcsat -DNDEBUG $(ubcsat-tmp-src-directory)/*.c -lm
	cd $(ubcsat-tmp-src-directory); gcc -Wall -O3 -c -DNDEBUG -DALTERNATEMAIN *.c
	cp $(ubcsat-tmp-src-directory)/*.o $(ubcsat-lib-directory)
	$(AR) $(ARFLAGS) $(ubcsat-lib-directory)/libubcsat.a $(ubcsat-lib-directory)/*.o
	cp -r $(ubcsat-lib-directory) $(ubcsat_installation_dir_okl)
	cp -r $(ubcsat-bin-directory) $(ubcsat_installation_dir_okl)
	cp -r $(ubcsat-src-directory) $(ubcsat_installation_dir_okl)
	ln -s --force $(ubcsat_call_okl) $(public_bin_dir_okl)/ubcsat

ubcsat-okl :
	$(preprocessing_call) $(OKbuildsystem)/ExternalSources/SpecialBuilds/Wrappers/ubcsat-okl > $(public_bin_dir_okl)/ubcsat-okl
	chmod u+x $(public_bin_dir_okl)/ubcsat-okl


# #################################
# Cleaning
# #################################

cleanallubcsat :
	-rm -rf $(ubcsat_base_build_dir_okl) $(ubcsat_base_installation_dir_okl)
