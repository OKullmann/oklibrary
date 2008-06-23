# Oliver Kullmann, 7.5.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

asciidoc_directories_okl := $(asciidoc_base_build_dir_okl) $(asciidoc_base_doc_dir_okl) $(asciidoc_doc_dir_okl)

$(asciidoc_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Asciidoc targets
# #################################


.PHONY : asciidoc

asciidoc : $(asciidoc_directories_okl)
	$(call unarchive,$(asciidoc_source_okl),$(asciidoc_base_build_dir_okl)) $(postcondition) \
	cd $(asciidoc_build_dir_okl); $(postcondition) \
	sudo ./install.sh; $(postcondition)


# #################################
# Cleaning
# #################################

cleanallasciidoc : 
	-rm -rf $(asciidoc_base_build_dir_okl)
