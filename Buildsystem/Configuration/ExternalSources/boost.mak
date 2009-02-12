# Oliver Kullmann, 21.8.2007 (Swansea)
# Copyright 2007, 2008, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# Settings for building and using the Boost library

boost_recommended_version_number ?= 1_38_0
boost_supported_not_recommended_version_numbers ?= 1_34_1
boost_supported_version_numbers ?= $(boost_supported_not_recommended_version_numbers) $(boost_recommended_version_number)

boost_prefix ?= boost

boost_base_directory ?= $(ExternalSources)/Boost

boost_documentation_dir ?= $(ExternalSources_doc)/Boost
boost_recommended_package_name ?= $(boost_recommended_version_number)
# OK: The variable boost_recommended_package_name seems wrong, and seems
# to be used only for setting boost_html_documentation_index_location ??
boost_html_documentation_index_location ?= $(boost_documentation_dir)/$(boost_recommended_package_name)/index.htm
boost_docu_page ?= $(doc_dir)/doxygen_html/d3/d24/docus_2Boost_8hpp.html

boost_homepage_url := http://www.boost.org/
boost_documentation_url := http://www.boost.org/libs/libraries.htm

boost_targets_prefix := $(boost_prefix)-
boost_prefix_underscore := $(boost_prefix)_
# OK: where do we use boost_prefix_underscore ??
boost_targets := $(addprefix $(boost_targets_prefix), $(boost_supported_version_numbers))
boost_recommended := $(boost_targets_prefix)$(boost_recommended_version_number)

ifndef Boost
  ifdef BOOST
    Boost = -I$(BOOST)
  else
    Boost = -I/usr/local/$(boost_recommended)
  endif
endif
# TODO: We must update and systematise the use of such variables!

