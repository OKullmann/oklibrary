# Recursive makefile, to invoke the makefiles for the modules in the OKlibrary

SHELL = /bin/bash
.SUFFIXES :

define last-element
$(word $(words $1),$1)
endef

srcdir := $(shell cd $(dir $(call last-element, $(MAKEFILE_LIST))); pwd)

# The oklib-masterscript definea OKplatform.
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

