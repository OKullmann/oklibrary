# Oliver Kullmann, 20.5.2009 (Swansea)
# Copyright 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

bzip2_directories_okl := $(bzip2_base_installation_dir_okl) $(bzip2_base_build_dir_okl) $(bzip2_base_doc_dir_okl) $(bzip2_doc_dir_okl)

$(bzip2_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main bzip2 targets
# #################################

.PHONY : bzip2 cleanbzip2 cleanallbzip2

bzip2 : $(bzip2_directories_okl)
	$(call unarchive,$(bzip2_source_package_okl),$(bzip2_base_build_dir_okl)) $(postcondition)
	cp $(bzip2_source_dir_okl)/Makefile_bzip2-$(bzip2_recommended_version_number_okl)_corrected $(bzip2_extracted_package_okl)/Makefile
	cd $(bzip2_extracted_package_okl); $(postcondition) \
	make CC=$(gcc412_call_okl); $(postcondition) \
	make install PREFIX=$(bzip2_installation_dir_okl); $(postcondition) \
	cp manual.html manual.pdf $(bzip2_doc_dir_okl); $(postcondition)


# ####################################
# Cleaning
# ####################################

cleanbzip2 : 
	-rm -rf $(bzip2_base_build_dir_okl)

cleanallbzip2 : cleanbzip2
	-rm -rf $(bzip2_base_installation_dir_okl) $(bzip2_base_doc_dir_okl)

