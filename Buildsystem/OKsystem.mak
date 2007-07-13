# Oliver Kullmann, 10.4.2005 (Swansea)

# Recursive makefile, to invoke the makefiles for annotations and the OKlibrary

SHELL = /bin/sh
.SUFFIXES :

# The following definitions are temporary: The oklib-masterscript will define OKplatform and OKconfiguration.
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

srcdir := $(OKsystem)

include $(OKbuildsystem)/standardgoals.mak
include $(OKbuildsystem)/recursive_noneincluded.mak

