# Oliver Kullmann, 5.6.2010 (Swansea)
# Copyright 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# The zlib library (needed by Minisat and derived solvers)

zlib_recommended_version_number_okl ?= 1.2.5
zlib_other_versions_okl ?= 

zlib_prefix_okl ?= zlib

zlib_recommended_okl := $(zlib_prefix_okl)-$(zlib_recommended_version_number_okl)

zlib_base_build_dir_okl ?= $(ExternalSources_builds)/Zlib
zlib_gccbuild_dir_okl ?= $(zlib_base_build_dir_okl)/$(gcc_recommended_version_number_okl)
zlib_build_dir_okl ?= $(zlib_gccbuild_dir_okl)/$(zlib_recommended_okl)

zlib32_base_build_dir_okl ?= $(ExternalSources_builds)/Zlib/32
zlib32_gccbuild_dir_okl ?= $(zlib32_base_build_dir_okl)/$(gcc_recommended_version_number_okl)
zlib32_build_dir_okl ?= $(zlib32_gccbuild_dir_okl)/$(zlib_recommended_okl)

zlib_base_installation_dir_okl ?= $(ExternalSources_installations)/Zlib
zlib_installation_dir_okl ?= $(zlib_base_installation_dir_okl)/$(gcc_recommended_version_number_okl)

zlib32_installation_dir_okl ?= $(zlib_base_installation_dir_okl)/$(gcc_recommended_version_number_okl)/32

zlib_docu_page_okl ?= $(doc_dir)/doxygen_html/

zlib_homepage_url_okl := http://www.zlib.net/
zlib_documentation_url_okl := http://www.zlib.net/manual.html

zlib_source_dir_okl := $(ExternalSources)/sources/SAT/MiniSat
zlib_source_package_okl := $(zlib_source_dir_okl)/$(zlib_recommended_okl)

# Standard
zlib_source_library_okl ?= $(zlib_installation_dir_okl)/include
zlib_include_option_okl ?= -I$(zlib_source_library_okl)
zlib_link_library_okl ?= $(zlib_installation_dir_okl)/lib
zlib_link_option_okl ?= -L$(zlib_link_library_okl) -Wl,-rpath,$(zlib_link_library_okl)
zlib_call_okl ?= $(zlib_installation_dir_okl)/bin/zlib

# 32 bit versions of the library
zlib32_source_library_okl ?= $(zlib32_installation_dir_okl)/include
zlib32_include_option_okl ?= -I$(zlib32_source_library_okl)
zlib32_link_library_okl ?= $(zlib32_installation_dir_okl)/lib
zlib32_link_option_okl ?= -L$(zlib32_link_library_okl) -Wl,-rpath,$(zlib32_link_library_okl)
zlib32_call_okl ?= $(zlib32_installation_dir_okl)/bin/zlib
