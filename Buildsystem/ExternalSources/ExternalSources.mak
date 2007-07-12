SHELL = /bin/sh
.SUFFIXES :

# The following definitions are temporary: The oklib-masterscript will define OKplatform and OKconfiguration.
ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
  endif
endif
ifndef OKconfiguration
  OKconfiguration := $(OKplatform)/.oklib/Configuration
endif

include $(OKconfiguration)/configuration_data.mak
export

# ######################################################################

include $(OKbuildsystem)/system_definitions.mak

prefix := $(shell pwd)
# OK: this looks odd???

# #################################
# Main targets
# #################################

.PHONY : all clean cleanall

all : gcc boost postgresql valgrind mhash doxygen ubcsat asciidoc git


# #################################
# Includes
# #################################

# Versions, System-wide definitions and local definitions :
include $(OKbuildsystem)/ExternalSources/definitions_.mak
# Special build targets:
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/gcc.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/boost.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/postgresql.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/valgrind.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/mhash.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/doxygen.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/ubcsat.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/asciidoc.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/git.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/cmake.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/R.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/mpfr.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/gmp.mak


# #################################
# Cleaning
# #################################

clean : cleangcc cleanboost cleanmhash

cleanall : clean cleanallgcc cleanallboost cleanallpostgresql cleanallvalgrind cleanallmhash cleanalldoxygen cleanallubcsat cleanallasciidoc cleanallgit cleanallmpfr
