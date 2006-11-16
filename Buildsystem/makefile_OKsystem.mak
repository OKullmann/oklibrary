# Oliver Kullmann, 10.4.2005 (Swansea)

SHELL = /bin/sh
.SUFFIXES :

ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKsystem
  ifdef OKSYSTEM
    OKsystem := $(OKSYSTEM)
  else
    OKsystem := $(OKplatform)/OKsystem
  endif
endif

srcdir := $(OKsystem)

OKBuildsystem := $(OKsystem)/Transitional/Buildsystem

export

include $(OKBuildsystem)/makefile_standardgoals.mak

export

include $(OKBuildsystem)/makefile_recursive_noneincluded.mak
