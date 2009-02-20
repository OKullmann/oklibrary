# Oliver Kullmann, 12.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Special makefile for setting up the basic system; does not depend on
# other makefiles

SHELL = /bin/bash
.SUFFIXES :

ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
  endif
endif
ifndef OKanchor
  OKanchor = $(OKplatform)/.oklib
endif
ifndef OKconfiguration
  OKconfiguration = $(OKanchor)/Configuration
endif
ifndef OKoverrideconfig
  OKoverrideconfig = $(OKanchor)/override.mak
endif
ifndef oklib_call
  oklib_call = ~/bin/oklib
endif
ifndef OKlog
  OKlog = $(OKanchor)/log
endif
ifndef oklib_call
  oklib_call = ~/bin/oklib
endif


.PHONY : all oklibrary_initialisation set_link

all : oklibrary_initialisation set_link

oklibrary_initialisation : $(OKanchor) $(OKconfiguration) $(OKoverrideconfig) $(OKlog)

set_link :  $(oklib_call)

$(OKanchor) :
	mkdir $(OKanchor)

$(OKconfiguration) :
	cd $(OKanchor) && ln -s ../OKsystem/OKlib/Buildsystem/Configuration

$(OKoverrideconfig) :
	touch $(OKoverrideconfig)

$(OKlog) :
	cd $(OKanchor) && ln -s ../system_directories/log

$(oklib_call) :
	cd ~/bin; ln -s $(OKconfiguration)/../MasterScript/oklib

