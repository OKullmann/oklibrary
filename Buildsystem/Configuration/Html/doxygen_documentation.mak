# Oliver Kullmann, 14.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Specific settings for the doxygen system

doxygen_project_name ?= "OKlibrary:OKlib"
doxygen_project_number_okl ?= $(transitional_version)

doxy_file_template ?= $(OKconfiguration)/Html/Doxyfile
doxy_file ?= $(aux_dir)/Doxyfile
doxygen_error_messages ?= $(log_dir)/DoxygenErrorMessages

doxygen_output_directory ?= $(doc_dir)
doxygen_html_directory ?= doxygen_html
doxygen_html_dir ?= $(doxygen_output_directory)/$(doxygen_html_directory)
OKlibrary_html_documentation_index_location ?= $(doxygen_html_dir)/index.html

doxygen-parameters ?= 
Doxygen_modifier ?= 2> $(doxygen_error_messages)

doxygen_footer_file_okl ?= $(OKconfiguration)/Html/doxygen_footer.html

