# Oliver Kullmann, 6.5.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

git_directories := $(git_base_directory) $(git_documentation_dir)

# #################################
# Main Git targets
# #################################

.PHONY : git $(git_targets) create_git_dirs

$(git_directories) : % : 
	mkdir -p $@

create_git_dirs : $(git_directories)

git : $(git_recommended)

$(git_targets) : create_git_dirs
	$(call unarchive,sources/Git/$@,$(git_base_directory)) $(postcondition) \
	cd $(git_base_directory)/$@; $(postcondition) \
	make configure; $(postcondition) \
	sh ./configure --prefix=/usr/local; $(postcondition) \
	make all doc; $(postcondition) \
	cp -r $(git_base_directory)/$@/Documentation $(git_documentation_dir); $(postcondition) \
	sudo make install install-doc; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallgit : 
	-rm -rf $(git_base_directory)
