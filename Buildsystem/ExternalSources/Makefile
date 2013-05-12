# Oliver Kullmann, 13.6.2005 (Swansea)
# Copyright 2005-2008, 2009, 2010, 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

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

# #################################
# Main targets
# #################################

.PHONY : $(global_targets_extsrc_okl) 

all : compilers libraries math sat boolean buildsystem math2

compilers : $(compiler_targets_extsrc_okl)
libraries : $(library_targets_extsrc_okl)
math : $(math_targets_extsrc_okl)
math2 : $(math2_targets_extsrc_okl)
sat : $(sat_targets_extsrc_okl)
boolean : $(boolean_targets_extsrc_okl)
buildsystem : $(buildsystem_targets_extsrc_okl)

# #################################
# Functions
# #################################

define unarchive_uncompressed
if [ -f $(1).tar ]; then tar --extract --directory=$(2) --file=$(1).tar; else exit 1; fi;
endef

define unarchive
if [ -f $(1).tar.gz ]; then tar --extract --directory=$(2) --file=$(1).tar.gz --ungzip; elif \
[ -f $(1).tgz ]; then tar --extract --directory=$(2) --file=$(1).tgz --ungzip; elif \
[ -f $(1).tar.bz2 ]; then tar --extract --directory=$(2) --file=$(1).tar.bz2 --bzip2; else exit 1; fi;
endef


# #################################
# Updating
# #################################

define full_name
$($(1)_source_okl)$($(1)_source_ext_okl)
endef

all_update_targets_okl := $(addprefix update_, $(all_extsrc_okl))
.PHONY : $(all_update_targets_okl)

update : $(all_update_targets_okl)

$(all_update_targets_okl) : update_% :
	@ if [[ -f $(call full_name,$*) ]]; then echo "$(call full_name,$*) is up-to-date."; exit; else echo "We are now downloading $(call full_name,$*)."; fi; \
	echo "Downloading!";


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
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/R.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/mpfr.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/gmp.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/sage.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/ocaml.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/coq.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/clisp.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/ecl.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/maxima.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/gap.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/grasp.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/minisat.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/march.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/satz.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/sp.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/bzip2.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/picosat.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/precosat.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/argosat.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/zlib.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/espresso.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/cryptominisat.mak
include $(OKbuildsystem)/ExternalSources/SpecialBuilds/glucose.mak


# #################################
# Cleaning
# #################################

clean :
	-rm -rf $(ExternalSources_builds)

cleanall : clean
	-rm -rf $(ExternalSources_doc) $(ExternalSources_installations)

