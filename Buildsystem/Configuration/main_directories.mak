# Oliver Kullmann, 11.7.2007 (Swansea)

# Here the main directories and files (for the whole system) are specified

ifndef OKplatform
  $(error The make-variable OKplatform must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
endif

ifndef ExternalSources
  ExternalSources = $(OKplatform)/ExternalSources
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

ifndef OKoverrideconfig
  OKoverrideconfig = $(OKplatform)/.oklib/override.mak
endif

