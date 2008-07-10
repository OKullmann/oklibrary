# Oliver Kullmann, 9.8.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE

# Settings for building and using Sage

sage_recommended_version_number ?= 2.7.2
sage_supported_not_recommended_version_numbers ?= 2.6
sage_supported_version_numbers ?= $(sage_supported_not_recommended_version_numbers) $(sage_recommended_version_number)

sage_prefix ?= sage

sage_recommended_package_name ?= $(sage_prefix)-$(sage_recommended_version_number)

sage_html_template ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Sage.html
sage_html_output ?= $(local_html_dir)/Sage.html
sage_html_documentation_index_location_okl ?= Sage.html

sage_base_directory ?= $(ExternalSources)/Sage

sage_installation_dir ?= $(sage_base_directory)/$(sage_recommended_package_name)

sage_call_okl ?= $(sage_installation_dir)/sage

sage_main_index ?= $(sage_installation_dir)/doc/index.html
sage_devel_doc ?= $(ExternalSources)/sources/Sage

sage_maxima_dir ?= $(sage_installation_dir)/local/share/maxima
sage_maxima_version ?= $(notdir $(wildcard $(sage_installation_dir)/local/share/maxima/*))
sage_maxima_html ?= $(sage_maxima_dir)/$(sage_maxima_version)/doc/html
sage_xmaxima_html ?= $(sage_maxima_dir)/$(sage_maxima_version)/xmaxima/html
sage_maxima_share ?= $(sage_maxima_dir)/$(sage_maxima_version)/share

sage_gap_version ?= $(subst gap-,,$(notdir $(wildcard $(sage_installation_dir)/local/lib/gap-*)))
sage_guava_version ?= $(subst guava,,$(notdir $(wildcard $(sage_installation_dir)/local/lib/gap-$(sage_gap_version)/pkg/guava*)))
sage_guave_html ?= $(sage_installation_dir)/local/lib/gap-$(sage_gap_version)/pkg/guava$(sage_guava_version)/htm

sage_clisp_dir ?= $(sage_installation_dir)/local/share/doc/clisp
sage_clisp_html ?= $(sage_clisp_dir)/doc

sage_ipython_version ?= $(subst ipython-,,$(notdir $(wildcard $(sage_installation_dir)/local/share/doc/ipython-*)))
sage_ipython_dir ?= $(sage_installation_dir)/local/share/doc/ipython-$(sage_ipython_version)

sage_qd_dir ?= $(sage_installation_dir)/local/share/doc/qd

sage_bzip2_version ?= $(subst bzip2-,,$(notdir $(wildcard $(sage_installation_dir)/spkg/build/bzip2-*)))
sage_bzip2_dir ?= $(sage_installation_dir)/spkg/build/bzip2-$(sage_bzip2_version)

sage_docu_page ?= $(doc_dir)/doxygen_html/d9/dc3/docus_2Sage_8hpp.html

sage_homepage_url := http://modular.math.washington.edu/sage/
sage_documentation_url := http://modular.math.washington.edu/sage/documentation.html

sage_targets_prefix := $(sage_prefix)-
sage_targets := $(addprefix $(sage_targets_prefix), $(sage_supported_version_numbers))
sage_recommended := $(sage_targets_prefix)$(sage_recommended_version_number)

