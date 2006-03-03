
ifndef OKPlatform
  ifdef OKPLATFORM
    OKPlatform := $(OKPLATFORM)
  else
    $(error Either OKPlatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKSystem
  ifdef OKSYSTEM
    OKSystem := $(OKSYSTEM)
  else
    OKSystem := $(OKPlatform)/OKsystem
  endif
endif

OKBuildsystem := $(OKSystem)/Transitional/Buildsystem

include $(OKBuildsystem)/makefile_generic.mak 
