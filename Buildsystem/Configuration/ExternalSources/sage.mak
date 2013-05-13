# Oliver Kullmann, 9.8.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011, 2012, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using Sage

sage_recommended_version_number_okl ?= 5.9
sage_other_versions_okl ?= 5.3

sage_prefix_okl ?= sage

sage_base_installation_dir_okl ?= $(ExternalSources_installations)/Sage
sage_installation_dir_okl ?= $(sage_base_installation_dir_okl)/$(sage_recommended_version_number_okl)

sage_call_okl ?= $(sage_installation_dir_okl)/sage
sage_datadir_okl ?= $(sage_base_installation_dir_okl)/Sage_data

sage_main_index_okl ?= $(sage_installation_dir_okl)/devel/sage-main/doc/output/html/en/index.html

sage_docu_page_okl ?= $(doc_dir)/doxygen_html/d9/dc3/docus_2Sage_8hpp.html

sage_homepage_url_okl := http://www.sagemath.org/
sage_documentation_url_okl := http://sage.math.washington.edu/sage/doc/

sage_recommended_okl := $(sage_prefix_okl)-$(sage_recommended_version_number_okl)
sage_source_dir_okl := $(ExternalSources)/sources/Sage
sage_source_okl := $(sage_source_dir_okl)/$(sage_recommended_okl)

