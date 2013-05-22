# Oliver Kullmann, 23.6.2008 (Swansea)
# Copyright 2008. 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using Asciidoc

asciidoc_recommended_version_number_okl ?= 8.2.5
asciidoc_other_versions_okl ?=
# Remark: version 8.2.6 leads to strange errors about "unsafe" files to be included, and thus can't be used

asciidoc_prefix_okl ?= asciidoc

asciidoc_recommended_okl := $(asciidoc_prefix_okl)-$(asciidoc_recommended_version_number_okl)
asciidoc_source_okl := $(ExternalSources)/sources/Asciidoc/$(asciidoc_recommended_okl)

asciidoc_base_build_dir_okl ?= $(ExternalSources_builds)/Asciidoc
asciidoc_build_dir_okl ?= $(asciidoc_base_build_dir_okl)/$(asciidoc_recommended_okl)

asciidoc_base_doc_dir_okl ?= $(ExternalSources_doc)/Asciidoc
asciidoc_doc_dir_okl ?= $(asciidoc_base_doc_dir_okl)/$(asciidoc_recommended_version_number_okl)

asciidoc_call_okl ?= asciidoc
