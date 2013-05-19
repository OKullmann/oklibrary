# Oliver Kullmann, 6.9.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Valgrind

# NEEDS UPDATE

valgrind_recommended_version_number_okl ?= 3.8.1
valgrind_other_versions_okl ?= 3.7.0

valgrind_prefix_okl ?= valgrind

valgrind_recommended_package_name_okl ?= $(valgrind_prefix_okl)-$(valgrind_recommended_version_number_okl)
valgrind_source_okl := $(ExternalSources)/sources/Valgrind/$(valgrind_recommended_package_name_okl)

valgrind_base_build_dir_okl ?= $(ExternalSources_builds)/Valgrind
valgrind_build_dir_okl ?= $(valgrind_base_build_dir_okl)/$(valgrind_recommended_package_name_okl)
valgrind_base_doc_dir_okl ?= $(ExternalSources_doc)/Valgrind
valgrind_doc_dir_okl ?= $(valgrind_base_doc_dir_okl)/$(valgrind_recommended_version_number_okl)

valgrind_doc_dir_build_okl ?= $(valgrind_build_dir_okl)/docs/html
valgrind_html_documentation_index_location_okl ?= $(valgrind_doc_dir_okl)/html/index.html


valgrind_docu_page_okl ?= $(doc_dir)/doxygen_html/d9/d5f/Valgrind_8hpp.html

valgrind_homepage_url_okl := http://valgrind.org/
valgrind_documentation_url_okl := http://valgrind.org/docs/manual/index.html

