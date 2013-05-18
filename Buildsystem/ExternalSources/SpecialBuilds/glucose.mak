# Matthew Gwynne, 13.1.2011 (Swansea)
# Copyright 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

glucose_directories_okl := $(glucose_base_installation_dir_okl) $(glucose_base_build_dir_okl) $(glucose_installation_dir_okl)

$(glucose_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Glucose targets
# #################################

.PHONY : glucose cleanglucose cleanallglucose

glucose : $(glucose_directories_okl)
	$(call unarchive,$(glucose_source_okl),$(glucose_base_build_dir_okl)) $(postcondition) \
	cd $(glucose_build_dir_okl); $(postcondition) \
	cd simp; $(postcondition) \
	make rs CXX=$(gpp_call_okl) LFLAGS="$(gcc_linking_okl) $(zlib_link_option_okl) -lz"; $(postcondition) \
	cp glucose_static $(glucose_call_okl); $(postcondition) \
	ln -s --force $(glucose_call_okl) $(public_bin_dir_okl)/$(glucose_recommended_okl); $(postcondition)
# REMARK: appending the link-option to CC and CXX is ugly, but the Glucose
# build doesn't use LFLAGS correctly.
# And parts of the build don't use CC and CXX, and thus the aliasing.


# #################################
# Cleaning
# #################################

cleanglucose : 
	-rm -rf $(glucose_base_build_dir_okl)

cleanallglucose : cleanglucose
	-rm -rf $(glucose_base_installation_dir_okl)
