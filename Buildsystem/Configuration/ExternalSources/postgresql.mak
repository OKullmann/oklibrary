# #################################
# Postgresql
# #################################

postgresql_recommended_version_number ?= 8.2.4
postgresql_supported_not_recommended_version_numbers ?= 
postgresql_supported_version_numbers ?= $(postgresql_supported_not_recommended_version_numbers) $(postgresql_recommended_version_number)

postgresql_prefix ?= postgresql
pgsql_html_documentation_index_location ?= $(ExternalSources_doc)/Postgresql/doc/postgresql/html/index.html

postgresql_base_directory ?= $(ExternalSources)/Postgresql

postgresql_documentation_dir ?= $(ExternalSources_doc)/Postgresql
postgresql_docu_page ?= $(doc_dir)/doxygen_html/d7/dbe/Postgresql_8hpp.html

pgsql_homepage_url := http://www.postgresql.org/
pgsql_documentation_url := http://www.postgresql.org/docs/

postgresql_targets_prefix := $(postgresql_prefix)-
postgresql_targets := $(addprefix $(postgresql_targets_prefix), $(postgresql_supported_version_numbers))
postgresql_recommended := $(postgresql_targets_prefix)$(postgresql_recommended_version_number)

