# Oliver Kullmann, 31.3.2009 (Swansea)
# Copyright 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

marchpl_directories_okl := $(march_base_installation_dir_okl) $(march_base_build_dir_okl) $(marchpl_installation_dir_okl)
march_directories_okl := $(march_base_installation_dir_okl) $(march_base_build_dir_okl) $(marchpl_installation_dir_okl)

$(march_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main March targets
# #################################

.PHONY : march marchpl cleanmarch cleanallmarch

march : marchpl

marchpl : $(marchpl_directories_okl)
	$(call unarchive,$(marchpl_source_okl),$(march_base_build_dir_okl)) $(postcondition) \
	cd $(marchpl_build_dir_okl); $(postcondition) \
	make CC=$(gcc412_call_okl); $(postcondition) \
	cp march_pl $(marchpl_installation_dir_okl); $(postcondition) \
	ln -s --force $(marchpl_call_okl) $(public_bin_dir_okl)/march_pl; $(postcondition)

# Remark: Once using a newer gcc (than 4.1.2), then
# cp $(marchpl_corrected_src_okl)/*.{c,h} $(marchpl_build_dir_okl) 
# needs to be used.

# #################################
# Cleaning
# #################################

cleanmarch : 
	-rm -rf $(march_base_build_dir_okl)

cleanallmarch : cleanmarch
	-rm -rf $(march_base_installation_dir_okl)
