ifndef OKBuildsystem
  ifdef OKBUILDSYSTEM
    OKBuildsystem := $(OKBUILDSYSTEM)
  else
    $(error Either OKBuildsystem or OKBUILDSYSTEM must be defined!)
  endif
endif

export

include $(OKBuildsystem)/makefile_generic

Generator : $(core_programs)
