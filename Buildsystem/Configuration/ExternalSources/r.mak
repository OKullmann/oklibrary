# Oliver Kullmann, 26.7.2007 (Swansea)

# Settings for building and using R

R_recommended_version_number = 2.5.0
R_supported_not_recommended_version_numbers = 
R_supported_version_numbers = $(R_supported_not_recommended_version_numbers) $(R_recommended_version_number)

R_prefix = R

R_html_documentation_index_location = $(ExternalSources_doc)/R/doc/html/index.html
R_docu_page := $(doc_dir)/
# OK: is the definition of R_docu_page stable?? Apparently not.

R_homepage_url := http://www.r-project.org/
R_documentation_url := http://www.r-project.org/

R_targets_prefix := $(R_prefix)-
R_targets := $(addprefix $(R_targets_prefix), $(R_supported_version_numbers))
R_recommended := $(R_targets_prefix)$(R_recommended_version_number)
