# Oliver Kullmann, 7.5.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

asciidoc-base-directory := $(prefix)/Asciidoc
asciidoc-directories := $(asciidoc-base-directory)

.PHONY : asciidoc $(asciidoc_targets) create_asciidoc_dirs

# #################################
# Main Asciidoc targets
# #################################

$(asciidoc-directories) : % : 
	mkdir -p $@

create_asciidoc_dirs : $(asciidoc-directories)

asciidoc : $(asciidoc_recommended)

$(asciidoc_targets) : create_asciidoc_dirs
	$(call unarchive,sources/Asciidoc/$@,$(asciidoc-base-directory))
	cd $(asciidoc-base-directory)/$@; $(postcondition) \
	sudo ./install.sh; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallasciidoc : 
	-rm -rf $(asciidoc-base-directory)
