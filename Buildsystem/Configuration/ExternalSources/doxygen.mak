# Oliver Kullmann, 24.7.2007 (Swansea)

# Settings for building and using doxygen

doxygen_recommended_version_number = 1.5.3
doxygen_supported_not_recommended_version_numbers = 1.5.2
doxygen_supported_version_numbers = $(doxygen_supported_not_recommended_version_numbers) $(doxygen_recommended_version_number)

doxygen_prefix = doxygen
doxygen_full_prefix = $(doxygen_prefix)-$(doxygen_recommended_version_number)
doxygen_recommended_package_name = $(dogygen_full_prefix).src

doxygen_html_documentation_index_location = $(ExternalSources_doc)/Doxygen/$(doxygen_full_prefix)/html/index.html
doxygen_docu_page := $(doc_dir)/doxygen_html/db/dee/Doxygen_8hpp.html
# OK: is the definition of doxygen_docu_page stable?? Apparently not.

doxygen_homepage_url := http://www.stack.nl/~dimitri/doxygen/
doxygen_documentation_url := http://www.stack.nl/~dimitri/doxygen/manual.html

doxygen_targets_prefix := $(doxygen_prefix)-
doxygen_targets := $(addprefix $(doxygen_targets_prefix), $(doxygen_supported_version_numbers))
doxygen_recommended := $(doxygen_targets_prefix)$(doxygen_recommended_version_number)

# New variables for the configuration of building doxygen (to be designed 
# and implemented):

doxygen_call = doxygen
doxytag_call = doxytag

location_doxygen_call := $(shell which $(doxygen_call))
ifeq ($(location_doxygen_call),)
  doxygen_call_ready = NO
else
  version_doxygen_call := $(shell $(doxygen_call) --version)
  ifeq ($(version_doxygen_call),$(doxygen_recommended_version_number))
    doxygen_call_ready = YES
  else
    doxygen_call_ready = MAYBE
  endif
endif

# the following construction needs to be generalised by some function
doxygen_html_documentation_index_location_tag = <a href="$(doxygen_html_documentation_index_location)">$(doxygen_html_documentation_index_location)</a>

