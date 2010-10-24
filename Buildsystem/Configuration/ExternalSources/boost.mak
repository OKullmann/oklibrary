# Oliver Kullmann, 21.8.2007 (Swansea)
# Copyright 2007, 2008, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

boost_recommended_version_number_okl ?= 1_44_0
boost_supported_not_recommended_version_numbers_okl ?= 
boost_supported_version_numbers ?= $(boost_supported_not_recommended_version_numbers) $(boost_recommended_version_number)

boost_base_build_dir_okl ?= $(ExternalSources_builds)/Boost
boost_extracted_package_okl ?= $(boost_base_build_dir_okl)/boost_$(boost_recommended_version_number_okl)
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

boost_source_library_okl ?= $(boost_installation_dir_okl)/include
boost_include_option_okl ?= -I$(boost_source_library_okl)
boost_link_library_okl ?= $(boost_installation_dir_okl)/lib
boost_link_option_okl ?= -L$(boost_link_library_okl) -Wl,-rpath,$(boost_link_library_okl)

ifeq ($(machine_bits_okl),64)
     boost_link_m32_library_okl ?= $(boost_installation_dir_okl)/lib32 
     boost_link_m32_option_okl ?= -L$(boost_link_m32_library_okl) -Wl,-rpath,$(boost_link_m32_library_okl)
else
     boost_link_m32_library_okl ?= $(boost_link_library_okl)
     boost_link_m32_option_okl ?= $(boost_link_option_okl)
endif
