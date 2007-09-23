# Oliver Kullmann, 14.9.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Helper makefile for package creation (needed to obtain access to the
# configuration data).

SHELL = /bin/bash
.SUFFIXES :

# The oklib-masterscript defines OKplatform.
ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform = $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
  endif
endif
ifndef OKconfiguration
  OKconfiguration = $(OKplatform)/.oklib/Configuration
endif

include $(OKconfiguration)/configuration_data.mak
include $(OKconfiguration)/Html/include.mak
export

# ######################################################################

.PHONY : all

all :
	$(package_build_script)
