SHELL = /bin/sh
.SUFFIXES :

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

# Boost

# Install Boost locally with system gcc:
#
# make boost_all (all supported versions of boost)
# make boost (recommended version)
# make boost boost_recommended=? (? is a version number like "1_33",
# overriding the default)
#
# For installing Boost locally with a local version of gcc (% is the version number),
# define
#
# gcc-version=%
#
# with % either the version-number (like "4.0.1") or "all" (for all supported
# gcc-versions).
# Finally with
#
# make boost_gcc_all
#
# all combinations of supported versions of boost with supported version of gcc
# plus the system gcc are installed.
# If some required local version is not available, then it will be built first.

# Mhash

# make mhash (recommended version)
# make mhash-? for building version number ?
# make gcc-version=% mhash
# with % either the version-number (like "4.0.1") or "all" (for all supported
# gcc-versions).
# make mhash_gcc_all
# for building all versions of mhash with all versions of gcc.

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

# UBCSAT

# make ubcsat

# creates the directory ExternalSources/Ubcsat/1-0-0
# with sub-directories bin, lib and src.

# Under bin the SAT solver "ubcsat" is found.
# Calling it by "ubcsat -h" will show the list of options.

# Under lib the library files are found.
# To use a function or variables from one of the .o files in Ubcsat/1-0-0/lib:
#  ubcsat.h must be included
#  all .o files in /Ubcsat/1-0-0/lib must be linked together

# These library files are unchanged from ubcsat-1-0-0.tar.gz except for ubcsat.o;
# ubcsat.o now does not have a main function and instead has the function ubcsatmain.
# Many of the functions and variables are in ubcsat-internal.o and ubcsat-triggers.o.
# These functions and variables are documented in ubcsat-globals.h, ubcsat-triggers.h 
# and ubcsat-internal.h

# Valgrind

# make valgrind (recommended version)
# make valgrind-?

# PostgreSQL

# Building server and client:
#   make postgresql (recommended version)
#   make postgresql-?

# Initialise a database and start the server (? is version number of postgresql):
#   a) Use the default location (postgresql-base-directory/?/data):
# 	make pgsql-version=? initialise-database
#   b) Use a different location (% is the absolute path of the location to place the database):
# 	make pgsql-version=? pgdata=% initialise-database

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

include $(OKbuildsystem)/system_definitions.mak

prefix := $(shell pwd)

# #################################
# Functions
# #################################

define postcondition 
if [ $$? != 0 ]; then exit 1; fi;
endef

define unarchive
if [ -f $(1).tar.gz ]; then tar --extract --directory=$(2) --file=$(1).tar.gz --ungzip; elif [ -f $(1).tar.bz2 ]; then tar --extract --directory=$(2) --file=$(1).tar.bz2 --bzip2; else exit 1; fi;
endef

# #################################
# Main targets
# #################################

.PHONY : all clean cleanall

all : gcc boost postgresql valgrind mhash doxygen


# #################################
# Includes
# #################################

include $(OKbuildsystem)/external_sources_versions.mak
include $(OKbuildsystem)/ExternalSources/mhash.mak
include $(OKbuildsystem)/ExternalSources/gcc.mak
include $(OKbuildsystem)/ExternalSources/doxygen.mak
include $(OKbuildsystem)/ExternalSources/boost.mak
include $(OKbuildsystem)/ExternalSources/postgresql.mak
include $(OKbuildsystem)/ExternalSources/valgrind.mak
include $(OKbuildsystem)/ExternalSources/ubcsat.mak

# #################################
# Cleaning
# #################################

clean : cleangcc cleanmhash cleanboost

cleanall : clean cleanallgcc cleanallmhash cleanallboost cleanalldoxygen cleanallpostgresql cleanallvalgrind
