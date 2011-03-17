# Oliver Kullmann, 4.1.2008 (Swansea)
# Copyright 2008, 2009, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# For calling maxima

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

.PHONY : maxima_configuration run_maxima

all : maxima_configuration run_maxima

maxima_configuration :
	$(preprocessing_call) $(OKbuildsystem)/MasterScript/SpecialProcessing/maxima-init.mac > $(maxima_init_okl)
ifeq ($(maxima_lisp_name_okl),ecl)
	$(preprocessing_call) $(OKbuildsystem)/MasterScript/SpecialProcessing/preload.lisp > $(maxima_preload_okl)
endif

run_maxima :
ifeq ($(maxima_lisp_name_okl),ecl)
	HOME=$(maxima_homedir_okl) $(maxima_call_okl) --preload="$(maxima_preload_okl)" $(argument_okl)
else
	HOME=$(maxima_homedir_okl) $(maxima_call_okl) $(argument_okl)
endif
