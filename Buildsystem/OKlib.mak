# Matthew Henderson, 10.1.2007 (Swansea)
# Recursive makefile, to invoke the makefiles for the modules in the OKlibrary
# Copyright 2005-2007, 2018 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :

define last-element
$(word $(words $1),$1)
endef

srcdir := $(shell cd $(dir $(call last-element, $(MAKEFILE_LIST))); pwd)

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

include $(OKbuildsystem)/Recursive.mak

