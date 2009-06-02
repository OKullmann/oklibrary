# Oliver Kullmann, 26.1.2008 (Swansea)
# Copyright 2008, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# For calling R

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

.PHONY : R_configuration run_R run_R_restore run_R_core run_R_restore_core

run_R : R_configuration run_R_core
run_R_restore : R_configuration run_R_restore_core

R_configuration :

run_R_core :
ifeq ($(argument_okl),)
	HOME=$(R_homedir_okl) $(R_call_okl) $(R_call_parameters)
else
	HOME=$(R_homedir_okl) $(R_call_okl) $(argument_okl)
endif

run_R_restore_core :
	HOME=$(R_homedir_okl) $(R_call_okl) $(Rrestore_call_parameters_okl)
