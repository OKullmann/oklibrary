# Oliver Kullmann, 21.8.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

boost_recommended_version_number_okl ?= 1_52_0
boost_other_versions_okl ?= 1_50_0

boost_base_build_dir_okl ?= $(ExternalSources_builds)/Boost
boost_extracted_package_okl ?= $(boost_base_build_dir_okl)/boost_$(boost_recommended_version_number_okl)
boost_bjamsrc_okl ?= $(boost_extracted_package_okl)/tools/build/v2/engine
boost_build_dir_okl ?= $(boost_base_build_dir_okl)/$(gcc_recommended_version_number_okl)/$(boost_recommended_version_number_okl)
boost_base_installation_dir_okl ?= $(ExternalSources_installations)/Boost
boost_installation_dir_okl ?= $(boost_base_installation_dir_okl)/$(gcc_recommended_version_number_okl)/$(boost_recommended_version_number_okl)
boost_base_doc_dir_okl ?= $(ExternalSources_doc)/Boost
boost_doc_dir_okl ?= $(boost_base_doc_dir_okl)/$(boost_recommended_version_number_okl)

boost_html_documentation_index_location_okl ?= $(boost_doc_dir_okl)/index.htm
boost_docu_page_okl ?= $(doc_dir)/doxygen_html/d3/d24/docus_2Boost_8hpp.html

boost_homepage_url_okl := http://www.boost.org/
boost_documentation_url_okl := http://www.boost.org/libs/libraries.htm

boost_source_dir_okl := $(ExternalSources)/sources/Boost/boost_$(boost_recommended_version_number_okl)

boost_configuration_okl ?= --without-python --without-wave --without-mpi

boost_source_library_okl ?= $(boost_installation_dir_okl)/include
boost_include_option_okl ?= -I$(boost_source_library_okl)
boost_link_library_okl ?= $(boost_installation_dir_okl)/lib
boost_link_option_okl ?= -L$(boost_link_library_okl) -Wl,-rpath,$(boost_link_library_okl)

# as long as the OKlibrary is built using gcc-4.1.2 (to be removed after that):
boost_include_option_okl := -I $(boost_base_installation_dir_okl)/4.1.2/$(boost_recommended_version_number_okl)/include
boost_link_option_okl := -L $(boost_base_installation_dir_okl)/4.1.2/$(boost_recommended_version_number_okl)/lib -Wl,-rpath,$(boost_base_installation_dir_okl)/4.1.2/$(boost_recommended_version_number_okl)/lib
