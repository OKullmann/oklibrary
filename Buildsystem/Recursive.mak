# Oliver Kullmann, 10.4.2005 (Swansea)

SHELL = /bin/bash
.SUFFIXES :

# This makefile is only to be invoked by other makefiles.

ifndef OKbuildsystem
  $(error The make-variable OKbuildsystem must be defined!)
endif

# ######################################################################


include $(OKbuildsystem)/standardgoals.mak
include $(OKbuildsystem)/recursive_noneincluded.mak

