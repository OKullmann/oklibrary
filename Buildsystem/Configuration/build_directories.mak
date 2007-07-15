# Oliver Kullmann, 14.7.2007 (Swansea)

# Here the directories and files specific for OKlib-building are specified

# Top-level directory for all executables:
bin_dir = $(system_directories)/bin

# Top-level directory for all object-files:
lib_dir = $(system_directories)/lib

# Top-level directory for all documentation:
doc_dir = $(system_directories)/doc

# Top-level directory for all dependency files,
# error, message and log files, and latex auxilliary files:
aux_dir = $(system_directories)/aux

# Directory for all latex auxilliary files:
latex_dir = $(aux_dir)/latex

# Directory for all dependency files:
dependencies_dir = $(aux_dir)/dependencies

# Directory for all test executables:
test-bin_dir = $(bin_dir)/tests

# Directory for current module test object-files:
test-lib_dir = $(lib_dir)/tests/$(module-name)

# Directory for current module test dependency files:
test-aux_dir = $(aux_dir)/tests/$(module-name)

build_directories = \
               $(bin_dir) \
               $(lib_dir) \
               $(aux_dir) \
               $(latex_dir) \
               $(dependencies_dir) \
               $(doc_dir) \
               $(doxygen_html_dir) \
               $(test-bin_dir) \
               $(test-lib_dir) \
               $(test-aux_dir) \
               $(local_html_dir)
