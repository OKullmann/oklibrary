# Oliver Kullmann, 9.9.2007 (Swansea)
# Copyright 2007, 2009, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Build related variables

OKsystem_include := -I$(OKsystem)

oklib_building_log ?= $(log_dir)/OKlibBuilding
external_sources_log ?= $(log_dir)/ExternalSources

rel_path_tool_origin ?= $(bin_dir)/PathDifference-O3-DNDEBUG
rel_path_tool ?= $(tool_dir)/PathDifference-O3-DNDEBUG
rel_path_tool_debug ?= $(tool_dir)/PathDifference

build_identification_okl ?= $(shell date "+%Y-%j-%s" | tr -d " ")
# This is year-(number of day in the year)-(seconds since 00:00:00 1970-01-01 UTC).

# The list of all external sources:
all_extsrc_okl ?= 

# The number of bits of the machine architecture
ifneq ($(shell uname -m | awk '/64/'),)
  machine_bits_okl ?= 64
else
  machine_bits_okl ?= 32
endif

git_id_oklib ?= $(shell cd $(OKlib); git rev-parse HEAD)
