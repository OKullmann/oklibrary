# Oliver Kullmann, 24.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using doxygen

doxygen_recommended_version_number ?= 1.5.3
doxygen_supported_not_recommended_version_numbers ?= 
doxygen_supported_version_numbers ?= $(doxygen_supported_not_recommended_version_numbers) $(doxygen_recommended_version_number)

doxygen_prefix ?= doxygen
doxygen_full_prefix ?= $(doxygen_prefix)-$(doxygen_recommended_version_number)
doxygen_recommended_package_name ?= $(dogygen_full_prefix).src

doxygen_base_directory ?= $(ExternalSources)/Doxygen

doxygen_documentation_dir ?= $(ExternalSources_doc)/Doxygen
doxygen_html_documentation_index_location ?= $(doxygen_documentation_dir)/$(doxygen_full_prefix)/html/index.html
doxygen_docu_page ?= $(doc_dir)/doxygen_html/db/dee/Doxygen_8hpp.html

doxygen_homepage_url := http://www.stack.nl/~dimitri/doxygen/
doxygen_documentation_url := http://www.stack.nl/~dimitri/doxygen/manual.html

doxygen_targets_prefix := $(doxygen_prefix)-
doxygen_targets := $(addprefix $(doxygen_targets_prefix), $(doxygen_supported_version_numbers))
doxygen_recommended := $(doxygen_targets_prefix)$(doxygen_recommended_version_number)


