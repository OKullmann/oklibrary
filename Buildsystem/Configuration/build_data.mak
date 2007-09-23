# Oliver Kullmann, 9.9.2007 (Swansea)

# Build related variables

OKsystem_include := -I$(OKsystem)

oklib_building_log ?= $(log_dir)/OKlibBuilding
external_sources_log ?= $(log_dir)/ExternalSources

rel_path_tool_origin ?= $(bin_dir)/PathDifference-O3-DNDEBUG
rel_path_tool ?= $(tool_dir)/PathDifference-O3-DNDEBUG
rel_path_tool_debug ?= $(tool_dir)/PathDifference

