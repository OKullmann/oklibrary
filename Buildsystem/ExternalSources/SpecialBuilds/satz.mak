# Oliver Kullmann, 10.4.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

satz215_directories_okl := $(satz_base_installation_dir_okl) $(satz_base_build_dir_okl) $(satz215_installation_dir_okl) $(satz215_build_dir_okl)
satz_directories_okl := $(satz_base_installation_dir_okl) $(satz_base_build_dir_okl) $(satz215_installation_dir_okl) $(satz215_build_dir_okl)

$(satz_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Satz targets
# #################################

.PHONY : satz satz215 cleansatz cleanallsatz

satz : satz215

satz215 : $(satz215_directories_okl)
	cp $(satz215_corrected_src_okl) $(satz215_build_dir_okl)
	cd $(satz215_build_dir_okl); $(postcondition) \
	gcc -O3 -o satz215 satz215.2.c; $(postcondition) \
	cp satz215 $(satz215_installation_dir_okl); $(postcondition) \
	ln -s --force $(satz215_call_okl) $(public_bin_dir_okl)/satz215; $(postcondition)

# #################################
# Cleaning
# #################################

cleansatz : 
	-rm -rf $(satz_base_build_dir_okl)

cleanallsatz : cleansatz
	-rm -rf $(satz_base_installation_dir_okl)
