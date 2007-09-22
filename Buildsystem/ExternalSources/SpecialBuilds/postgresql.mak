# Matthew Henderson, 19.7.2006 (Paderborn)
# filename : Buildsystem/ExternalSources/postgresql.mak

# ####################################
# Directory Structure
# #################################### 

# In the following text, ? denotes the PostgreSql version number.
#./Postgresql : Contain locally installed various versions of PostgreSQL Database System.
#./Postgresql/postgresql-? : This is the original unarchived source directory for postgresql version ?.
#./Postgresql/? : This is the local installation of Postgresql version ?.

pgsql-version :=
pgdata :=
postgresql-base-directory := $(prefix)/Postgresql
postgresql_installation_directory_names := $(patsubst postgresql-%, %, $(postgresql_targets))
postgresql_installation_directory_paths := $(addprefix $(postgresql-base-directory)/,$(postgresql_installation_directory_names))
postgresql_doc_dir := $(external_sources_doc_base_dir)/Postgresql
postgresql-directories := $(postgresql-base-directory) $(postgresql_doc_dir) $(postgresql_installation_directory_paths)
postgresql_timestamp_prefix := _
postgresql_tag_names:= $(addprefix $(postgresql_timestamp_prefix),$(postgresql_targets))
postgresql_tag_paths := $(addprefix $(postgresql-base-directory)/,$(postgresql_tag_names))



.PHONY : postgresql $(postgresql_targets) initialise-database create_postgresql_dirs

# ####################################
# Documentation
# ####################################

postgresql_doc : | $(postgresql_doc_dir)
	- $(call unarchive,sources/Postgresql/$(postgresql_recommended),$(postgresql_doc_dir))

# ####################################
# The main targets for PostgreSQL
# ####################################

$(postgresql-directories) : % : 
	mkdir -p $@

create_postgresql_dirs : $(postgresql-directories)

postgresql : $(postgresql_recommended)

ifeq ($(pgsql-version), )
pgsql-version := $(postgresql_recommended_version_number)
endif
ifeq ($(pgdata), )
pgdata := $(postgresql-base-directory)/$(pgsql-version)/data
endif

$(postgresql_targets) : $(postgresql_tag_paths)

$(postgresql_tag_paths) : $(postgresql_installation_directory_paths)
	$(call unarchive,sources/Postgresql/$(addprefix postgresql-,$(notdir $<)),$(postgresql-base-directory))
	cd $(postgresql-base-directory)/$(addprefix postgresql-,$(notdir $<)); $(postcondition) \
	./configure CFLAGS='-Wl,-rpath,$(postgresql-base-directory)/$(notdir $<)/lib' --prefix=$(postgresql-base-directory)/$(notdir $<); $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition) \
	touch $@; $(postcondition) \

initialise-database :
	$(postgresql-base-directory)/$(pgsql-version)/bin/initdb -D $(pgdata)
	$(postcondition)
	$(postgresql-base-directory)/$(pgsql-version)/bin/pg_ctl -D $(pgdata) -l logfile start

# ####################################
# Cleaning
# ####################################

cleanallpostgresql :
	-rm -rf $(postgresql-base-directory)
