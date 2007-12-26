# Oliver Kullmann, 13.6.2005 (Swansea)

# Master makefile for building external sources

SHELL = /bin/bash
.SUFFIXES :

# The oklib-masterscript defines OKplatform.
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

prefix := $(shell pwd)
# OK: this looks odd??? Must be eliminated.

# #################################
# Main targets
# #################################

.PHONY : all clean cleanall

all : gcc boost postgresql valgrind mhash doxygen ubcsat asciidoc git

# #################################
# Functions
# #################################

define postcondition 
if [ $$? != 0 ]; then exit 1; fi;
endef

define unarchive_uncompressed
if [ -f $(1).tar ]; then tar --extract --directory=$(2) --file=$(1).tar; else exit 1; fi;
endef

define unarchive
if [ -f $(1).tar.gz ]; then tar --extract --directory=$(2) --file=$(1).tar.gz --ungzip; elif [ -f $(1).tar.bz2 ]; then tar --extract --directory=$(2) --file=$(1).tar.bz2 --bzip2; else exit 1; fi;
endef

# #################################
# Directory Structure
# #################################

external_sources_doc_base_dir := $(ExternalSources)/doc

# #################################
# Includes
# #################################

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
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/sage.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/ocaml.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/coq.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/clisp.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/maxima.mak

# #################################
# Cleaning
# #################################

clean : cleangcc cleanboost cleanmhash

cleanall : clean cleanallgcc cleanallboost cleanallpostgresql cleanallvalgrind cleanallmhash cleanalldoxygen cleanallubcsat cleanallasciidoc cleanallgit cleanallmpfr cleanallgcc
