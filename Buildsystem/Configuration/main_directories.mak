# Oliver Kullmann, 11.7.2007 (Swansea)

# Here the main directories and files (for the whole system) are specified

ifndef OKplatform
  $(error The make-variable OKplatform must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
endif

ifndef ExternalSources
  ExternalSources = $(OKplatform)/ExternalSources
endif

ifndef system_directories
  ifdef SYSTEMDIRECTORIES
    system_directories := $(SYSTEMDIRECTORIES)
  else
    system_directories := $(OKplatform)/system_directories
  endif
endif

ifndef OKsystem
  OKsystem = $(OKplatform)/OKsystem
endif

ifndef Transitional
  Transitional = $(OKsystem)/Transitional
endif

ifndef OKbuildsystem
  OKbuildsystem = $(Transitional)/Buildsystem
endif

ifndef OKanchor
  OKanchor = $(OKplatform)/.oklib
endif

ifndef OKoverrideconfig
  OKoverrideconfig = $(OKanchor)/override.mak
endif

ifndef Annotations_dir
  Annotations_dir = $(OKsystem)/Annotations
endif
