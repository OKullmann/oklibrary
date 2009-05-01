# Oliver Kullmann, 6.9.2007 (Swansea)
# Copyright 2007, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the Mhash library

# NEEDS UPDATE

mhash_recommended_version_number ?= 0.9.9
mhash_supported_not_recommended_version_numbers ?= 0.9.7.1
mhash_supported_version_numbers ?= $(mhash_supported_not_recommended_version_numbers) $(mhash_recommended_version_number)

mhash_prefix ?= mhash

mhash_base_directory ?= $(ExternalSources)/Installations/Mhash

mhash_docu_page ?= $(doc_dir)/doxygen_html/d4/da3/docus_2Mhash_8hpp.html

mhash_homepage_url := http://mhash.sourceforge.net/
mhash_documentation_url := http://mhash.sourceforge.net/mhash.3.html

mhash_targets_prefix := $(mhash_prefix)-
mhash_targets := $(addprefix $(mhash_targets_prefix), $(mhash_supported_version_numbers))
# for example "mhash-0.9.7.1 mhash-0.9.9"
mhash_recommended := $(mhash_targets_prefix)$(mhash_recommended_version_number)
# for example "mhash-0.9.9"


Mhash = $(mhash_base_directory)/$(mhash_recommended_version_number)
# TODO: We must update and systematise the use of such variables!
