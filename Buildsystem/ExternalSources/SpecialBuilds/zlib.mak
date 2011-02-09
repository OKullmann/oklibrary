# Oliver Kullmann, 5.6.2010 (Swansea)
# Copyright 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

zlib_directories_okl := $(zlib_base_installation_dir_okl) $(zlib_gccbuild_dir_okl)  $(zlib32_gccbuild_dir_okl)

$(zlib_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main zlib targets
# #################################

.PHONY : zlib zlib_core zlib32 cleanzlib cleanallzlib

zlib: zlib_core zlib32

zlib_core : $(zlib_directories_okl)
	$(call unarchive,$(zlib_source_package_okl),$(zlib_gccbuild_dir_okl))
	cd $(zlib_build_dir_okl); $(postcondition) \
	CC=$(gcc_call_okl) ./configure --prefix $(zlib_installation_dir_okl); $(postcondition) \
	make; $(postcondition) \
	make test; $(postcondition) \
	make install; $(postcondition)

ifneq ($(machine_bits_okl),32)
zlib32 : $(zlib_directories_okl)
	$(call unarchive,$(zlib_source_package_okl),$(zlib32_gccbuild_dir_okl))
	cd $(zlib32_build_dir_okl); $(postcondition) \
	CC=$(gcc_call_okl) CFLAGS="-m32" ./configure --prefix $(zlib32_installation_dir_okl); $(postcondition) \
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

