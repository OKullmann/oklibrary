# Oliver Kullmann, 31.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Settings for building and using Coq

coq_recommended_version_number ?= 8.1
coq_supported_not_recommended_version_numbers ?=
coq_supported_version_numbers ?= $(coq_supported_not_recommended_version_numbers) $(coq_recommended_version_number)

coq_prefix ?= coq

coq_html_template ?= $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Coq.html
coq_html_output ?= $(local_html_dir)/Coq.html
coq_html_documentation_index_location ?= Coq.html

coq_base_directory ?= $(ExternalSources)/Coq

coq_installation_dir ?= $(coq_base_directory)/$(coq_recommended_version_number)

coq_documentation_dir ?= $(ExternalSources_doc)/Coq/$(coq_recommended_version_number)
coq_docu_page ?= $(doc_dir)/doxygen_html/db/d6c/docus_2Coq_8hpp.html

coq_homepage_url := http://coq.inria.fr/
coq_documentation_url := http://coq.inria.fr/doc-eng.html

coq_targets_prefix := $(coq_prefix)-
coq_targets := $(addprefix $(coq_targets_prefix), $(coq_supported_version_numbers))
coq_recommended := $(coq_targets_prefix)$(coq_recommended_version_number)

