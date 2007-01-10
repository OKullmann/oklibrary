# Matthew Henderson, 19.7.2006 (Paderborn)
# filename : Buildsystem/ExternalSources/makefile_doxygen.mak 

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
doxygen-directories := $(doxygen-base-directory)

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
