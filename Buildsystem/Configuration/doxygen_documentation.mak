# Oliver Kullmann, 14.7.2007 (Swansea)

# Specific settings for the doxygen system

doxygen_project_name = "OKlibrary:Transitional (version $(transitional_version))"

doxy_file_template = $(OKconfiguration)/Doxyfile
doxy_file = $(aux_dir)/Doxyfile
doxygen_error_messages = $(aux_dir)/DoxygenErrorMessages

doxygen_output_directory = $(doc_dir)
doxygen_html_directory = doxygen_html
doxygen_html_dir = $(doxygen_output_directory)/$(doxygen_html_directory)
OKlibrary_html_documentation_index_location = $(doxygen_html_dir)/index.html

doxygen-parameters = 
Doxygen_modifier = 2> $(doxygen_error_messages)

