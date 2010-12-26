# Oliver Kullmann, 6.9.2007 (Swansea)
# Copyright 2007, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using the Mhash library

mhash_recommended_version_number_okl ?= 0.9.9.9
mhash_other_versions_okl ?= 0.9.9

mhash_prefix_okl ?= mhash

mhash_targets_prefix_okl := $(mhash_prefix_okl)-
mhash_recommended_okl := $(mhash_targets_prefix_okl)$(mhash_recommended_version_number_okl)
mhash_source_dir_okl := $(ExternalSources)/sources/Mhash/$(mhash_recommended_okl)
mhash_source_doc_okl := $(ExternalSources)/sources/Mhash/mhash.3.html

mhash_base_build_dir_okl ?= $(ExternalSources_builds)/Mhash
mhash_gccbuild_dir_okl ?= $(mhash_base_build_dir_okl)/$(gcc_recommended_version_number_okl)
mhash_build_dir_okl ?= $(mhash_gccbuild_dir_okl)/$(mhash_recommended_okl)

mhash_base_installation_dir_okl ?= $(ExternalSources_installations)/Mhash
mhash_installation_dir_okl ?= $(mhash_base_installation_dir_okl)/$(gcc_recommended_version_number_okl)/$(mhash_recommended_version_number_okl)

mhash_link_option_okl ?= -L $(mhash_installation_dir_okl)/lib -Wl,-rpath,$(mhash_installation_dir_okl)/lib -lmhash
mhash_source_library_okl ?= $(mhash_installation_dir_okl)/include
mhash_include_option_okl ?= -I $(mhash_source_library_okl)

mhash_base_doc_dir_okl ?= $(ExternalSources_doc)/Mhash
mhash_doc_dir_okl ?= $(mhash_base_doc_dir_okl)/$(mhash_recommended_version_number_okl)
mhash_html_documentation_index_location_okl ?= $(mhash_doc_dir_okl)/mhash.html
mhash_docu_page_okl ?= $(doc_dir)/doxygen_html/d4/da3/docus_2Mhash_8hpp.html

mhash_homepage_url_okl := http://mhash.sourceforge.net/
mhash_documentation_url_okl := http://mhash.sourceforge.net/mhash.3.html

