# Matthew Henderson, 18.7.2006 (Paderborn)
# filename : Buildsystem/ExternalSources/makefile_gcc.mak

# ##################################
# Usage
# ##################################

# Gcc

# Install all versions of gcc:
# make gcc_all
# Install recommended version of gcc:
# make gcc
# Install a version of gcc (? is the version number):
# make gcc-? 

# ##################################
# Versions
# ##################################

include $(OKBuildsystem)/external_sources_versions.mak

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
gcc-base-directory := $(prefix)/Gcc
gcc_build_directory_names := $(addsuffix _Build, $(gcc_targets))
gcc_build_directory_paths := $(addprefix $(gcc-base-directory)/,$(gcc_build_directory_names))
gcc_installation_directory_names := $(patsubst gcc-%, %, $(gcc_targets))
gcc_installation_directory_paths := $(addprefix $(gcc-base-directory)/,$(gcc_installation_directory_names))
gcc_distribution_directories := $(addprefix $(gcc-base-directory)/,$(gcc_targets))
gcc-directories := $(gcc-base-directory) $(gcc_build_directory_paths) $(gcc_installation_directory_paths) $(gcc_distribution_directories)

.PHONY : gcc gcc_all $(gcc_targets) create_gcc_dirs

# ####################################
# The main targets for making gcc
# ####################################

gcc_all : $(gcc_targets)

gcc : $(gcc_recommended)

$(gcc-directories) : % : 
	mkdir $@

create_gcc_dirs : $(gcc-directories)

$(gcc_targets) : gcc-% : create_gcc_dirs
	$(call unarchive,$@,$(gcc-base-directory))
	cd $(gcc-base-directory)/$@_Build; $(postcondition) \
	../$@/configure --prefix=$(gcc-base-directory)/$* --enable-languages=$(enable-languages) --enable-threads=posix --enable-shared; $(postcondition) \
	make bootstrap; $(postcondition) \
	make install; 

# ####################################
# Cleaning
# ####################################

cleangcc :
	-rm -rf $(gcc_build_directory_paths) $(gcc_distribution_directories)

cleanallgcc : 
	-rm -rf $(gcc-base-directory)


