SHELL = /bin/sh
.SUFFIXES :

# ################################################################
# Original definitions of OKplatform and OKbuildsystem, are 
# in Transtional/Buildsystem/generic.mak and cut-and-pasted
# to :
#  Transitional/Buildsystem/ExternalSources.mak
#  Transitional/Buildsystem/makefile
#  Transitional/Buildsystem/OKsystem.mak
#  Transitional/Buildsystem/recursive.mak
#  Transitional/Buildsystem/Transitional.mak
#  Transitional/Buildsystem/Annotations.mak
# ################################################################

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

ifndef OKbuildsystem
  ifdef OKBUILDSYSTEM
    OKbuildsystem := $(OKBUILDSYSTEM)
  else
    OKbuildsystem := $(OKsystem)/Transitional/Buildsystem
  endif
endif

# ######################################################################

define last-element
$(word $(words $1),$1)
endef

srcdir := $(shell cd $(dir $(call last-element, $(MAKEFILE_LIST))); pwd)

export

include $(OKbuildsystem)/recursive.mak

