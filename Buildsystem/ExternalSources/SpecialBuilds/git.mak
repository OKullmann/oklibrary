# Oliver Kullmann, 6.5.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

git_directories_okl := $(git_base_installation_dir_okl) $(git_base_build_dir_okl) $(git_base_doc_dir_okl) $(git_doc_dir_okl)

$(git_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main Git targets
# #################################

.PHONY : git git_base git_links cleangit cleanallgit

git : git_base git_links

git_base : $(git_directories_okl)
	$(call unarchive,$(git_source_okl),$(git_base_build_dir_okl)) $(postcondition) \
	cd $(git_build_dir_okl); $(postcondition) \
	make configure; $(postcondition) \
	sh ./configure --prefix=$(git_install_directory_okl); $(postcondition) \
	make all doc; $(postcondition) \
	cp -r $(git_build_dir_okl)/Documentation $(git_doc_dir_okl); $(postcondition) \
	$(git_install_command_okl) install-doc; $(postcondition) \
	cp $(git_book_okl) $(git_base_doc_dir_okl); $(postcondition)

ifeq ($(git_default_install_okl),local)
git_links :
	ln -s --force $(git_call_okl) $(public_bin_dir_okl)/git; $(postcondition) \
	ln -s --force $(gitk_call_okl) $(public_bin_dir_okl)/gitk; $(postcondition) \
	ln -s --force $(git_upload_pack_okl) $(public_bin_dir_okl)/git-upload-pack; $(postcondition) \
	ln -s --force $(git_receive_pack_okl) $(public_bin_dir_okl)/git-receive-pack; $(postcondition)
else
git_links :
endif


# #################################
# Cleaning
# #################################

cleangit : 
	-rm -rf $(git_base_build_dir_okl)

cleanallgit : cleangit
	-rm -rf $(git_base_installation_dir_okl) $(git_base_doc_dir_okl)
