# Matthew Henderson, 19.7.2006 (Paderborn)
# filename : Buildsystem/ExternalSources/valgrind.mak

# ##################################
# Directory Structure
# ################################## 

#Root directory ".", where this makefile is located via inclusion),
# contains also all software source archives and sub-directories.

#In the following text, ? denotes the Valgrind version number.
#./Valgrind : Contain unarchived various versions of Valgrind.
#./Valgrind/valgrind-? : This is the original unarchived source directory which is also used for configuration and building. Valgrind is only installed system-wide.

valgrind-base-directory := $(prefix)/Valgrind
valgrind-directories := $(valgrind-base-directory)

.PHONY : valgrind $(valgrind_targets) create_valgrind_dirs

# #################################
# Main Valgrind targets
# #################################

$(valgrind-directories) : % : 
	mkdir $@

create_valgrind_dirs : $(valgrind-directories)

valgrind : $(valgrind_recommended)

$(valgrind_targets) : create_valgrind_dirs
	$(call unarchive,$@,$(valgrind-base-directory))
	cd $(valgrind-base-directory)/$@; $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallvalgrind : 
	-rm -rf $(valgrind-base-directory)
