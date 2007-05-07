# Matthew Henderson, 18.7.2006 (Paderborn)
# filename : Buildsystem/ExternalSources/gcc.mak

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
gcc-directories := $(gcc-base-directory) $(gcc_build_directory_paths) $(gcc_installation_directory_paths) $(gcc_distribution_directories) $(gcc_doc_dir)

.PHONY : gcc gcc_all $(gcc_targets)

# ####################################
# Documentation
# ####################################

gcc_doc : | $(gcc_doc_dir)
	- $(call unarchive,$(gcc_recommended),$(gcc_doc_dir))

# ####################################
# The main targets for making gcc
# ####################################

gcc_all : $(gcc_targets)

gcc : $(gcc_recommended)

$(gcc-directories) : % : 
	mkdir $@

gcc_timestamp_prefix := _
gcc_tag_names:= $(addprefix $(gcc_timestamp_prefix),$(gcc_targets))
gcc_tag_paths := $(addprefix $(gcc-base-directory)/,$(gcc_tag_names))

$(gcc_tag_paths) : $(gcc-base-directory)/_gcc-%  : | $(gcc-base-directory) $(gcc-base-directory)/gcc-%_Build $(gcc-base-directory)/%
	$(call unarchive,gcc-$*,$(gcc-base-directory))
	cd $(gcc-base-directory)/gcc-$*_Build; $(postcondition) \
	../gcc-$*/configure --prefix=$(gcc-base-directory)/$* --enable-languages=$(enable-languages) --enable-threads=posix --enable-shared; $(postcondition) \
	make bootstrap; $(postcondition) \
	make install; 
	cd $(gcc-base-directory); $(postcondition) \
	touch $@ 

$(gcc_targets) : % : $(addprefix $(gcc-base-directory)/,_%)


# ####################################
# Cleaning
# ####################################

cleangcc :
	-rm -rf $(gcc_build_directory_paths) $(gcc_distribution_directories)

cleanallgcc : 
	-rm -rf $(gcc-base-directory)


