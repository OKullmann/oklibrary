# Oliver Kullmann, 31.7.2007 (Swansea)
# Copyright 2007, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE TO NEW SYSTEM

# Settings for building and using Ocaml

ocaml_recommended_version_number ?= 3.09.3
ocaml_other_versions_okl ?= 3.10.0
ocaml_supported_version_numbers ?= $(ocaml_other_versions_okl) $(ocaml_recommended_version_number)

ocaml_prefix ?= ocaml

ocaml_base_directory ?= $(coq_base_directory)/Ocaml
ocaml_installation_dir ?= $(ocaml_base_directory)/$(ocaml_recommended_version_number)
ocaml_bin_dir ?= $(ocaml_installation_dir)/bin
ocaml_lib_dir ?= $(ocaml_installation_dir)/lib
ocaml_man_dir ?= $(ocaml_installation_dir)/man

ocaml_docu_page ?= $(doc_dir)/doxygen_html/d8/da7/Ocaml_8hpp.html

ocaml_homepage_url := 
ocaml_documentation_url := 

ocaml_targets_prefix := $(ocaml_prefix)-
ocaml_targets := $(addprefix $(ocaml_targets_prefix), $(ocaml_supported_version_numbers))
ocaml_recommended := $(ocaml_targets_prefix)$(ocaml_recommended_version_number)

