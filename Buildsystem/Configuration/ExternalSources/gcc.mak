# Oliver Kullmann, 26.7.2007 (Swansea)

# Settings for building and using gcc

gcc_recommended_version_number = 4.1.2
gcc_supported_not_recommended_version_numbers = 4.0.1 4.0.2 4.0.3 4.1.0 4.1.1 4.2.0 4.2.1
gcc_old_installation = 4.0.1 4.0.2 4.0.3 4.1.0 4.1.1 4.1.2
gcc_new_installation = 4.2.0 4.2.1
gcc_supported_version_numbers = $(gcc_supported_not_recommended_version_numbers) $(gcc_recommended_version_number)

gcc_prefix = gcc
gcc_html_documentation_index_location = $(ExternalSources_doc)/Gcc/$(gcc_recommended_version_number)/html/gcc/index.html
gcc_docu_page := $(doc_dir)/doxygen_html/d4/dfb/docus_2Gcc_8hpp.html
# OK: is the definition of gcc_docu_page stable?? Apparently not.


gcc_homepage_url := http://gcc.gnu.org/
gcc_documentation_url := http://gcc.gnu.org/onlinedocs/

gcc_targets_prefix := $(gcc_prefix)-
gcc_targets := $(addprefix $(gcc_targets_prefix), $(gcc_supported_version_numbers))
gcc_targets_old := $(addprefix $(gcc_targets_prefix), $(gcc_old_installation))
gcc_targets_new := $(addprefix $(gcc_targets_prefix), $(gcc_new_installation))
# OK: temporary hack! The system must be rethought.
gcc_recommended := $(gcc_targets_prefix)$(gcc_recommended_version_number)

# New variables for the configuration of building doxygen (to be designed 
# and implemented):

gpp_system_call = g++
gcc_system_call = gcc

gcc_version_number_extraction := awk '/[0-9]\.[0-9]\.[0-9]/{print $$3}'
# assumes that the output of "gcc --version" contains a line of the form
# (for example) "gcc (GCC) 3.4.3"

location_gpp_system_call := $(shell which $(gpp_system_call))
ifeq ($(location_gpp_system_call),)
  gpp_system_call_ready = NO
else
  version_gpp_system_call := $(shell $(gpp_system_call) --version | $(gcc_version_number_extraction))
  ifeq ($(version_gpp_system_call),$(gcc_recommended_version_number))
    gpp_system_call_ready = YES
  else
    gpp_system_call_ready = MAYBE
  endif
endif
location_gcc_system_call := $(shell which $(gcc_system_call))
ifeq ($(location_gcc_system_call),)
  gcc_system_call_ready = NO
else
  version_gcc_system_call := $(shell $(gcc_system_call) --version| $(gcc_version_number_extraction))
  ifeq ($(version_gcc_system_call),$(gcc_recommended_version_number))
    gcc_system_call_ready = YES
  else
    gcc_system_call_ready = MAYBE
  endif
endif

gpp_local_call = $(ExternalSources)/Gcc/$(gcc_recommended_version_number)/bin/g++
gcc_local_call = $(ExternalSources)/Gcc/$(gcc_recommended_version_number)/bin/gcc

location_gpp_local_call := $(shell which $(gpp_local_call))
ifeq ($(location_gpp_local_call),)
  gpp_local_call_ready = NO
else
  version_gpp_local_call := $(shell $(gpp_local_call) --version | $(gcc_version_number_extraction))
  ifeq ($(version_gpp_local_call),$(gcc_recommended_version_number))
    gpp_local_call_ready = YES
  else
    gpp_local_call_ready = ERROR
  endif
endif
location_gcc_local_call := $(shell which $(gcc_local_call))
ifeq ($(location_gcc_local_call),)
  gcc_local_call_ready = NO
else
  version_gcc_local_call := $(shell $(gcc_local_call) --version | $(gcc_version_number_extraction))
  ifeq ($(version_gcc_local_call),$(gcc_recommended_version_number))
    gcc_local_call_ready = YES
  else
    gcc_local_call_ready = ERROR
  endif
endif

# the following construction needs to be generalised by some function
gcc_html_documentation_index_location_tag = <a href="$(gcc_html_documentation_index_location)">$(gcc_html_documentation_index_location)</a>

