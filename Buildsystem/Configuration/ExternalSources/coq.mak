# Oliver Kullmann, 31.7.2007 (Swansea)

# Settings for building and using Coq

coq_recommended_version_number = 8.1
coq_supported_not_recommended_version_numbers =
coq_supported_version_numbers = $(coq_supported_not_recommended_version_numbers) $(coq_recommended_version_number)

coq_prefix = coq
coq_html_template = $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Coq.html
coq_html_output = $(local_html_dir)/Coq.html
coq_html_documentation_index_location = Coq.html

coq_base_directory = $(ExternalSources)/Coq

coq_documentation_dir = $(ExternalSources_doc)/Coq/$(coq_recommended_version_number)
coq_docu_page = $(doc_dir)/doxygen_html/db/d6c/docus_2Coq_8hpp.html

coq_homepage_url := 
coq_documentation_url := 

coq_targets_prefix := $(coq_prefix)-
coq_targets := $(addprefix $(coq_targets_prefix), $(coq_supported_version_numbers))
coq_recommended := $(coq_targets_prefix)$(coq_recommended_version_number)

# New variables for the configuration of building coq (to be designed 
# and implemented):

coq_call = $(ExternalSources)/Coq/$(coq_recommended_version_number)/bin/coqtop

coq_version_number_extraction := awk '/ [0-9]\.[0-9] /{print $$6}'
# assumes that the output of "coq --version" contains a line of the form
# (for example) "The Coq Proof Assistant, version 8.1 (Feb. 2007)"

location_coq_call := $(shell which $(coq_call))
ifeq ($(location_coq_call),)
  coq_call_ready = NO
else
  version_coq_call := $(shell $(coq_call) --version | $(coq_version_number_extraction))
  ifeq ($(version_coq_call),$(coq_recommended_version_number))
    coq_call_ready = YES
  else
    coq_call_ready = MAYBE
  endif
endif

# the following construction needs to be generalised by some function
coq_html_documentation_index_location_tag = <a href="$(coq_html_output)">$(coq_html_output)</a>

