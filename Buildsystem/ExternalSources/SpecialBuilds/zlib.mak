# Oliver Kullmann, 5.6.2010 (Swansea)
# Copyright 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

zlib_directories_okl := $(zlib_base_installation_dir_okl) $(zlib_build_dir_okl) $(zlib_base_doc_dir_okl) $(zlib_doc_dir_okl)
zlib_directories_32_okl := $(zlib_base_installation_dir_okl) $(zlib_build_dir_32_okl)

$(zlib_directories_okl) : % : 
	mkdir -p $@
$(zlib_directories_32_okl) : % :
	mkdir -p $@

# #################################
# Main zlib targets
# #################################

.PHONY : zlib cleanzlib cleanallzlib

zlib: zlib64 zlib32

zlib64 : $(zlib_directories_okl)
	$(call unarchive,$(zlib_source_package_okl),$(zlib_base_build_dir_okl)) $(postcondition)
	mv -fT $(zlib_extracted_package_okl) $(zlib_build_dir_okl); $(postcondition) \
	cd $(zlib_build_dir_okl); $(postcondition) \
	CC=$(gcc_call_okl) ./configure --prefix $(zlib_installation_dir_okl); $(postcondition) \
	make; $(postcondition) \
	make test; $(postcondition) \
	make install; $(postcondition)

ifneq ($(machine_bits_okl),32)
zlib32 : $(zlib_directories_32_okl)
	$(call unarchive,$(zlib_source_package_okl),$(zlib_base_build_dir_okl)) $(postcondition)
	mv -fT $(zlib_extracted_package_okl) $(zlib_build_dir_32_okl); $(postcondition) \
	cd $(zlib_build_dir_32_okl); $(postcondition) \
	CC=$(gcc_call_okl) CFLAGS="-m32" ./configure --prefix $(zlib_installation_dir_32_okl); $(postcondition) \
	make; $(postcondition) \
	make test; $(postcondition) \
	make install; $(postcondition)
else
zlib32 :
       : # No op
endif


# ####################################
# Cleaning
# ####################################

cleanzlib : 
	-rm -rf $(zlib_base_build_dir_okl)

cleanallzlib : cleanzlib
	-rm -rf $(zlib_base_installation_dir_okl)

