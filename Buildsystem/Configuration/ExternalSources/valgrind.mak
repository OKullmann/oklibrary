# Oliver Kullmann, 6.9.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Valgrind

valgrind_recommended_version_number ?= 3.3.0
valgrind_supported_not_recommended_version_numbers ?= 3.2.3
# OK: shouldn't this be 3.2.2 ? Or perhaps just empty?
valgrind_supported_version_numbers ?= $(valgrind_supported_not_recommended_version_numbers) $(valgrind_recommended_version_number)

valgrind_prefix ?= valgrind

valgrind_base_directory ?= $(ExternalSources)/Valgrind

valgrind_docu_page ?= $(doc_dir)/doxygen_html/d9/d5f/Valgrind_8hpp.html

valgrind_homepage_url := http://valgrind.org/
valgrind_documentation_url := http://valgrind.org/docs/manual/index.html

valgrind_targets_prefix := $(valgrind_prefix)-
valgrind_targets := $(addprefix $(valgrind_targets_prefix),$(valgrind_supported_version_numbers))
valgrind_recommended := $(valgrind_targets_prefix)$(valgrind_recommended_version_number)


