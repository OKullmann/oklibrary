# Oliver Kullmann, 19.12.2007 (Swansea)
# Copyright 2007, 2008, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using Clisp

clisp_recommended_version_number_okl ?= 2.49
# on 64-bit machines this needs libffcall
clisp_other_versions_okl ?= 2.48

clisp_prefix_okl ?= clisp

clisp_recommended_package_name_okl ?= $(clisp_prefix_okl)-$(clisp_recommended_version_number_okl)

clisp_html_template_okl ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/CLisp.html
clisp_html_output_okl ?= $(local_html_dir)/CLisp.html
clisp_html_documentation_index_location_okl ?= CLisp.html

clisp_base_installation_dir_okl ?= $(ExternalSources_installations)/CLisp
clisp_installation_dir_okl ?= $(clisp_base_installation_dir_okl)/$(clisp_recommended_version_number_okl)
clisp_base_build_dir_okl ?= $(ExternalSources_builds)/CLisp
clisp_build_dir_okl ?= $(clisp_base_build_dir_okl)/$(clisp_recommended_package_name_okl)
clisp_base_doc_dir_okl ?= $(ExternalSources_doc)/CLisp
clisp_doc_dir_okl ?= $(clisp_base_doc_dir_okl)/$(clisp_recommended_version_number_okl)

clisp_man_html_okl ?= $(clisp_doc_dir_okl)/clisp.html
clisp_man_pdf_okl ?= $(clisp_doc_dir_okl)/clisp.pdf
clisp_tutorial_okl ?= $(clisp_doc_dir_okl)/LISP-tutorial.txt
clisp_class_guide_okl ?= $(clisp_doc_dir_okl)/CLOS-guide.txt
clisp_impnotes_html_okl ?= $(clisp_doc_dir_okl)/impnotes.html

clisp_call_okl ?= $(clisp_installation_dir_okl)/bin/clisp
clisp_lib_okl ?= $(clisp_installation_dir_okl)/lib/clisp-$(clisp_recommended_version_number_okl)/base/lisp.run

clisp_docu_page_okl ?= $(doc_dir)/doxygen_html/df/dfc/CLisp_8hpp.html

clisp_homepage_url_okl := http://www.clisp.org/
clisp_documentation_url_okl := http://clisp.sourceforge.net/resources.html

clisp_recommended_okl := $(clisp_prefix_okl)-$(clisp_recommended_version_number_okl)
clisp_source_okl := $(ExternalSources)/sources/CLisp/$(clisp_recommended_okl)

# Tool: libsigsegv

libsigsegv_recommended_version_number_okl ?= 2.6
libsigsegv_other_versions_okl ?= 2.5

libsigsegv_prefix_okl ?= libsigsegv

libsigsegv_recommended_package_name_okl ?= $(libsigsegv_prefix_okl)-$(libsigsegv_recommended_version_number_okl)

libsigsegv_base_installation_dir_okl ?= $(ExternalSources_installations)/Libsigsegv
libsigsegv_installation_dir_okl ?= $(libsigsegv_base_installation_dir_okl)/$(libsigsegv_recommended_version_number_okl)
libsigsegv_base_build_dir_okl ?= $(clisp_base_build_dir_okl)/Libsigsegv
libsigsegv_build_dir_okl ?= $(libsigsegv_base_build_dir_okl)/$(libsigsegv_recommended_package_name_okl)

libsigsegv_homepage_url_okl := http://libsigsegv.sourceforge.net/

libsigsegv_recommended_okl := $(libsigsegv_prefix_okl)-$(libsigsegv_recommended_version_number_okl)
libsigsegv_source_okl := $(ExternalSources)/sources/CLisp/$(libsigsegv_recommended_okl)

# Tool: libffcall

libffcall_recommended_version_number_okl ?= 1.10
libffcall_other_versions_okl ?=

libffcall_prefix_okl ?= ffcall

libffcall_recommended_package_name_okl ?= $(libffcall_prefix_okl)-$(libffcall_recommended_version_number_okl)

libffcall_base_installation_dir_okl ?= $(ExternalSources_installations)/Libffcall
libffcall_installation_dir_okl ?= $(libffcall_base_installation_dir_okl)/$(libffcall_recommended_version_number_okl)
libffcall_base_build_dir_okl ?= $(clisp_base_build_dir_okl)/Libffcall
libffcall_build_dir_okl ?= $(libffcall_base_build_dir_okl)/$(libffcall_recommended_package_name_okl)

libffcall_homepage_url_okl := http://www.haible.de/bruno/packages-ffcall.html

libffcall_recommended_okl := $(libffcall_prefix_okl)-$(libffcall_recommended_version_number_okl)
libffcall_source_okl := $(ExternalSources)/sources/CLisp/$(libffcall_recommended_okl)

