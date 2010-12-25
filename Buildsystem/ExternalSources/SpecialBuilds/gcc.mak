# Matthew Henderson, 18.7.2006 (Paderborn)
# Copyright 2006-2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ##################################

gcc_directories_okl := $(gcc_base_installation_dir_okl) $(gcc_base_build_dir_okl) $(gcc_base_doc_dir_okl) $(gcc_doc_dir_okl) $(gcc_build_dir_okl)

$(gcc_directories_okl) : % : 
	mkdir -p $@


# ####################################
# The main targets for making gcc
# ####################################

.PHONY : gcc cleangcc cleanallgcc

ifeq ($(gcc_recommended_version_number_okl),4.1.2)

gcc : $(gcc_directories_okl)
	$(call unarchive,$(gcc_source_okl),$(gcc_base_build_dir_okl))
	cat $(ExternalSources)/sources/Gcc/configure-4.1.2.gz | gunzip > $(gcc_unarchived_source_okl)/configure
	cd $(gcc_build_dir_okl); $(postcondition) \
	$(gcc_unarchived_source_okl)/configure --prefix=$(gcc_installation_dir_okl) --enable-languages=$(gcc_enable_languages_okl) --enable-threads=$(gcc_threads_okl) $(gcc_other_options_okl) --with-gmp=$(gmp_locsys_install_directory_okl) --with-mpfr=$(mpfr_locsys_install_directory_okl); $(postcondition) \
	make; $(postcondition) \
	make html; $(postcondition) \
	make install; $(postcondition) \
	cp -fr gcc/doc $(gcc_doc_dir_okl); $(postcondition) \
	rm -rf $(gcc_doc_dir_okl)/html; $(postcondition) \
	cp -r gcc/HTML/$(gcc_recommended_okl) $(gcc_doc_dir_okl)/html; $(postcondition)

else

gcc : $(gcc_directories_okl)
	$(call unarchive,$(gcc_source_okl),$(gcc_base_build_dir_okl))
	cd $(gcc_build_dir_okl); $(postcondition) \
	$(gcc_unarchived_source_okl)/configure --prefix=$(gcc_installation_dir_okl) --enable-languages=$(gcc_enable_languages_okl) --enable-threads=$(gcc_threads_okl) $(gcc_other_options_okl) --with-gmp=$(gmp_locsys_install_directory_okl) --with-mpfr=$(mpfr_locsys_install_directory_okl); $(postcondition) \
	make; $(postcondition) \
	make html dvi pdf; $(postcondition) \
	make install install-html; $(postcondition) \
	cp -fr $(gcc_installation_dir_okl)/share/doc $(gcc_doc_dir_okl); $(postcondition) \
	mv -f $(gcc_doc_dir_okl)/doc $(gcc_doc_dir_okl)/html; $(postcondition) \
	cp -fr gcc/doc $(gcc_doc_dir_okl); $(postcondition)

endif

# ####################################
# Cleaning
# ####################################

cleangcc :
	-rm -rf $(gcc_base_build_dir_okl)

cleanallgcc : cleangcc
	-rm -rf $(gcc_base_installation_dir_okl) $(gcc_base_doc_dir_okl)
