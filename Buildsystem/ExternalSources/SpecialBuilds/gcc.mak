# Matthew Henderson, 18.7.2006 (Paderborn)
# Copyright 2006-2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ##################################

# ATTENTION: The following needs an UPDATE!

#In the following text, ? denotes the GCC version number.
#./Gcc : Contain locally installed various versions of GCC.
#./Gcc/gcc-? : This is the original unarchived source directory for gcc version ?.
#./GCC/gcc-?_Build : This is the staging directory where configuration and temporary files are stored.
#./Gcc/? : This is the local installation of gcc version ?. 

enable-languages := c,c++
gcc-version =
gcc-base-directory := $(ExternalSources)/Gcc
gcc_build_directory_names := $(addsuffix _Build, $(gcc_targets))
gcc_build_directory_paths := $(addprefix $(gcc-base-directory)/,$(gcc_build_directory_names))
gcc_installation_directory_names := $(patsubst gcc-%, %, $(gcc_targets))
gcc_installation_directory_paths := $(addprefix $(gcc-base-directory)/,$(gcc_installation_directory_names))
gcc_distribution_directories := $(addprefix $(gcc-base-directory)/,$(gcc_targets))
gcc_doc_dir := $(external_sources_doc_base_dir)/Gcc
gcc_doc_dir_directories := $(patsubst gcc-%, %, $(gcc_targets))
gcc_doc_dir_directories := $(addprefix $(gcc_doc_dir)/,$(gcc_doc_dir_directories))
gcc-directories := $(gcc-base-directory) $(gcc_installation_directory_paths) $(gcc_distribution_directories) $(gcc_doc_dir) $(gcc_doc_dir_directories)

.PHONY : gcc gcc_all $(gcc_targets)

# ####################################
# The main targets for making gcc
# ####################################

gcc_all : $(gcc_targets)

gcc : $(gcc_recommended)

$(gcc-directories) : % : 
	mkdir -p $@

gcc_timestamp_prefix := _
# Auxiliary variables (local):
gcc_tag_names:= $(addprefix $(gcc_timestamp_prefix),$(gcc_targets))
gcc_tag_names_old:= $(addprefix $(gcc_timestamp_prefix),$(gcc_targets_old))
gcc_tag_names_new:= $(addprefix $(gcc_timestamp_prefix),$(gcc_targets_new))
#
gcc_tag_paths := $(addprefix $(gcc-base-directory)/,$(gcc_tag_names))
gcc_tag_paths_old := $(addprefix $(gcc-base-directory)/,$(gcc_tag_names_old))
gcc_tag_paths_new := $(addprefix $(gcc-base-directory)/,$(gcc_tag_names_new))

$(gcc_tag_paths_old) : $(gcc-base-directory)/_gcc-%  : | $(gcc-base-directory) $(gcc-base-directory)/% $(gcc_doc_dir)/%
	$(call unarchive,$(ExternalSources)/sources/Gcc/gcc-$*,$(gcc-base-directory)) $(postcondition) \
	if [[ !(-d $(gcc-base-directory)/gcc-$*_Build) ]]; then mkdir $(gcc-base-directory)/gcc-$*_Build; fi; $(postcondition) \
	cd $(gcc-base-directory)/gcc-$*_Build; $(postcondition) \
	../gcc-$*/configure --prefix=$(gcc-base-directory)/$* --enable-languages=$(enable-languages) --enable-threads=posix --enable-shared; $(postcondition) \
	make; $(postcondition) \
	make html; $(postcondition) \
	make install; $(postcondition) \
	if [[ !(-d $(gcc_doc_dir)/$*/doc) ]]; then mv gcc/doc $(gcc_doc_dir)/$*; fi; $(postcondition) \
	if [[ !(-d $(gcc_doc_dir)/$*/man) ]]; then mv $(gcc-base-directory)/$*/man $(gcc_doc_dir)/$*; fi; $(postcondition) \
	mv $(gcc-base-directory)/gcc-$*_Build/gcc/HTML/gcc-$* $(gcc-base-directory)/gcc-$*_Build/gcc/html; $(postcondition) \
	if [[ !(-d $(gcc_doc_dir)/$*/html) ]]; then mv $(gcc-base-directory)/gcc-$*_Build/gcc/html $(gcc_doc_dir)/$*/html; fi; $(postcondition) \
	cd $(gcc-base-directory); $(postcondition) \
	touch $@; $(postcondition)

$(gcc_tag_paths_new) : $(gcc-base-directory)/_gcc-%  : | $(gcc-base-directory) $(gcc-base-directory)/% $(gcc_doc_dir)/%
	$(call unarchive,$(ExternalSources)/sources/Gcc/gcc-$*,$(gcc-base-directory)) $(postcondition) \
	if [[ !(-d $(gcc-base-directory)/gcc-$*_Build) ]]; then mkdir $(gcc-base-directory)/gcc-$*_Build; fi; $(postcondition) \
	cd $(gcc-base-directory)/gcc-$*_Build; $(postcondition) \
	../gcc-$*/configure --prefix=$(gcc-base-directory)/$* --enable-languages=$(enable-languages) --enable-threads=posix --enable-shared; $(postcondition) \
	make; $(postcondition) \
	make html dvi pdf; $(postcondition) \
	make install install-html; $(postcondition) \
	if [[ !(-d $(gcc_doc_dir)/$*/doc) ]]; then mv gcc/doc $(gcc_doc_dir)/$*; fi; $(postcondition) \
	if [[ !(-d $(gcc_doc_dir)/$*/man) ]]; then mv $(gcc-base-directory)/$*/man $(gcc_doc_dir)/$*; fi; $(postcondition) \
	mv $(gcc-base-directory)/$*/share/doc $(gcc-base-directory)/$*/share/html; $(postcondition) \
	if [[ !(-d $(gcc_doc_dir)/$*/html) ]]; then mv $(gcc-base-directory)/$*/share/html $(gcc_doc_dir)/$*/html; fi; $(postcondition) \
	cd $(gcc-base-directory); $(postcondition) \
	touch $@; $(postcondition)

# Comments:
# 1) If a documentation-directory already exists in doc/Gcc, then it is
# not changed.
# 2) Target pdf apparently only supported with version 4.2.0,
#    and same with install-html.

$(gcc_targets) : % : $(addprefix $(gcc-base-directory)/,_%)


# ####################################
# Cleaning
# ####################################

cleangcc :
	-rm -rf $(gcc_build_directory_paths) $(gcc_distribution_directories)

cleanallgcc : 
	-rm -rf $(gcc-base-directory) $(gcc_doc_dir)

