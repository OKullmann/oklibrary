# Oliver Kullmann, 24.12.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

maxima_directories_okl := $(maxima_base_installation_dir_okl) $(maxima_base_build_dir_okl) $(maxima_base_doc_dir_okl) $(maxima_doc_dir_okl)

.PHONY : maxima $(maxima_targets_okl)

# #################################
# Main maxima targets
# #################################

$(maxima_directories_okl) : % : 
	mkdir -p $@

maxima : $(maxima_recommended_okl)

$(maxima_targets_okl) : $(maxima_directories_okl)
	$(call unarchive,sources/Maxima/$@,$(maxima_base_build_dir_okl))
	cd $(maxima_build_dir_okl); $(postcondition) \
	LANG=C ./configure --prefix=${maxima_installation_dir_okl}; $(postcondition) \
	LANG=C make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition) \
	cp -r $(maxima_installation_dir_okl)/share/maxima/$(maxima_recommended_version_number_okl)/doc/* $(maxima_doc_dir_okl)

# #################################
# Cleaning
# #################################

cleanallmaxima : 
	-rm -rf $(maxima_base_installation_dir_okl) $(maxima_base_build_dir_okl) $(maxima_base_doc_dir_okl)
