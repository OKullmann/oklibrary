# Oliver Kullmann, 10.4.2005 (Swansea)
# Copyright 2005-2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :

# This makefile is only to be invoked by other makefiles.

ifndef OKbuildsystem
  $(error The make-variable OKbuildsystem must be defined!)
endif

# ######################################################################


include $(OKbuildsystem)/standardgoals.mak
include $(OKbuildsystem)/recursive_noneincluded.mak

