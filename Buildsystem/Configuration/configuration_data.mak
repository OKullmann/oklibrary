# Oliver Kullmann, 11.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# This makefile provides all configuration variables (except for html purposes)

ifndef OKplatform
  $(error The make-variable OKplatform must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
endif

OKanchor ?= $(OKplatform)/.oklib
OKoverrideconfig ?= $(OKanchor)/override.mak
OKlog ?= $(OKanchor)/log

include $(OKoverrideconfig)

ifndef OKconfiguration
  $(error The make-variable OKconfiguration must be defined as the directory path for the configuration directory (default is a symbolic link to OKlib/Buildsystem/Configuration in OKplatform/.oklib)!)
endif

include $(OKconfiguration)/main_directories.mak
include $(OKconfiguration)/version_numbers.mak
include $(OKconfiguration)/build_directories.mak
include $(OKconfiguration)/build_data.mak
include $(OKconfiguration)/ExternalSources/all.mak # Must be lower than build_data (requires machine_bits_okl)
include $(OKconfiguration)/macro_replacement.mak
include $(OKconfiguration)/ReleaseProcess/data.mak
include $(OKconfiguration)/SourceControl/data.mak
include $(OKconfiguration)/public_links.mak

include $(OKconfiguration)/functions.mak
