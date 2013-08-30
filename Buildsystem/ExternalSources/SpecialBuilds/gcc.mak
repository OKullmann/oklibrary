# Matthew Henderson, 18.7.2006 (Paderborn)
# Copyright 2006-2007, 2008, 2010, 2011, 2012, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# ##################################
# Directory Structure
# ##################################

gcc_directories_okl := $(gcc_base_installation_dir_okl) $(gcc_base_build_dir_okl) $(gcc_base_doc_dir_okl) $(gcc_doc_dir_okl) $(gcc_build_dir_okl) $(gcc412_build_dir_okl) $(gcc412_doc_dir_okl)

$(gcc_directories_okl) : % :
	mkdir -p $@


# ####################################
# The main targets for making gcc
# ####################################

.PHONY : gcc gcc_core gcc_doc gcc412 cleangcc cleanallgcc

gcc412 : $(gcc_base_installation_dir_okl) $(gcc412_build_dir_okl) $(gcc412_doc_dir_okl)
	$(call unarchive,$(gcc412_source_okl),$(gcc_base_build_dir_okl))
	cp -rf $(gcc412_corrected_dir_okl)/* $(gcc412_unarchived_source_okl)/.
	cd $(gcc412_unarchived_source_okl)/; for P in $$(ls $(gcc412_patched_dir_okl)/*.patch); do patch < $${P}; done
	cp $(gcc_source_base_okl)/copying-lib.texi $(gcc412_unarchived_source_okl)/libiberty
	cd $(gcc412_build_dir_okl); $(postcondition) \
	$(gcc412_unarchived_source_okl)/configure --prefix=$(gcc412_installation_dir_okl) --enable-languages=$(gcc412_enable_languages_okl) --enable-threads=$(gcc412_threads_okl) $(gcc412_other_options_okl); $(postcondition) \
	make; $(postcondition) \
	make html; $(postcondition) \
	make install; $(postcondition) \
	cp -fr gcc/doc $(gcc412_doc_dir_okl); $(postcondition) \
	rm -rf $(gcc412_doc_dir_okl)/html; $(postcondition) \
	cp -r gcc/HTML/gcc-4.1.2 $(gcc412_doc_dir_okl)/html; $(postcondition)


gcc : gcc_core gcc_doc

gcc_core : $(gcc_directories_okl)
	$(call unarchive,$(gcc_source_okl),$(gcc_base_build_dir_okl))
	$(call unarchive,$(gmpgcc_source_okl),$(gcc_unarchived_source_okl))
	mv $(gcc_unarchived_source_okl)/$(gmpgcc_name_okl) $(gcc_unarchived_source_okl)/gmp
	$(call unarchive,$(mpfrgcc_source_okl),$(gcc_unarchived_source_okl))
	mv $(gcc_unarchived_source_okl)/$(mpfrgcc_name_okl) $(gcc_unarchived_source_okl)/mpfr
	$(call unarchive,$(mpcgcc_source_okl),$(gcc_unarchived_source_okl))
	mv $(gcc_unarchived_source_okl)/$(mpcgcc_name_okl) $(gcc_unarchived_source_okl)/mpc
	cp $(gcc_source_base_okl)/copying-lib.texi $(gcc_unarchived_source_okl)/libiberty
	cp $(gcc_source_base_okl)/gpl.texi $(gcc_unarchived_source_okl)/gcc/doc/include
	cd $(gcc_build_dir_okl); $(postcondition) \
	$(gcc_unarchived_source_okl)/configure --prefix=$(gcc_installation_dir_okl) --enable-languages=$(gcc_enable_languages_okl) --enable-threads=$(gcc_threads_okl) --with-system-zlib $(gcc_other_options_okl) $(gcc_user_options_okl); $(postcondition) \
	make; $(postcondition) \
	make html dvi; $(postcondition) \
	make install; $(postcondition)

gcc_doc :
	cd $(gcc_build_dir_okl); $(postcondition) \
	cp -fr gcc/HTML $(gcc_doc_dir_okl); $(postcondition) \
	mv -f $(gcc_doc_dir_okl)/$(gcc_name_okl) $(gcc_doc_dir_okl)/html; $(postcondition) \
	cp -fr gcc/doc $(gcc_doc_dir_okl); $(postcondition)

# Remark: Correction of copying-lib.texi, gpl.texi should be removed for
# target gcc, once it has been done in the package itself.
# Also target "pdf" needs to be re-added to "make html dvi" when working again.


# ####################################
# Cleaning
# ####################################

cleangcc :
	-rm -rf $(gcc_base_build_dir_okl)

cleanallgcc : cleangcc
	-rm -rf $(gcc_base_installation_dir_okl) $(gcc_base_doc_dir_okl)
