SHELL = /bin/sh
.SUFFIXES :

doxygen_targets := doxygen-1.4.4
gcc_targets := gcc-3.4.3 gcc-3.4.4 gcc-4.0.0 gcc-4.0.1
boost_targets := boost-1_32 boost-1_33
postgresql_targets := postgresql-8.0.3
valgrind_targets := valgrind-3.0.1

# Usage

# Install Doxygen (? is the version number)
# make doxygen-?

# Install all versions of gcc
# make gcc

# Install a version of gcc (? is the version number)
# make gcc-? 

# Install Boost locally (? is version number) with system gcc
# make boost_?

# Install Boost locally (? is version number) with a version of gcc (% is the version number) 
# make gcc-version=% boost_?

# Install all versions of boost with aspecified version of gcc.
# Use system gcc
# 	make boost
# Use a local gcc with version number ?
# 	make gcc-version=? boost

# Install PostgreSQL (? is version number)
# make postgresql-?

# Initialise a database and start the server (? is version number of postgresql)
# Use the default location (postgresql-base-directory/?/data)
# 	make pgsql-version=? initialise-database
# Use a different location (% is the absolute path of the location to place the database)
# 	make pgsql-version=? pgdata=% initialise-database

# Install Doxygen (? is the version number)
# make doxygen-?

# --------------------------------------------------------------------------------------------------------------------------------------------------------------

#Directory Structure used for ExternalSources

#Root directory ".", where this makefile is located, contains also all software source archives and sub-directories.

#In the following text, ? denotes the GCC version number.
#./Gcc : Contain locally installed various versions of GCC.
#./Gcc/gcc-? : This is the original unarchived source directory for gcc version ?.
#./GCC/gcc-?_Build : This is the staging directory where configuration and temporary files are stored.
#./Gcc/? : This is the local installation of gcc version ?. 

#In the following text, ? denotes the GCC version number and % denotes the Boost version number.
#./Boost : Contain locally stage built, various versions of Boost C++ Libraries.
#./Boost/boost_% : This is the original unarchived source directory for boost version %.
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


# ------------------------------------------------------

prefix := $(shell pwd)
gcc-version := system
pgsql-version :=
pgdata :=
# languages to build compiler for. Defaults to c, c++. Other languages are f77,objc,java,ada
enable-languages := c,c++

doxygen-base-directory := $(prefix)/Doxygen
gcc-base-directory := $(prefix)/Gcc
boost-base-directory := $(prefix)/Boost
postgresql-base-directory := $(prefix)/Postgresql
valgrind-base-directory := $(prefix)/Valgrind

gcc_build_directory_names := $(addsuffix _Build, $(gcc_targets))
gcc_build_directory_paths := $(addprefix $(gcc-base-directory)/,$(gcc_build_directory_names))
gcc_installation_directory_names := $(patsubst gcc-%, %, $(gcc_targets))
gcc_installation_directory_paths := $(addprefix $(gcc-base-directory)/,$(gcc_installation_directory_names))

boost_build_directory_names := $(foreach gccversion, $(gcc_installation_directory_names), $(addsuffix +$(gccversion)_Build, $(patsubst boost_%, %, $(boost_targets))))
boost_build_directory_names += $(addsuffix _Build, $(patsubst boost_%, %, $(boost_targets)))
boost_build_directory_paths := $(addprefix $(boost-base-directory)/,$(boost_build_directory_names))
boost_installation_directory_names := $(foreach gccversion, $(gcc_installation_directory_names), $(addsuffix +$(gccversion), $(patsubst boost_%, %, $(boost_targets))))
boost_installation_directory_names += $(patsubst boost_%, %, $(boost_targets))
boost_installation_directory_paths := $(addprefix $(boost-base-directory)/,$(boost_installation_directory_names))

bjam_directory_path := $(boost-base-directory)/bjam

doxygen-directories := $(doxygen-base-directory)
gcc-directories := $(gcc-base-directory) $(gcc_build_directory_paths) $(gcc_installation_directory_paths)
boost-directories := $(boost-base-directory) $(boost_build_directory_paths) $(boost_installation_directory_paths) $(bjam_directory_path)
postgresql-directories := $(postgresql-base-directory)
valgrind-directories := $(valgrind-base-directory)

define postcondition 
if [ $$? != 0 ]; then exit 1; fi;
endef

define unarchive
if [ -f $1.tar.gz ]; then \
	tar --extract --directory=$2 --file=$1.tar.gz --ungzip;\
else \
	if [ -f $1.tar.bz2 ]; then \
		tar --extract --directory=$2 --file=$1.tar.bz2 --bzip2;\
	else \
		exit 1; \
	fi; \
fi;
endef


.PHONY : all gcc boost clean cleanall create_doxygen_dirs create_gcc_dirs create_boost_dirs create_postgresql_dirs valgrind_dirs $(doxygen_targets) $(gcc_targets) $(boost_targets) $(postgresql_targets) initialise-database $(valgrind_targets)

all :

$(doxygen-directories) : % : 
	mkdir $@

create_doxygen_dirs : $(doxygen-directories)

$(doxygen_targets) : create_doxygen_dirs
	$(call unarchive,$@.src,$(doxygen-base-directory))
	cd $(doxygen-base-directory)/$@; $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition) \
	make docs; $(postcondition) \
	make pdf; $(postcondition) \
	sudo make install; $(postcondition) \
	sudo make install_docs

$(gcc-directories) : % : 
	mkdir $@

create_gcc_dirs : $(gcc-directories)

$(gcc_targets) : gcc-% : create_gcc_dirs
	$(call unarchive,$@,$(gcc-base-directory))
	cd $(gcc-base-directory)/$@_Build; $(postcondition) \
	../$@/configure --prefix=$(gcc-base-directory)/$* --enable-languages=$(enable-languages) --enable-threads=posix --enable-shared; $(postcondition) \
	make bootstrap; $(postcondition) \
	make install; 

gcc : $(gcc_targets)

$(boost-directories) : % : 
	mkdir $@

create_boost_dirs : $(boost-directories)

ifeq ($(gcc-version),system)
define install-boost 
	$(bjam_directory_path)/bjam "-sTOOLS=gcc" --prefix=$(boost-base-directory)/$@ --builddir=$(boost-base-directory)/$@_Build install
endef
else 
define install-boost
	$(bjam_directory_path)/bjam "-sTOOLS=gcc" "-sGCC_ROOT_DIRECTORY=$(gcc-base-directory)/$(gcc-version)" --prefix=$(boost-base-directory)/$*+$(gcc-version) --builddir=$(boost-base-directory)/$*+$(gcc-version)_Build install
	if [ -d $(gcc-base-directory)/$(gcc-version)/$@ ]; then echo; else mkdir $(gcc-base-directory)/$(gcc-version)/$@; fi;
	cp $(boost-base-directory)/$*+$(gcc-version)/lib/* $(gcc-base-directory)/$(gcc-version)/$@
	if [ -d $(gcc-base-directory)/$(gcc-version)/include/$@ ]; then echo; else mkdir $(gcc-base-directory)/$(gcc-version)/include/$@; fi;
	cp -r $(boost-base-directory)/$*+$(gcc-version)/include/boost-$*/boost $(gcc-base-directory)/$(gcc-version)/include/$@
endef
endif

$(boost_targets) : boost-% : create_boost_dirs
	$(call unarchive,boost_$*_0,$(boost-base-directory))
	cd $(boost-base-directory)/boost_$*_0; $(postcondition) \
	cd tools/build/jam_src/; $(postcondition) \
	./build.sh; $(postcondition) \
	cp bin.*/bjam $(bjam_directory_path); $(postcondition) \
	cd $(boost-base-directory)/boost_$*_0; $(postcondition) \
	$(install-boost)

boost : $(boost_targets)

$(postgresql-directories) : % : 
	mkdir $@

create_postgresql_dirs : $(postgresql-directories)

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

$(valgrind-directories) : % : 
	mkdir $@

create_valgrind_dirs : $(valgrind-directories)

$(valgrind_targets) : create_valgrind_dirs
	$(call unarchive,$@,$(valgrind-base-directory))
	cd $(valgrind-base-directory)/$@; $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition) \
	sudo make install; $(postcondition)

# --------------------------------------------------------------------------------------------------

clean :
	-rm -rf $(gcc_build_directory_paths)
	-rm -rf $(boost_build_directory_paths)

cleanall : clean
	-rm -rf $(doxygen-directories)
	-rm -rf $(gcc-directories)
	-rm -rf $(boost-directories)
	-rm -rf $(postgresql-directories)
	-rm -rf $(valgrind-directories)
