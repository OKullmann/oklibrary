# Oliver Kullmann, 24.12.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# This installation possibly has to be performed by hand, since apparently the
# configure-script of the clisp-installation cannot handle correctly
# the bash-shell.

# The clisp-configure-script apparently cannot handle correctly bash in the
# context of make.

# ##################################
# Directory Structure
# ################################## 

clisp_directories_okl := $(clisp_base_installation_dir_okl) $(clisp_base_build_dir_okl) $(clisp_base_doc_dir_okl)

.PHONY : clisp $(clisp_targets)

# #################################
# Main clisp targets
# #################################

$(clisp_directories_okl) : % : 
	mkdir -p $@

clisp : $(clisp_recommended_okl)

$(clisp_targets_okl) : $(clisp_directories_okl)
	$(call unarchive,$(ExternalSources)/sources/CLisp/$@,$(clisp_base_build_dir_okl))
	cd $(clisp_build_dir_okl); $(postcondition) \
	./configure --prefix=$(clisp_installation_dir_okl) --build $(clisp_build_dir_okl)/oklib-build; $(postcondition) \
	cd $(clisp_build_dir_okl)/oklib-build; $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallclisp : 
	-rm -rf $(clisp_base_installation_dir_okl) $(clisp_base_build_dir_okl) $(clisp_base_doc_dir_okl)

# #################################
# Tool libsigsegv
###################################

libsigsegv_directories_okl := $(libsigsegv_base_build_dir_okl)

.PHONY : libsigsegv $(libsigsegv_targets_okl)

$(libsigsegv_directories_okl) : % : 
	mkdir -p $@

libsigsegv : $(libsigsegv_recommended_okl)

$(libsigsegv_targets_okl) : $(libsigsegv_directories_okl)
	$(call unarchive,sources/CLisp/$@,$(libsigsegv_base_build_dir_okl))
	cd $(libsigsegv_build_dir_okl); $(postcondition) \
	./configure; $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	sudo make install; $(postcondition)

cleanalllibsigsegv : 
	-rm -rf $(libsigsegv_base_build_dir_okl)
