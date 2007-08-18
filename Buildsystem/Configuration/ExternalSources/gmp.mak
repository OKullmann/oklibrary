# Oliver Kullmann, 18.8.2007 (Swansea)

# Settings for building and using the Gmp library

gmp_recommended_version_number ?= 4.2.1
gmp_supported_not_recommended_version_numbers ?= 
gmp_supported_version_numbers ?= $(gmp_recommended_version_number) $(gmp_supported_not_recommended_version_numbers)

gmp_prefix ?= gmp
gmp_html_documentation_index_location ?= $(ExternalSources_doc)/Gmp/gmp.html/index.html

gmp_base_directory ?= $(ExternalSources)/Gmp

gmp_documentation_dir ?= $(ExternalSources_doc)/Gmp
gmp_docu_page ?= $(doc_dir)/doxygen_html/d7/d77/docus_2Gmp_8hpp.html

gmp_homepage_url := http://gmplib.org/
gmp_documentation_url := http://gmplib.org/\#DOC

gmp_targets_prefix := $(gmp_prefix)-
gmp_targets := $(addprefix $(gmp_targets_prefix), $(gmp_supported_version_numbers))
gmp_recommended := $(gmp_targets_prefix)$(gmp_recommended_version_number)

