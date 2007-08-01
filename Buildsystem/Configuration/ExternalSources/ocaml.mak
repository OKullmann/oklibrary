# Oliver Kullmann, 31.7.2007 (Swansea)

# Settings for building and using Ocaml

ocaml_recommended_version_number ?= 3.09.3
ocaml_supported_not_recommended_version_numbers ?= 3.10.0
ocaml_supported_version_numbers ?= $(ocaml_supported_not_recommended_version_numbers) $(ocaml_recommended_version_number)

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

