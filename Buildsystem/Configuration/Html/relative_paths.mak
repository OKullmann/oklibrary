# Oliver Kullmann, 9.9.2007 (Swansea)

# Here all relative paths, to be used in html pages (except in case of
# links between doxygen pages, which are handled by doxygen itself),
# are defined.

# It is assumed here that all relevant make-variables got their values now.

rel_path_tool ?= $(bin_dir)/PathDifference-O3-DNDEBUG
rel_path_tool_debug ?= $(bin_dir)/PathDifference

# From the local home page:
rel_lindex_OKlibrary_html_documentation_index_location := $(shell $(rel_path_tool) $(OKlibrary_html_documentation_index_location) $(local_home_page_output))
rel_lindex_OKlibrary_full_documentation_dvi_location := $(shell $(rel_path_tool) $(OKlibrary_full_documentation_dvi_location) $(local_home_page_output))
rel_lindex_main_docu_page := $(shell $(rel_path_tool) $(main_docu_page) $(local_home_page_output))

# From the external sources page:
rel_les_boost_docu_page := $(shell $(rel_path_tool) $(boost_docu_page) $(externalsources_page_output))
# to be completed XXX

# From the Internet home page:
rel_ihp_local_home_page_output := $(shell $(rel_path_tool) $(local_home_page_output) $(internet_home_page_output))

