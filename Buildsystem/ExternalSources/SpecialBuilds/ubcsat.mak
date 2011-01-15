# Matthew Lewsey, 9.11.2006 (Swansea)
# Copyright 2006-2007, 2008, 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ##################################

# Old settings, to be removed once 1-0-0 is removed:
ubcsat_lib_directory := $(ubcsat_build_dir_okl)/lib
ubcsat_bin_directory := $(ubcsat_build_dir_okl)/bin
ubcsat_src_directory := $(ubcsat_build_dir_okl)/src
ubcsat_tmp_src_directory := $(ubcsat_build_dir_okl)/tmp

ubcsat_directories := $(ubcsat_base_build_dir_okl) $(ubcsat_lib_directory) $(ubcsat_bin_directory) $(ubcsat_src_directory) $(ubcsat_tmp_src_directory) $(ubcsat_installation_dir_okl)

$(ubcsat_directories) : % : 
	mkdir -p $@


# #################################
# The Targets
# #################################

.PHONY : ubcsat ubcsat-old old-ubcsat-core old-ubcsat-okl ubcsat-new ubcsat-beta ubcsat-okl-beta

ubcsat : ubcsat-old ubcsat-new

ubcsat-old :
	env -i PATH=${PATH} make -f $(OKbuildsystem)/ExternalSources/Makefile OKplatform=$(OKplatform) ubcsat_recommended_version_number_okl="1-0-0" ubcsat_build_dir_okl="$(ubcsat_base_build_dir_okl)/ubcsat-1-0-0" old-ubcsat-core old-ubcsat-okl

old-ubcsat-core : $(ubcsat_directories)
	$(call unarchive,$(ubcsat_source_okl),$(ubcsat_build_dir_okl),src)
	cd $(ubcsat_src_directory); $(postcondition) \
	chmod u+w *; $(postcondition) \
	dos2unix --quiet *
	cp $(ubcsat_src_directory)/* $(ubcsat_tmp_src_directory)
	cp -f $(ubcsat_corrected_files_okl) $(ubcsat_tmp_src_directory)
	$(gcc412_call_okl) -Wall -O3 -o $(ubcsat_bin_directory)/$(ubcsat_recommended_okl) -DNDEBUG $(ubcsat_tmp_src_directory)/*.c -lm
	cd $(ubcsat_tmp_src_directory); $(gcc412_call_okl) -Wall -O3 -c -DNDEBUG -DALTERNATEMAIN *.c
	cp $(ubcsat_tmp_src_directory)/*.o $(ubcsat_lib_directory)
	$(AR) $(ARFLAGS) $(ubcsat_lib_directory)/libubcsat.a $(ubcsat_lib_directory)/*.o
	cp -r $(ubcsat_lib_directory) $(ubcsat_installation_dir_okl)
	cp -r $(ubcsat_bin_directory) $(ubcsat_installation_dir_okl)
	cp -r $(ubcsat_src_directory) $(ubcsat_installation_dir_okl)

old-ubcsat-okl :
	$(preprocessing_call) $(old_ubcsat_wrapper_okl) > $(public_bin_dir_okl)/old-ubcsat-okl
	chmod u+x $(public_bin_dir_okl)/old-ubcsat-okl


ubcsat-new : ubcsat-beta ubcsat-okl-beta

ubcsat-beta : $(ubcsat_directories)
	$(call unarchive,$(ubcsat_source_okl),$(ubcsat_base_build_dir_okl))
	cd $(ubcsat_build_dir_okl); $(postcondition) \
	make CC=$(gcc_call_okl) CXX=$(gpp_call_okl) ubcsat ubcsat_cpp ubcsat_debug; $(postcondition) \
	mv ubcsat bin/$(ubcsat_recommended_okl); $(postcondition) \
	mv ubcsat_cpp ubcsat_debug bin; $(postcondition)
	cp -r $(ubcsat_bin_directory) $(ubcsat_installation_dir_okl)
	cp -r $(ubcsat_src_directory) $(ubcsat_installation_dir_okl)

ubcsat-okl-beta :
	$(preprocessing_call) $(ubcsat_wrapper_okl) > $(public_bin_dir_okl)/$(script_name_ubcsat_okl)
	chmod u+x $(public_bin_dir_okl)/$(script_name_ubcsat_okl)



# #################################
# Cleaning
# #################################

cleanubcsat :
	-rm -rf $(ubcsat_base_build_dir_okl)

cleanallubcsat : cleanubcsat
	-rm -rf $(ubcsat_base_installation_dir_okl)
