# Matthew Gwynne, 7.1.2011 (Swansea)
# Copyright 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

cryptominisat_directories_okl := $(cryptominisat_base_installation_dir_okl) $(cryptominisat_installation_dir_okl) $(cryptominisat_base_build_dir_okl) $(cryptominisat_base_doc_dir_okl) $(cryptominisat_doc_dir_okl)

$(cryptominisat_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main cryptominisat targets
# #################################

.PHONY : allcryptominisat cryptominisat cryptominisat_doc cleancryptominisat2 cleanallcryptominisat2 cleancryptominisat cleanallcryptominisat

allcryptominisat : cryptominisat cryptominisat3


cryptominisat : $(cryptominisat_directories_okl)
	$(call unarchive,$(cryptominisat_source_okl),$(cryptominisat_base_build_dir_okl)) $(postcondition) \
	cd $(cryptominisat_build_dir_okl); $(postcondition) \
	CC="$(gcc_call_okl)" CXX="$(gpp_call_okl)" LDFLAGS="$(gcc_linking_okl)" ./configure; $(postcondition) \
	make all; $(postcondition) \
	cp -f $(cryptominisat_exec_okl) $(cryptominisat_call_okl); $(postcondition) \
	ln -s --force $(cryptominisat_call_okl) $(public_bin_dir_okl)/$(cryptominisat_public_call_okl); $(postcondition)


cryptominisat_doc :

# #################################
# Cleaning
# #################################

cleancryptominisat2 :
	-rm -rf $(cryptominisat_build_dir_okl)

cleanallcryptominisat2 : cleancryptominisat2
	-rm -rf $(cryptominisat_installation_dir_okl) $(cryptominisat_doc_dir_okl)

# Cleaning *all* cryptominisat versions
cleancryptominisat : cleancryptominisat2 cleancryptominisat3
	-rm -rf $(cryptominisat_base_build_dir_okl)

cleanallcryptominisat : cleanallcryptominisat2 cleanallcryptominisat3
	-rm -rf $(cryptominisat_base_installation_dir_okl) $(cryptominisat_base_doc_dir_okl)


# #################################
# Main cryptominisat3 targets
# #################################

cryptominisat3_directories_okl :=  $(cryptominisat3_installation_dir_okl) $(cryptominisat3_doc_dir_okl)

$(cryptominisat3_directories_okl) : % : $(cryptominisat_base_build_dir_okl) $(cryptominisat_base_installation_dir_okl) $(cryptominisat_base_build_dir_okl) $(cryptominisat_base_doc_dir_okl)
	mkdir -p $@

.PHONY : cryptominisat3 cryptominisat3_doc cleancryptominisat3 cleanallcryptominisat3


cryptominisat3 : $(cryptominisat3_directories_okl) m4ri
	$(call unarchive,$(cryptominisat3_source_okl),$(cryptominisat_base_build_dir_okl)) $(postcondition) \
	cd $(cryptominisat3_build_dir_okl); $(postcondition) \
	mkdir -p build; $(postcondition) \
	cd build; $(postcondition) \
	CC="$(gcc_call_okl) $(m4ri_include_option_okl)" CXX="$(gpp_call_okl) $(m4ri_include_option_okl)" LDFLAGS="$(gcc_linking_okl)" CMAKE_LIBRARY_PATH="$(boost_link_library_okl):$(zlib_link_library_okl):$(m4ri_link_library_okl)" CMAKE_INCLUDE_PATH="$(boost_source_library_okl):$(zlib_source_library_okl):$(m4ri_source_library_okl):$(CMAKE_INCLUDE_PATH)" M4RI_HOME="$(m4ri_installation_dir_okl)" cmake ../; $(postcondition) \
	make; $(postcondition) \
	cp -f cryptominisat $(cryptominisat3_call_okl); $(postcondition) \
	ln -s --force $(cryptominisat3_call_okl) $(public_bin_dir_okl)/$(cryptominisat3_public_call_okl); $(postcondition)


cryptominisat3_doc :


cleancryptominisat3 : cleanm4ri
	-rm -rf $(cryptominisat3_build_dir_okl)

cleanallcryptominisat3 : cleancryptominisat3 cleanallm4ri
	-rm -rf $(cryptominisat3_installation_dir_okl) $(cryptominisat3_doc_dir_okl)


# #################################
# M4RI tool targets
# #################################

m4ri_directories_okl := $(m4ri_build_dir_okl) $(m4ri_installation_dir_okl)

.PHONY : m4ri m4ri_doc cleanm4ri cleanallm4ri

$(m4ri_directories_okl) : % : $(cryptominisat_base_build_dir_okl) $(cryptominisat_base_installation_dir_okl)
	mkdir -p $@


m4ri : $(m4ri_directories_okl)
	$(call unarchive,$(m4ri_source_okl),$(cryptominisat_base_build_dir_okl)) $(postcondition) \
	cd $(m4ri_build_dir_okl); $(postcondition) \
	CC="$(gcc_call_okl)" CXX="$(gpp_call_okl)" LDFLAGS="$(gcc_linking_okl)" ./configure --prefix="$(m4ri_installation_dir_okl)"; $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition)


m4ri_doc :


cleanm4ri:
	-rm -rf $(m4ri_build_dir_okl)

cleanallm4ri: cleanm4ri
	-rm -rf $(m4ri_installation_dir_okl) $(m4ri_doc_dir_okl)
