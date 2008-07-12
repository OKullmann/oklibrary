# Oliver Kullmann, 12.7.2008 (Swansea)
# Copyright 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

gap_directories_okl := $(gap_base_installation_dir_okl) $(gap_base_doc_dir_okl) $(gap_doc_dir_okl) $(gap_monoid_docdir_okl) $(gap_grape_docdir_okl)

.PHONY : gap cleanallgap

$(gap_directories_okl) : % : 
	mkdir -p $@


# #################################
# Main gap targets
# #################################

gap : $(gap_directories_okl)
	$(call unarchive,$(gap_source_okl),$(gap_base_installation_dir_okl))
	$(call unarchive,$(gap_packages_source_okl),$(gap_installation_dir_okl)/pkg)
	cp --force $(gap_instpackages_source_okl) $(gap_installation_dir_okl)/pkg
	cd $(gap_installation_dir_okl); $(postcondition) \
	./configure; $(postcondition) \
	make; $(postcondition) \
	cp --force $(gap_call_okl) $(public_bin_dir_okl)/gap; $(postcondition) \
	cd pkg; $(postcondition) \
	chmod u+x $(gap_instpackages_name_okl); $(postcondition) \
	./$(gap_instpackages_name_okl); $(postcondition) \
	cp -rf $(gap_installation_dir_okl)/doc/* $(gap_doc_dir_okl); $(postcondition) \
	cp -f $(gap_monoid_installdocdir_okl)/* $(gap_monoid_docdir_okl); $(postcondition) \
	cp -f $(gap_grape_installdocdir_okl)/* $(gap_grape_docdir_okl); $(postcondition)


# #################################
# Cleaning
# #################################

cleanallgap :
	-rm -rf $(gap_base_installation_dir_okl) $(gap_base_doc_dir_okl)

