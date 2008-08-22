# Matthew Henderson, 6.3.2006 (Swansea)
# Copyright 2006-2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# ##################################
# Directory Structure
# ##################################

mhash_base_installation_dir_okl := $(ExternalSources_installations)/Mhash
# normally ExternalSources/Mhash
abbr_mhash_targets := $(patsubst mhash-%, %, $(mhash_targets))
# this should be just mhash_supported_version_numbers ?!

mhash_extract_dirs := $(addprefix /,$(mhash_targets))
mhash_extract_dirs_paths := $(addprefix $(mhash_base_installation_dir_okl),$(mhash_extract_dirs))

# This line is necessary - but also belongs to makefile_gcc.mak
gcc_installation_directory_names := $(patsubst gcc-%, %, $(gcc_targets))
# and this should be just the supported version numbers for gcc ?!

mhash_installation_directory_names := $(foreach gccversion, $(gcc_installation_directory_names), $(addsuffix +$(gccversion), $(abbr_mhash_targets)))
# all combinations of supported mhash-versions with supported gcc-versions; this relates only to usage of local gcc's ?

mhash_installation_directory_names += $(abbr_mhash_targets)
# adding the "pure" mhash-versions to the combined mhash-gcc-versions
mhash_installation_directory_paths := $(addprefix $(mhash_base_installation_dir_okl)/,$(mhash_installation_directory_names))
# adding the full path-names

mhash_build_directory_names := $(addsuffix _Build, $(mhash_installation_directory_names))
mhash_build_directory_paths := $(addprefix $(mhash_base_installation_dir_okl)/,$(mhash_build_directory_names))
mhash_distribution_directories := $(addprefix $(mhash_base_installation_dir_okl)/mhash-, $(abbr_mhash_targets))

mhash_base_doc_dir_okl := $(ExternalSources_doc)/Mhash

mhash-directories := $(mhash_base_installation_dir_okl) $(mhash_build_directory_paths) $(mhash_installation_directory_paths) $(mhash_base_doc_dir_okl)

# ##################################
# Documentation building
# ##################################

mhash_doc : | $(mhash_base_doc_dir_okl)
	- $(call unarchive,$(ExternalSources)/sources/Mhash/$(mhash_recommended),$(mhash_base_doc_dir_okl))
# OK : This should automatically be done when building.

# ##################################
# Making mhash with the system gcc:
# ##################################

mhash_gcc_targets := $(foreach mhashversion, $(mhash_targets), $(addprefix $(mhashversion)+, $(gcc_installation_directory_names)))
# combined targets like mhash-0.9.7.1+4.0.1 --- this seems to be the wrong place here??
all_mhash_targets := $(mhash_targets) $(mhash_gcc_targets)
# adding the pure mhash-targets --- only they should be relevant here?!

.PHONY : mhash mhash_all mhash_gcc_all $(all_mhash_targets) cleanmhash cleanallmhash

$(mhash_installation_directory_paths) : % : | $(mhash_base_installation_dir_okl) %_Build 

$(mhash-directories) : % : 
	mkdir -p $@

define install-mhash
	cd $(mhash_base_installation_dir_okl)/$(1)_Build; $(postcondition) \
	$(mhash_base_installation_dir_okl)/mhash-$(1)/configure --prefix=$(mhash_base_installation_dir_okl)/$(1); $(postcondition) \
	cp $(mhash_base_installation_dir_okl)/mhash-$(1)/include/mutils/*.h $(mhash_base_installation_dir_okl)/$(1)_Build/include/mutils; $(postcondition) \
	make;	$(postcondition) \
	make install;
endef

$(mhash_extract_dirs_paths) : $(mhash_base_installation_dir_okl)/mhash-% : $(mhash_base_installation_dir_okl)/%
	$(call unarchive,$(ExternalSources)/sources/Mhash/mhash-$*,$(mhash_base_installation_dir_okl))
	$(call install-mhash,$*)
	touch $@

# ##################################
# Making mhash with a local gcc
# ##################################

define install-mhash_gcc
	cd $(mhash_base_installation_dir_okl)/$(1)+$(2)_Build;  if [ $$$$? != 0 ]; then exit 1; fi; \
	$(mhash_base_installation_dir_okl)/mhash-$(1)/configure --prefix=$(mhash_base_installation_dir_okl)/$(1)+$(2) --with-CC=$(gcc_base_installation_dir_okl)/$(gcc_version_okl)/bin/gcc; if [ $$$$? != 0 ]; then exit 1; fi; \
	cp $(mhash_base_installation_dir_okl)/mhash-$(1)/include/mutils/*.h $(mhash_base_installation_dir_okl)/$(1)+$(2)_Build/include/mutils; if [ $$$$? != 0 ]; then exit 1; fi; \
	make; if [ $$$$? != 0 ]; then exit 1; fi; \
	make install;
endef

define mhash_gcc_rule
$(mhash_base_installation_dir_okl)/mhash-$(1)+$(2) : $(mhash_base_installation_dir_okl)/$(1)+$(2)
	$(call unarchive,$(ExternalSources)/sources/Mhash/mhash-$(1),$(mhash_base_installation_dir_okl))
	$(call install-mhash_gcc,$(1),$(2))
endef

$(foreach mhashversion, $(abbr_mhash_targets), $(foreach gccversion, $(gcc_installation_directory_names), $(eval $(call mhash_gcc_rule,$(mhashversion),$(gccversion)))))

# ####################################
# The main targets for making mhash
# ####################################

mhash_gcc_all : $(all_mhash_targets)

$(all_mhash_targets) : % : $(mhash_base_installation_dir_okl)/%

ifeq ($(gcc_version_okl),all)
 mhash_all : $(mhash_gcc_targets)
 mhash : $(addprefix $(mhash_recommended)+,$(gcc_installation_directory_names))
else
 ifeq ($(gcc_version_okl),)
  mhash_all : $(mhash_targets)
  mhash : $(mhash_recommended)
 else
  mhash_all : $(addsuffix $(gcc_version_okl),$(mhash_targets))
  mhash : $(mhash_recommended)+$(gcc_version_okl)
 endif
endif

# ####################################
# Cleaning
# ####################################

cleanmhash : 
	-rm -rf $(mhash_build_directory_paths)


cleanallmhash : 
	-rm -rf $(mhash-directories)

