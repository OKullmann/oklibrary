# Oliver Kullmann, 1.8.2007 (Swansea)

# Tests for external sources.
# To be executed *after* the configuration variables have been set.

# New variables for the configuration of building gcc (to be designed 
# and implemented):

gpp_system_call ?= g++
gcc_system_call ?= gcc

gcc_version_number_extraction := awk '/[0-9]\.[0-9]\.[0-9]/{print $$3}'
# assumes that the output of "gcc --version" contains a line of the form
# (for example) "gcc (GCC) 3.4.3"

location_gpp_system_call ?= $(shell which $(gpp_system_call))
ifeq ($(location_gpp_system_call),)
  gpp_system_call_ready ?= NO
else
  version_gpp_system_call ?= $(shell $(gpp_system_call) --version | $(gcc_version_number_extraction))
  ifeq ($(version_gpp_system_call),$(gcc_recommended_version_number))
    gpp_system_call_ready ?= YES
  else
    gpp_system_call_ready ?= MAYBE
  endif
endif
location_gcc_system_call ?= $(shell which $(gcc_system_call))
ifeq ($(location_gcc_system_call),)
  gcc_system_call_ready ?= NO
else
  version_gcc_system_call ?= $(shell $(gcc_system_call) --version| $(gcc_version_number_extraction))
  ifeq ($(version_gcc_system_call),$(gcc_recommended_version_number))
    gcc_system_call_ready ?= YES
  else
    gcc_system_call_ready ?= MAYBE
  endif
endif

gpp_local_call ?= $(ExternalSources)/Gcc/$(gcc_recommended_version_number)/bin/g++
gcc_local_call ?= $(ExternalSources)/Gcc/$(gcc_recommended_version_number)/bin/gcc

location_gpp_local_call ?= $(shell which $(gpp_local_call))
ifeq ($(location_gpp_local_call),)
  gpp_local_call_ready ?= NO
else
  version_gpp_local_call ?= $(shell $(gpp_local_call) --version | $(gcc_version_number_extraction))
  ifeq ($(version_gpp_local_call),$(gcc_recommended_version_number))
    gpp_local_call_ready ?= YES
  else
    gpp_local_call_ready ?= ERROR
  endif
endif
location_gcc_local_call ?= $(shell which $(gcc_local_call))
ifeq ($(location_gcc_local_call),)
  gcc_local_call_ready ?= NO
else
  version_gcc_local_call ?= $(shell $(gcc_local_call) --version | $(gcc_version_number_extraction))
  ifeq ($(version_gcc_local_call),$(gcc_recommended_version_number))
    gcc_local_call_ready ?= YES
  else
    gcc_local_call_ready ?= ERROR
  endif
endif

# the following construction needs to be generalised by some function
gcc_html_documentation_index_location_tag ?= <a href="$(gcc_html_documentation_index_location)">$(gcc_html_documentation_index_location)</a>

# New variables for the configuration of building doxygen (to be designed 
# and implemented):

doxygen_call ?= doxygen
doxytag_call ?= doxytag

location_doxygen_call ?= $(shell which $(doxygen_call))
ifeq ($(location_doxygen_call),)
  doxygen_call_ready ?= NO
else
  version_doxygen_call ?= $(shell $(doxygen_call) --version)
  ifeq ($(version_doxygen_call),$(doxygen_recommended_version_number))
    doxygen_call_ready ?= YES
  else
    doxygen_call_ready ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
doxygen_html_documentation_index_location_tag ?= <a href="$(doxygen_html_documentation_index_location)">$(doxygen_html_documentation_index_location)</a>

# New variables for the configuration of building ocaml (to be designed 
# and implemented):

ocaml_call ?= $(ocaml_bin_dir)/ocaml

ocaml_version_number_extraction := awk '/ [0-9]+\.[0-9]+(\.[0-9]+)?/{print $$6}'
# assumes that the output of "ocaml -version" contains a line of the form
# (for example) "The Objective Caml toplevel, version 3.10.0"

location_ocaml_call ?= $(shell which $(ocaml_call) 2> /dev/null)
ifeq ($(location_ocaml_call),)
  ocaml_call_ready ?= NO
else
  version_ocaml_call ?= $(shell $(ocaml_call) -version | $(ocaml_version_number_extraction))
  ifeq ($(version_ocaml_call),$(ocaml_recommended_version_number))
    ocaml_call_ready ?= YES
  else
    ocaml_call_ready ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
ocaml_docu_page_tag ?= <a href="$(ocaml_docu_page)">Ocaml installation page</a>


# New variables for the configuration of building coq (to be designed 
# and implemented):

coq_call ?= $(ExternalSources)/Coq/$(coq_recommended_version_number)/bin/coqtop

coq_version_number_extraction := awk '/ [0-9]\.[0-9] /{print $$6}'
# assumes that the output of "coq --version" contains a line of the form
# (for example) "The Coq Proof Assistant, version 8.1 (Feb. 2007)"

location_coq_call ?= $(shell which $(coq_call))
ifeq ($(location_coq_call),)
  coq_call_ready ?= NO
else
  version_coq_call ?= $(shell $(coq_call) --version | $(coq_version_number_extraction))
  ifeq ($(version_coq_call),$(coq_recommended_version_number))
    coq_call_ready ?= YES
  else
    coq_call_ready ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
coq_html_documentation_index_location_tag ?= <a href="$(coq_html_output)">$(coq_html_output)</a>

