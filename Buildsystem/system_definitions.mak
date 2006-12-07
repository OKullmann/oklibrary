ifndef ExternalSources
  ifdef EXTERNALSOURCES
    ExternalSources := $(EXTERNALSOURCES)
  else
    ExternalSources := $(OKplatform)/ExternalSources
  endif
endif

ifndef OKsystem
  ifdef OKSYSTEM
    OKsystem := $(OKSYSTEM)
  else
    OKsystem := $(OKplatform)/OKsystem
  endif
endif

OKsystem_include := -I$(OKsystem)

Annotations_dir := $(OKsystem)/Annotations

ifndef Boost
  ifdef BOOST
    Boost = -I$(BOOST)
  else
    Boost = -I/usr/local/boost-1_33_1
  endif
endif

ifndef system_directories
  ifdef SYSTEMDIRECTORIES
    system_directories := $(SYSTEMDIRECTORIES)
  else
    system_directories := $(OKplatform)/system_directories
  endif
endif
