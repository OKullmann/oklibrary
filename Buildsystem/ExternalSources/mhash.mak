# Matthew Henderson, 6.3.2006 (Swansea)
# filename : Buildsystem/ExternalSources/mhash.mak

# ################################################################
# Original definitions of OKplatform and OKbuildsystem, are 
# in Transtional/Buildsystem/generic.mak and cut-and-pasted
# to :
#  Transitional/Buildsystem/ExternalSources.mak
#  Transitional/Buildsystem/ExternalSources/boost.mak
#  Transitional/Buildsystem/ExternalSources/doxygen.mak
#  Transitional/Buildsystem/ExternalSources/gcc.mak
#  Transitional/Buildsystem/ExternalSources/mhash.mak
#  Transitional/Buildsystem/ExternalSources/postgresql.mak
#  Transitional/Buildsystem/ExternalSources/ubcsat.mak
#  Transitional/Buildsystem/ExternalSources/valgrind.mak
#  Transitional/Buildsystem/makefile
#  Transitional/Buildsystem/OKsystem.mak
#  Transitional/Buildsystem/recursive.mak
#  Transitional/Buildsystem/Transitional.mak
#  Transitional/Buildsystem/Annotations.mak
# ################################################################

ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKsystem
  ifdef OKSYSTEM
    OKsystem := $(OKSYSTEM)
  else
    OKsystem := $(OKplatform)/OKsystem
  endif
endif

ifndef OKbuildsystem
  ifdef OKBUILDSYSTEM
    OKbuildsystem := $(OKBUILDSYSTEM)
  else
    OKbuildsystem := $(OKsystem)/Transitional/Buildsystem
  endif
endif

# ######################################################################

# ##################################
# Directory Structure
# ##################################

mhash-base-directory := $(prefix)/Mhash
abbr_mhash_targets := $(patsubst mhash-%, %, $(mhash_targets))

mhash_extract_dirs := $(addprefix /,$(mhash_targets))
mhash_extract_dirs_paths := $(addprefix $(mhash-base-directory),$(mhash_extract_dirs))

# This line is necessary - but also belongs to makefile_gcc.mak
gcc_installation_directory_names := $(patsubst gcc-%, %, $(gcc_targets))

mhash_installation_directory_names := $(foreach gccversion, $(gcc_installation_directory_names), $(addsuffix +$(gccversion), $(abbr_mhash_targets)))

mhash_installation_directory_names += $(abbr_mhash_targets)
mhash_installation_directory_paths := $(addprefix $(mhash-base-directory)/,$(mhash_installation_directory_names))

mhash_build_directory_names := $(addsuffix _Build, $(mhash_installation_directory_names))
mhash_build_directory_paths := $(addprefix $(mhash-base-directory)/,$(mhash_build_directory_names))

mhash_distribution_directories := $(addprefix $(mhash-base-directory)/mhash-, $(abbr_mhash_targets))

mhash_doc_dir := $(external_sources_doc_base_dir)/Mhash

mhash-directories := $(mhash-base-directory) $(mhash_build_directory_paths) $(mhash_installation_directory_paths) $(mhash_doc_dir)

# ##################################
# Documentation
# ##################################

mhash_doc : | $(mhash_doc_dir)
	- $(call unarchive,$(mhash_recommended),$(mhash_doc_dir))
# OK : This should automatically be done when building.

# ##################################
# Making mhash with the system gcc:
# ##################################

mhash_gcc_targets := $(foreach mhashversion, $(mhash_targets), $(addprefix $(mhashversion)+, $(gcc_installation_directory_names)))
all_mhash_targets := $(mhash_targets) $(mhash_gcc_targets)

.PHONY : mhash mhash_all mhash_gcc_all $(all_mhash_targets) cleanmhash cleanallmhash

$(mhash_installation_directory_paths) : % : | $(mhash-base-directory) %_Build 

$(mhash-directories) : % : 
	mkdir $@

define install-mhash
	cd $(mhash-base-directory)/$(1)_Build; $(postcondition) \
	$(mhash-base-directory)/mhash-$(1)/configure --prefix=$(mhash-base-directory)/$(1); $(postcondition) \
	cp $(mhash-base-directory)/mhash-$(1)/include/mutils/*.h $(mhash-base-directory)/$(1)_Build/include/mutils; $(postcondition) \
	make;	$(postcondition) \
	make install;
endef

$(mhash_extract_dirs_paths) : $(mhash-base-directory)/mhash-% : $(mhash-base-directory)/%
	$(call unarchive,mhash-$*,$(mhash-base-directory))
	$(call install-mhash,$*)
	touch $@

# ##################################
# Making mhash with a local gcc
# ##################################

gcc-base-directory := $(prefix)/Gcc

define install-mhash_gcc
	cd $(mhash-base-directory)/$(1)+$(2)_Build;  if [ $$$$? != 0 ]; then exit 1; fi; \
	$(mhash-base-directory)/mhash-$(1)/configure --prefix=$(mhash-base-directory)/$(1)+$(2) --with-CC=$(gcc-base-directory)/$(gcc-version)/bin/gcc; if [ $$$$? != 0 ]; then exit 1; fi; \
	cp $(mhash-base-directory)/mhash-$(1)/include/mutils/*.h $(mhash-base-directory)/$(1)+$(2)_Build/include/mutils; if [ $$$$? != 0 ]; then exit 1; fi; \
	make; if [ $$$$? != 0 ]; then exit 1; fi; \
	make install;
endef

define mhash_gcc_rule
$(mhash-base-directory)/mhash-$(1)+$(2) : $(mhash-base-directory)/$(1)+$(2)
	$(call unarchive,mhash-$(1),$(mhash-base-directory))
	$(call install-mhash_gcc,$(1),$(2))
endef

$(foreach mhashversion, $(abbr_mhash_targets), $(foreach gccversion, $(gcc_installation_directory_names), $(eval $(call mhash_gcc_rule,$(mhashversion),$(gccversion)))))

# ####################################
# The main targets for making mhash
# ####################################

mhash_gcc_all : $(all_mhash_targets)

$(all_mhash_targets) : % : $(mhash-base-directory)/%

ifeq ($(gcc-version),all)
 mhash_all : $(mhash_gcc_targets)
 mhash : $(addprefix $(mhash_recommended)+,$(gcc_installation_directory_names))
else
 ifeq ($(gcc-version),)
  mhash_all : $(mhash_targets)
  mhash : $(mhash_recommended)
 else
  mhash_all : $(addsuffix $(gcc-version),$(mhash_targets))
  mhash : $(mhash_recommended)+$(gcc-version)
 endif
endif

# ####################################
# Cleaning
# ####################################

cleanmhash : 
	-rm -rf $(mhash_build_directory_paths)


cleanallmhash : 
	-rm -rf $(mhash-directories)

