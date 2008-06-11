# Oliver Kullmann, 26.7.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using gcc

gcc_recommended_version_number ?= 4.1.2
gcc_supported_not_recommended_version_numbers ?= 4.2.4 4.3.1
gcc_old_installation ?= 4.1.2
gcc_new_installation ?= 4.2.4 4.3.1
gcc_supported_version_numbers ?= $(gcc_supported_not_recommended_version_numbers) $(gcc_recommended_version_number)

gcc_prefix ?= gcc

gcc_base_directory ?= $(ExternalSources)/Gcc

gcc_documentation_dir ?= $(ExternalSources_doc)/Gcc
gcc_html_documentation_index_location ?= $(gcc_documentation_dir)/$(gcc_recommended_version_number)/html/gcc/index.html
gcc_docu_page ?= $(doc_dir)/doxygen_html/d4/dfb/docus_2Gcc_8hpp.html

gcc_homepage_url := http://gcc.gnu.org/
gcc_documentation_url := http://gcc.gnu.org/onlinedocs/

gcc_targets_prefix := $(gcc_prefix)-
gcc_targets := $(addprefix $(gcc_targets_prefix), $(gcc_supported_version_numbers))
gcc_targets_old := $(addprefix $(gcc_targets_prefix), $(gcc_old_installation))
gcc_targets_new := $(addprefix $(gcc_targets_prefix), $(gcc_new_installation))
# OK: temporary hack! The system must be rethought.
gcc_recommended := $(gcc_targets_prefix)$(gcc_recommended_version_number)

