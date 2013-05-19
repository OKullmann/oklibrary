# Oliver Kullmann, 10.4.2009 (Swansea)
# Copyright 2009, 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

satz_directories_okl := $(satz_base_installation_dir_okl) $(satz_base_build_dir_okl) $(satz_installation_dir_okl) $(satz_build_dir_okl)
satz_directories_okl := $(satz_base_installation_dir_okl) $(satz_base_build_dir_okl) $(satz_installation_dir_okl) $(satz_build_dir_okl)

$(satz_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Satz targets
# #################################

.PHONY : satzall satz satzm2pp cleansatz cleanallsatz

satzall : satz satzm2pp

satz : $(satz_directories_okl)
	cp $(satz_corrected_src_okl) $(satz_build_dir_okl)
	cd $(satz_build_dir_okl); $(postcondition) \
	gcc -Wall -Wno-parentheses -std=c99 -O3 -o satz $(satz_corrected_src_base_okl); $(postcondition) \
	cp satz $(satz_call_okl); $(postcondition) \
	ln -s --force $(satz_call_okl) $(public_bin_dir_okl)/$(satz_public_call_okl); $(postcondition)

satzm2pp :
	$(preprocessing_call) $(satz_m2pp_okl) > $(public_bin_dir_okl)/satz215-m2pp
	chmod u+x $(public_bin_dir_okl)/satz215-m2pp

# #################################
# Cleaning
# #################################

cleansatz : 
	-rm -rf $(satz_base_build_dir_okl)

cleanallsatz : cleansatz
	-rm -rf $(satz_base_installation_dir_okl)
	-rm $(public_bin_dir_okl)/$(satz_public_call_okl)
	-rm $(public_bin_dir_okl)/satz215-m2pp
