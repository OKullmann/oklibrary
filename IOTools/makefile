ifndef OKSystem
  ifdef OKSYSTEM
    OKSystem := $(OKSYSTEM)
  else
    $(error Either OKSystem or OKSYSTEM must be defined!)
  endif
endif

OKBuildsystem := $(OKSYSTEM)/Transitional/Buildsystem

include $(OKBuildsystem)/makefile_generic
