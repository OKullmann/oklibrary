# Matthew Henderson, 19.7.2006 (Paderborn)
# Copyright 2006-2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# ##################################
# Directory Structure
# ##################################

# In the following text, ? denotes the GCC version number and % denotes the Boost version number, while & denotes the minor version number.
#./Boost : Contain locally stage built, various versions of Boost C++ Libraries.
#./Boost/boost_%_& : This is the original unarchived source directory for boost version %.
#./Boost/bjam : Contain the bjam tool. This binary is re-generated each time when boost is to be installed.
#./Boost/% : Locally installed boost using system-wide version of GCC.
#./Boost/%+?_Build : This is the staging directory where configuration and temporary files are stored. This Boost version % is build using GCC version ?. 
#./Boost/%+? : The built Boost Libraries using GCC version ?. Libraries in this directory are then copied to the GCC ? lib directory.

abbr_boost_targets := $(patsubst boost-%, %, $(boost_targets))
# creates e.g. 1_32_0 1_33_1

# These two lines are necessary - but also belong to makefile_gcc.mak
gcc_installation_directory_names := $(patsubst gcc-%, %, $(gcc_targets))
gcc-base-directory := $(ExternalSources)/Gcc

boost_installation_directory_names := $(foreach gccversion, $(gcc_installation_directory_names), $(addsuffix +$(gccversion), $(abbr_boost_targets)))
# creates e.g. 1_32_0+3.4.3 1_32_0+3.4.4 1_33_1+3.4.3 1_33_1+3.4.4
boost_installation_directory_names += $(abbr_boost_targets)
boost_installation_directory_paths := $(addprefix $(boost_base_directory)/,$(boost_installation_directory_names))

boost_build_directory_names := $(addsuffix _Build, $(boost_installation_directory_names))
boost_build_directory_paths := $(addprefix $(boost_base_directory)/,$(boost_build_directory_names))

bjam_directory_path := $(boost_base_directory)/bjam
# The relative path to the bjam-source:
bjam_source := tools/jam/src

boost_base_doc_dir_okl := $(ExternalSources_doc)/Boost

boost-directories := $(boost_base_directory) $(boost_build_directory_paths) $(boost_installation_directory_paths) $(bjam_directory_path) $(boost_base_doc_dir_okl) $(addprefix $(boost_base_doc_dir_okl)/, $(abbr_boost_targets))

boost_distribution_directories := $(addprefix $(boost_base_directory)/boost_, $(abbr_boost_targets))

boost_gcc_targets := $(foreach boostversion, $(boost_targets), $(addprefix $(boostversion)+, $(gcc_installation_directory_names)))
all_boost_targets := $(boost_targets) $(boost_gcc_targets)

# ####################################
# Documentation
# ####################################

# files and directories containing the documentation:
boost_documentation := boost.png \
                       boost.css \
                       index.htm \
                       more \
                       libs \
                       doc \
                       tools

# ###############################
# General targets
# ###############################

.PHONY : boost boost_all boost_gcc_all $(all_boost_targets)

$(boost_installation_directory_paths) : % : | $(boost_base_directory) %_Build $(bjam_directory_path)

$(boost-directories) : % : 
	mkdir -p $@

# ###############################
# Making boost with the system gcc
# ###############################

define install-boost
	$(bjam_directory_path)/bjam --toolset=gcc --prefix=$(boost_base_directory)/$(1) --build-dir=$(boost_base_directory)/$(1)_Build install --without-python
endef

$(addprefix $(boost_base_directory)/, $(boost_targets)) : $(boost_base_directory)/boost-% : $(boost_base_directory)/% $(boost_base_doc_dir_okl)/%
	$(call unarchive,$(ExternalSources)/sources/Boost/boost_$*,$(boost_base_directory)) $(postcondition) \
	cd $(boost_base_directory)/boost_$*; $(postcondition) \
	cd $(bjam_source); $(postcondition) \
	./build.sh; $(postcondition) \
	cp bin.*/bjam $(bjam_directory_path); $(postcondition) \
	cd $(boost_base_directory)/boost_$*; $(postcondition) \
	$(call install-boost,$*); \
	mln -s "$(boost_base_directory)/$*/lib/*gcc[0-9][0-9]*" "$(boost_base_directory)/$*/lib/#1gcc#4"; $(postcondition) \
	cp -r $(boost_documentation) $(boost_base_doc_dir_okl)/$*; $(postcondition) \
	touch $@; $(postcondition)

# Comments:
# 0) Failing boost-build does not stop the process (since a partial build is still useful).
# 1) The mln provides the usable links.
# 2) The new documentation replaces old one (if existent).

# ###############################
# Making boost with a local gcc
# ###############################

define install-boost_gcc
	$(bjam_directory_path)/bjam --toolset=gcc-$(2) --toolset-root=$(gcc-base-directory)/$(2) --prefix=$(boost_base_directory)/$(1)+$(2) --build-dir=$(boost_base_directory)/$(1)+$(2)_Build "-sGCC_ROOT_DIRECTORY=$(gcc-base-directory)/$(2)" install --without-python
endef

define boost_gcc_rule
$(boost_base_directory)/boost-$(1)+$(2) : $(boost_base_directory)/$(1)+$(2) $(boost_base_doc_dir_okl)/$(1)
	$(call unarchive,$(ExternalSources)/sources/Boost/boost_$(1),$(boost_base_directory)) if [ $$$$? != 0 ]; then exit 1; fi; \
	cd $(boost_base_directory)/boost_$(1); if [ $$$$? != 0 ]; then exit 1; fi; \
	cd $(bjam_source); if [ $$$$? != 0 ]; then exit 1; fi; \
	./build.sh; if [ $$$$? != 0 ]; then exit 1; fi; \
	cp bin.*/bjam $(bjam_directory_path); if [ $$$$? != 0 ]; then exit 1; fi; \
	cd $(boost_base_directory)/boost_$(1); if [ $$$$? != 0 ]; then exit 1; fi; \
	$(call install-boost_gcc,$(1),$(2)); \
	mln -s "$(boost_base_directory)/$(1)+$(2)/lib/*gcc[0-9][0-9]*" "$(boost_base_directory)/$(1)+$(2)/lib/#1gcc#4"; if [ $$$$? != 0 ]; then exit 1; fi; \
	cp -r $(boost_documentation) $(boost_base_doc_dir_okl)/$(1); if [ $$$$? != 0 ]; then exit 1; fi; \
	touch $(boost_base_directory)/boost-$(1)+$(2); if [ $$$$? != 0 ]; then exit 1; fi;
endef

# Comments:
# This is the same as above (for system-gcc), but now using the local compiler (and thus also using 
# different naming conventions for the directories built).

$(foreach boostversion, $(abbr_boost_targets), $(foreach gccversion, $(gcc_installation_directory_names), $(eval $(call boost_gcc_rule,$(boostversion),$(gccversion)))))


# ###############################
# The main targets for making boost
# ###############################

boost_gcc_all : $(all_boost_targets)

$(all_boost_targets) : % : $(boost_base_directory)/%

ifeq ($(gcc_version_okl),all)
 boost_all : $(boost_gcc_targets)
 boost : $(addprefix $(boost_recommended)+,$(gcc_installation_directory_names))
else
 ifeq ($(gcc_version_okl),)
  boost_all : $(boost_targets)
  boost : $(boost_recommended)
 else
  boost_all : $(addsuffix $(gcc_version_okl),$(boost_targets))
  boost : $(boost_recommended)+$(gcc_version_okl)
 endif
endif

# ####################################
# Cleaning
# ####################################

cleanboost :
	-rm -rf $(boost_build_directory_paths) $(boost_distribution_directories) $(bjam_directory_path)

cleanallboost : 
	-rm -rf $(boost_base_directory)
