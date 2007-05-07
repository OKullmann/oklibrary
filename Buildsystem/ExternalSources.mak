SHELL = /bin/sh
.SUFFIXES :

# For user documentation see Buildsystem/docus/ExternalSources.hpp.

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
# OK: this looks odd???

# #################################
# Main targets
# #################################

.PHONY : all clean cleanall

all : gcc boost postgresql valgrind mhash doxygen ubcsat


# #################################
# Includes
# #################################

# Versions, System-wide definitions and local definitions :
include $(OKbuildsystem)/external_sources_versions.mak
include $(OKbuildsystem)/ExternalSources/definitions_.mak
# Special build targets:
include $(OKbuildsystem)/ExternalSources/gcc.mak
include $(OKbuildsystem)/ExternalSources/boost.mak
include $(OKbuildsystem)/ExternalSources/postgresql.mak
include $(OKbuildsystem)/ExternalSources/valgrind.mak
include $(OKbuildsystem)/ExternalSources/mhash.mak
include $(OKbuildsystem)/ExternalSources/doxygen.mak
include $(OKbuildsystem)/ExternalSources/ubcsat.mak
include $(OKbuildsystem)/ExternalSources/git.mak
# Git is experimental

# #################################
# Cleaning
# #################################

clean : cleangcc cleanboost cleanmhash

cleanall : clean cleanallgcc cleanallboost cleanallpostgresql cleanallvalgrind cleanallmhash cleanalldoxygen
