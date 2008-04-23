# Oliver Kullmann, 14.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Here the directories and files specific for OKlib-building are specified

# Top-level directory for all executables:
bin_dir ?= $(system_directories)/bin

# Top-level directory for all object-files:
lib_dir ?= $(system_directories)/lib

# Top-level directory for all documentation:
doc_dir ?= $(system_directories)/doc

# Top-level directory for all dependency files,
# error, message and log files, and latex auxilliary files:
aux_dir ?= $(system_directories)/aux

# Directory for all latex auxilliary files:
latex_dir ?= $(aux_dir)/latex

# Directory for all dvi files:
dvi_dir ?= $(doc_dir)/dvi

# Directory for all dependency files:
dependencies_dir ?= $(aux_dir)/dependencies

# Directory for all test executables:
test_bin_dir ?= $(bin_dir)/tests

# Directory for current module test object-files:
test_lib_dir = $(lib_dir)/tests/$(module-name)

# Directory for current module test dependency files:
test_aux_dir = $(aux_dir)/tests/$(module-name)

# Directory for Maxima test-files:
maxima_test_aux_dir_okl ?= $(aux_dir)/tests/maxima

# Directory for log-files:
log_dir ?= $(system_directories)/log

# Html directories:
local_html_dir ?= $(doc_dir)/local_html
internet_html_dir ?= $(doc_dir)/internet_html
documents_html_dir ?= $(doc_dir)/documents

# Tools for the build system
tool_dir ?= $(system_directories)/tools

# For documents (including the full documentation):
documents_dir ?= $(OKsystem)/documents

# Links to all main programs
public_bin_dir_okl ?= $(OKplatform)/bin


build_directories = \
               $(bin_dir) \
               $(lib_dir) \
               $(aux_dir) \
               $(latex_dir) \
               $(dependencies_dir) \
               $(doc_dir) \
               $(doxygen_html_dir) \
               $(test_bin_dir) \
               $(test_lib_dir) \
               $(test_aux_dir) \
               $(maxima_test_aux_dir_okl) \
               $(local_html_dir) \
               $(internet_html_dir) \
               $(log_dir) \
               $(tool_dir) \
               $(documents_dir) \
               $(public_bin_dir_okl)

