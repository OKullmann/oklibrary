# Oliver Kullmann, 11.7.2007 (Swansea)

# This makefile provides all configuration variables (except for html purposes)

ifndef OKplatform
  $(error The make-variable OKplatform must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
endif

OKanchor ?= $(OKplatform)/.oklib
OKoverrideconfig ?= $(OKanchor)/override.mak
OKlog ?= $(OKanchor)/log

include $(OKoverrideconfig)

ifndef OKconfiguration
  $(error The make-variable OKconfiguration must be defined as the directory path for the configuration directory (default is a symbolic link to Transitional/Buildsystem/Configuration in OKplatform/.oklib)!)
endif

include $(OKconfiguration)/main_directories.mak
include $(OKconfiguration)/version_numbers.mak
include $(OKconfiguration)/ExternalSources/all.mak
include $(OKconfiguration)/build_directories.mak
include $(OKconfiguration)/build_data.mak
include $(OKconfiguration)/macro_replacement.mak
include $(OKconfiguration)/ReleaseProcess/data.mak

include $(OKconfiguration)/ExternalSources/tests.mak

