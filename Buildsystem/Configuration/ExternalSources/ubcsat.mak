# Oliver Kullmann, 6.9.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the Ubcsat package

ubcsat_recommended_version_number ?= 1-0-0
ubcsat_supported_but_not_recommended_version_numbers ?= 
ubcsat_supported_version_numbers ?= $(ubcsat_recommended_version_number) $(ubcsat_supported_but_not_recommended_version_numbers)

ubcsat_prefix ?= ubcsat

ubcsat_base_directory ?= $(ExternalSources)/Ubcsat

ubcsat_docu_page ?= $(doc_dir)/doxygen_html/d6/d0c/docus_2Ubcsat_8hpp.html

ubcsat_homepage_url := http://www.satlib.org/ubcsat/
ubcsat_documentation_url := http://www.satlib.org/ubcsat/\#documentation

ubcsat_targets_prefix := $(ubcsat_prefix)-
ubcsat_targets := $(addprefix $(ubcsat_targets_prefix), $(ubcsat_supported_version_numbers))
ubcsat_recommended := $(ubcsat_targets_prefix)$(ubcsat_recommended_version_number)

ifndef Ubcsat
  ifdef UBCSAT
    Ubcsat_dir = $(UBCSAT)
  else
    Ubcsat_dir = $(ExternalSources)/Ubcsat
  endif
  Ubcsat = -I$(Ubcsat_dir)/$(ubcsat_recommended_version_number)/src
endif
# TODO: We must update and systematise the use of such variables.
