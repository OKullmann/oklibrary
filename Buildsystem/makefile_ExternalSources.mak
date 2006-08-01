SHELL = /bin/sh
.SUFFIXES :

# At the moment the gcc_targets have to go here because they are
# needed by both makefile_gcc.mak and makefile_boost.mak
# Perhaps actually all such variables should go here. (Probably better
# that the individual makefiles are standalone).

gcc_targets := gcc-3.4.3 gcc-3.4.4 gcc-3.4.5 gcc-3.4.6 gcc-4.0.0 gcc-4.0.1 gcc-4.0.2 gcc-4.0.3 gcc-4.1.0 gcc-4.1.1
# remark: gcc-3.4.6 creates linking errors on cs-wsok when compiling
# optimised new test programs (it seems that code for message objects has
# been optimised away); one needs to find out for which other gcc-versions
# we also have this problem (at least with gcc-4.0.3 the problem seems
# to be solved).
gcc_recommended := gcc-4.1.1

####################################################

ifndef OKPlatform
  ifdef OKPLATFORM
    OKPlatform := $(OKPLATFORM)
  else
    $(error Either OKPlatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKSystem
  ifdef OKSYSTEM
    OKSystem := $(OKSYSTEM)
  else
    OKSystem := $(OKPlatform)/OKsystem
  endif
endif

ifndef OKBuildsystem
  ifdef OKBUILDSYSTEM
    OKBuildsystem := $(OKBUILDSYSTEM)
  else
    OKBuildsystem := $(OKSystem)/Transitional/Buildsystem
  endif
endif

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

include $(OKBuildsystem)/ExternalSources/makefile_mhash.mak
include $(OKBuildsystem)/ExternalSources/makefile_gcc.mak
include $(OKBuildsystem)/ExternalSources/makefile_doxygen.mak
include $(OKBuildsystem)/ExternalSources/makefile_boost.mak
include $(OKBuildsystem)/ExternalSources/makefile_postgresql.mak
include $(OKBuildsystem)/ExternalSources/makefile_valgrind.mak

# #################################
# Cleaning
# #################################

clean : cleangcc cleanmhash cleanboost

cleanall : clean cleanallgcc cleanallmhash cleanallboost cleanalldoxygen cleanallpostgresql cleanallvalgrind
