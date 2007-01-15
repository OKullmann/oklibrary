# Matthew Henderson, 19.7.2006 (Paderborn)
# filename : Buildsystem/ExternalSources/makefile_doxygen.mak 

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
# Versions
# ##################################

include $(OKbuildsystem)/external_sources_versions.mak

# ##################################
# Directory Structure
# ##################################

#Root directory ".", where this makefile is located, contains also all software source archives and sub-directories.

#In the following text, ? denotes the Doxygen version number.
#./Doxygen : Contain unarchived various versions of Doxygen.
#./Doxygen/doxygen-? : This is the original unarchived source directory which is also used for configuration and building. Doxygen is only installed system-wide.

doxygen-base-directory := $(prefix)/Doxygen
doxygen_doc_dir := $(external_sources_doc_base_dir)/Doxygen
doxygen-directories := $(doxygen-base-directory) $(doxygen_doc_dir)

# ##################################
# Documentation
# ##################################

doxygen_doc : | $(doxygen_doc_dir)
	$(call unarchive,$(doxygen_recommended_package_name),$(doxygen_doc_dir))
	cd $(doxygen_doc_dir)/$(doxygen_recommended); $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition)\
	make docs; $(postcondition) \
	make pdf; $(postcondition) \

# ##################################
# The main targets for doxygen
# ##################################

.PHONY : doxygen $(doxygen_targets) create_doxygen_dirs

$(doxygen-directories) : % : 
	mkdir $@

create_doxygen_dirs : $(doxygen-directories)

doxygen : $(doxygen_recommended)

$(doxygen_targets) : create_doxygen_dirs
	$(call unarchive,$@.src,$(doxygen-base-directory))
	cd $(doxygen-base-directory)/$@; $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition) \
	make docs; $(postcondition) \
	make pdf; $(postcondition) \
	sudo make install; $(postcondition) \
	sudo make install_docs

# ####################################
# Cleaning
# ####################################

cleanalldoxygen :
	-rm -rf $(doxygen-base-directory)
