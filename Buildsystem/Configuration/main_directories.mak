# Oliver Kullmann, 11.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Here the main directories and files (for the whole system) are specified

ExternalSources ?= $(OKplatform)/ExternalSources

system_directories ?= $(OKplatform)/system_directories

OKsystem ?= $(OKplatform)/OKsystem

OKlib ?= $(OKsystem)/OKlib

OKbuildsystem ?= $(OKlib)/Buildsystem

Annotations_dir ?= $(OKsystem)/Annotations

annotations_dir ?= $(OKlib)/annotations

oklib_call ?= $(OKbuildsystem)/MasterScript/oklib

