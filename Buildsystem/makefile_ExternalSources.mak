SHELL = /bin/sh
.SUFFIXES :

doxygen_targets := doxygen-1.4.5
doxygen_recommended := doxygen-1.4.5
gcc_targets := gcc-3.4.3 gcc-3.4.4 gcc-3.4.5 gcc-4.0.0 gcc-4.0.1 gcc-4.0.2
gcc_recommended := gcc-4.0.2
boost_targets := boost-1_33_1
boost_recommended := boost-1_33_1
postgresql_targets := postgresql-8.0.3
postgresql_recommended := postgresql-8.0.3
valgrind_targets := valgrind-3.1.0
valgrind_recommended := valgrind-3.1.0
mhash_targets := mhash-0.9.2
mhash_recommended := mhash-0.9.2

# ####################################################
# Usage
# ####################################################

# Gcc

# Install all versions of gcc:
# make gcc_all
# Install recommended version of gcc:
# make gcc
# Install a version of gcc (? is the version number):
# make gcc-? 

# Boost

# Install Boost locally with system gcc:
# make boost_all (all supported versions of boost)
# make boost (recommended version)
# make boost boost_recommended=? (? is a version number like "1_33",
# overriding the default)
# For installing Boost locally with a version of gcc (% is the version number),
# define
# gcc-version=%
# with % either the version-number (like "4.0.1") or "all" (for all supported
# gcc-versions).
# Finally with
# make boost_gcc_all
# all combinations of supported versions of boost with supported version of gcc
# plus the system gcc are installed.

# The default minor version number of the Boost release is "_0"; if necessary,
# this can by changed by calling make with for example
# boost_minor_version="1"

# PostgreSQL

# make postgresql (recommended version)
# make postgresql-?

# Initialise a database and start the server (? is version number of postgresql):
# Use the default location (postgresql-base-directory/?/data):
# 	make pgsql-version=? initialise-database
# Use a different location (% is the absolute path of the location to place the database):
# 	make pgsql-version=? pgdata=% initialise-database

# Doxygen

# make doxygen (recommended version)
# make doxygen-?

# Valgrind

# make valgrind (recommended version)
# make valgrind-?


# --------------------------------------------------------------------------------------------------------------------------------------------------------------

#Directory Structure used for ExternalSources

#Root directory ".", where this makefile is located, contains also all software source archives and sub-directories.

#In the following text, ? denotes the GCC version number.
#./Gcc : Contain locally installed various versions of GCC.
#./Gcc/gcc-? : This is the original unarchived source directory for gcc version ?.
#./GCC/gcc-?_Build : This is the staging directory where configuration and temporary files are stored.
#./Gcc/? : This is the local installation of gcc version ?. 

#In the following text, ? denotes the GCC version number and % denotes the Boost version number, while & denotes the minor version number.
#./Boost : Contain locally stage built, various versions of Boost C++ Libraries.
#./Boost/boost_%_& : This is the original unarchived source directory for boost version %.
#./Boost/bjam : Contain the bjam tool. This binary is re-generated each time when boost is to be installed.
#./Boost/% : Locally installed boost using system-wide version of GCC.
#./Boost/%+?_Build : This is the staging directory where configuration and temporary files are stored. This Boost version % is build using GCC version ?. 
#./Boost/%+? : The built Boost Libraries using GCC version ?. Libraries in this directory are then copied to the GCC ? lib directory.

#In the following text, ? denotes the PostgreSql version number.
#./Postgresql : Contain locally installed various versions of PostgreSQL Database System.
#./Postgresql/postgresql-? : This is the original unarchived source directory for postgresql version ?.
#./Postgresql/? : This is the local installation of Postgresql version ?.

#In the following text, ? denotes the Doxygen version number.
#./Doxygen : Contain unarchived various versions of Doxygen.
#./Doxygen/doxygen-? : This is the original unarchived source directory which is also used for configuration and building. Doxygen is only installed system-wide.

#In the following text, ? denotes the Valgrind version number.
#./Valgrind : Contain unarchived various versions of Valgrind.
#./Valgrind/voxygen-? : This is the original unarchived source directory which is also used for configuration and building. Valgrind is only installed system-wide.


# ####################################################

prefix := $(shell pwd)

# ###################
doxygen-base-directory := $(prefix)/Doxygen
doxygen-directories := $(doxygen-base-directory)

# ###################
enable-languages := c,c++ # languages to build compiler for. Defaults to c, c++. Other languages are f77,objc,java,ada
gcc-version =
gcc-base-directory := $(prefix)/Gcc
gcc_build_directory_names := $(addsuffix _Build, $(gcc_targets))
gcc_build_directory_paths := $(addprefix $(gcc-base-directory)/,$(gcc_build_directory_names))
gcc_installation_directory_names := $(patsubst gcc-%, %, $(gcc_targets))
gcc_installation_directory_paths := $(addprefix $(gcc-base-directory)/,$(gcc_installation_directory_names))
gcc-directories := $(gcc-base-directory) $(gcc_build_directory_paths) $(gcc_installation_directory_paths)

# ###################

boost-base-directory := $(prefix)/Boost
abbr_boost_targets := $(patsubst boost-%, %, $(boost_targets))
# creates e.g. 1_32_0 1_33_1

boost_installation_directory_names := $(foreach gccversion, $(gcc_installation_directory_names), $(addsuffix +$(gccversion), $(abbr_boost_targets)))
# creates e.g. 1_32_0+3.4.3 1_32_0+3.4.4 1_33_1+3.4.3 1_33_1+3.4.4
boost_installation_directory_names += $(abbr_boost_targets)
boost_installation_directory_paths := $(addprefix $(boost-base-directory)/,$(boost_installation_directory_names))

boost_build_directory_names := $(addsuffix _Build, $(boost_installation_directory_names))
boost_build_directory_paths := $(addprefix $(boost-base-directory)/,$(boost_build_directory_names))

bjam_directory_path := $(boost-base-directory)/bjam

boost-directories := $(boost-base-directory) $(boost_build_directory_paths) $(boost_installation_directory_paths) $(bjam_directory_path)

boost_distribution_directories := $(addprefix $(boost-base-directory)/boost_, $(abbr_boost_targets))

boost_gcc_targets := $(foreach boostversion, $(boost_targets), $(addprefix $(boostversion)+, $(gcc_installation_directory_names)))
all_boost_targets := $(boost_targets) $(boost_gcc_targets)

# ###################
pgsql-version :=
pgdata :=
postgresql-base-directory := $(prefix)/Postgresql
postgresql-directories := $(postgresql-base-directory)

# ###################
valgrind-base-directory := $(prefix)/Valgrind
valgrind-directories := $(valgrind-base-directory)

# ###################
mhash-base-directory := $(prefix)/Mhash

abbr_mhash_targets := $(patsubst mhash-%, %, $(mhash_targets))
mhash_installation_directory_names := $(foreach gccversion, $(gcc_installation_directory_names), $(addsuffix +$(gccversion), $(abbr_mhash_targets)))
mhash_installation_directory_names += $(abbr_mhash_targets)

mhash_installation_directory_paths := $(addprefix $(mhash-base-directory)/,$(mhash_installation_directory_names))
mhash_build_directory_names := $(addsuffix _Build, $(mhash_installation_directory_names))
mhash_build_directory_paths := $(addprefix $(mhash-base-directory)/,$(mhash_build_directory_names))
mhash-directories := $(mhash-base-directory) $(mhash_build_directory_paths) $(mhash_installation_directory_paths)

# ###############################################

define postcondition 
if [ $$? != 0 ]; then exit 1; fi;
endef

define unarchive
if [ -f $(1).tar.gz ]; then tar --extract --directory=$(2) --file=$(1).tar.gz --ungzip; elif [ -f $(1).tar.bz2 ]; then tar --extract --directory=$(2) --file=$(1).tar.bz2 --bzip2; else exit 1; fi;
endef

.PHONY : boost boost_all boost_gcc_all $(all_boost_targets)
.PHONY : gcc gcc_all $(gcc_targets) create_gcc_dirs
.PHONY : doxygen $(doxygen_targets) create_doxygen_dirs
.PHONY : postgresql $(postgresql_targets) initialise-database create_postgresql_dirs
.PHONY : valgrind $(valgrind_targets) create_valgrind_dirs
.PHONY : mhash $(mhash_targets) create_mhash_dirs
.PHONY : all clean cleanall

all : gcc boost postgresql valgrind mhash doxygen

# ###############################
# Doxygen
# ###############################

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

# ###############################
# Gcc
# ###############################

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

# ###############################
# Boost
# ###############################

$(boost_installation_directory_paths) : % : | $(boost-base-directory) %_Build $(bjam_directory_path)

$(boost-directories) : % : 
	mkdir $@

# Making boost with the system gcc:

define install-boost
	$(bjam_directory_path)/bjam "-sTOOLS=gcc" --prefix=$(boost-base-directory)/$(1) --builddir=$(boost-base-directory)/$(1)_Build install
endef

$(boost-base-directory)/$(boost_targets) : $(boost-base-directory)/boost-% : $(boost-base-directory)/%
	$(call unarchive,boost_$*,$(boost-base-directory))
	cd $(boost-base-directory)/boost_$*; $(postcondition) \
	cd tools/build/jam_src/; $(postcondition) \
	./build.sh; $(postcondition) \
	cp bin.*/bjam $(bjam_directory_path); $(postcondition) \
	cd $(boost-base-directory)/boost_$*; $(postcondition) \
	$(call install-boost,$*)
	touch $@

# Making boost with a local gcc:

define install-boost_gcc
              $(bjam_directory_path)/bjam "-sTOOLS=gcc" "-sGCC_ROOT_DIRECTORY=$(gcc-base-directory)/$(2)" --prefix=$(boost-base-directory)/$(1)+$(2) --builddir=$(boost-base-directory)/$(1)+$(2)_Build install
#	if [ -d $(gcc-base-directory)/$(2)/boost-$(1) ]; then echo; else mkdir $(gcc-base-directory)/$(2)/boost-$(1); fi;
#	cp $(boost-base-directory)/$(1)+$(2)/lib/* $(gcc-base-directory)/$(2)/boost-$(1)
#	if [ -d $(gcc-base-directory)/$(2)/include/boost-$(1) ]; then echo; else mkdir $(gcc-base-directory)/$(2)/include/boost-$(1); fi;
#	cp -r $(boost-base-directory)/$(1)+$(2)/include/boost-$(1)/boost $(gcc-base-directory)/$(2)/include/boost-$(1)
endef

define boost_gcc_rule
$(boost-base-directory)/boost-$(1)+$(2) : $(boost-base-directory)/$(1)+$(2)
	$(call unarchive,boost_$(1),$(boost-base-directory))
	cd $(boost-base-directory)/boost_$(1); if [ $$$$? != 0 ]; then exit 1; fi; \
	cd tools/build/jam_src/;  if [ $$$$? != 0 ]; then exit 1; fi; \
	./build.sh;  if [ $$$$? != 0 ]; then exit 1; fi; \
	cp bin.*/bjam $(bjam_directory_path);  if [ $$$$? != 0 ]; then exit 1; fi; \
	cd $(boost-base-directory)/boost_$(1);  if [ $$$$? != 0 ]; then exit 1; fi; \
	$(call install-boost_gcc,$(1),$(2))
	touch $@
endef

$(foreach boostversion, $(abbr_boost_targets), $(foreach gccversion, $(gcc_installation_directory_names), $(eval $(call boost_gcc_rule,$(boostversion),$(gccversion)))))

# The main targets for making boost

boost_gcc_all : $(all_boost_targets)

$(all_boost_targets) : % : $(boost-base-directory)/%

ifeq ($(gcc-version),all)
 boost_all : $(boost_gcc_targets)
 boost : $(addprefix $(boost_recommended)+,$(gcc_installation_directory_names))
else
 ifeq ($(gcc-version),)
  boost_all : $(boost_targets)
  boost : $(boost_recommended)
 else
  boost_all : $(addsuffix $(gcc-version),$(boost_targets))
  boost : $(boost_recommended)+$(gcc-version)
 endif
endif

# ###############################
# PostgreSQL
# ###############################

$(postgresql-directories) : % : 
	mkdir $@

create_postgresql_dirs : $(postgresql-directories)

postgresql : $(postgresql_recommended)

ifeq ($(pgdata), )
pgdata := $(postgresql-base-directory)/$(pgsql-version)/data
endif

$(postgresql_targets) : create_postgresql_dirs
	$(call unarchive,$@,$(postgresql-base-directory))
	cd $(postgresql-base-directory)/$@; $(postcondition) \
	./configure CFLAGS='-Wl,-rpath,$(postgresql-base-directory)/$*/lib' --prefix=$(postgresql-base-directory)/$*; $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition) \

initialise-database :
	$(postgresql-base-directory)/$(pgsql-version)/bin/initdb -D $(pgdata)
	$(postcondition)
	$(postgresql-base-directory)/$(pgsql-version)/bin/pg_ctl -D $(pgdata) -l logfile start

# ###############################
# Valgrind
# ###############################

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

# ###############################
# Mhash
# ###############################

$(mhash-directories) : % : 
	mkdir $@

create_mhash_dirs : $(mhash-directories)

ifeq ($(gcc-version),)

$(mhash_targets) :  mhash-% : create_mhash_dirs
	$(call unarchive,$@,$(mhash-base-directory))
	cd $(mhash-base-directory)/$*_Build; $(postcondition) \
	sh ../mhash-$*/configure --prefix=$(mhash-base-directory)/$*; $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition)

else 

$(mhash_targets) :  mhash-% : create_mhash_dirs
	$(call unarchive,$@,$(mhash-base-directory))
	cd $(mhash-base-directory)/$*+$(gcc-version)_Build; $(postcondition) \
	sh ../mhash-$*/configure CC=$(gcc-base-directory)/$(gcc-version)/bin/gcc --prefix=$(mhash-base-directory)/$*+$(gcc-version); $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition)

endif

mhash : $(mhash_recommended)

# ####################################################

clean :
	-rm -rf $(gcc_build_directory_paths)
	-rm -rf $(boost_build_directory_paths) $(boost_distribution_directories) $(bjam_directory_path)
	-rm -rf $(mhash_build_directory_paths)

cleanall : clean
	-rm -rf $(doxygen-directories)
	-rm -rf $(gcc-directories)
	-rm -rf $(boost-directories)
	-rm -rf $(postgresql-directories)
	-rm -rf $(valgrind-directories)
	-rm -rf $(mhash-directories)
