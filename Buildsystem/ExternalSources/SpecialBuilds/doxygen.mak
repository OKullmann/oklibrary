# Matthew Henderson, 19.7.2006 (Paderborn)
# Copyright 2006-2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ##################################

doxygen_directories_okl := $(doxygen_base_installation_dir_okl) $(doxygen_base_build_dir_okl) $(doxygen_base_doc_dir_okl) $(doxygen_doc_dir_okl)

$(doxygen_directories_okl) : % : 
	mkdir -p $@


# ##################################
# The main targets for doxygen
# ##################################

.PHONY : doxygen doxygen_base doxygen_links cleandoxygen cleanalldoxygen

doxygen : doxygen_base doxygen_links

doxygen_base : $(doxygen_directories_okl)
	$(call unarchive,$(doxygen_source_okl),$(doxygen_base_build_dir_okl)) $(postcondition) \
	cd $(doxygen_build_dir_okl); $(postcondition) \
	sh ./configure --prefix $(doxygen_install_directory_okl); $(postcondition) \
	make; $(postcondition) \
	make docs; $(postcondition) \
	$(doxygen_install_command_okl); $(postcondition) \
	cp -r $(doxygen_build_dir_okl)/html $(doxygen_doc_dir_okl)

ifeq ($(doxygen_default_install_okl),local)
doxygen_links :
	ln -s --force $(doxygen_call_okl) $(public_bin_dir_okl)/doxygen; $(postcondition) \
	ln -s --force $(doxytag_call_okl) $(public_bin_dir_okl)/doxytag
else
doxygen_links :
endif


# ####################################
# Cleaning
# ####################################

cleandoxygen : 
	-rm -rf $(doxygen_base_build_dir_okl)

cleanalldoxygen : cleandoxygen
	-rm -rf $(doxygen_base_installation_dir_okl) $(doxygen_base_doc_dir_okl)
