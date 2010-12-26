# Matthew Henderson, 19.7.2006 (Paderborn)
# Copyright 2006-2007, 2008, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# NEEDS UPDATE TO NEW SYSTEM

# ####################################
# Directory Structure
# #################################### 

# In the following text, ? denotes the PostgreSql version number.
#./Postgresql : Contain locally installed various versions of PostgreSQL Database System.
#./Postgresql/postgresql-? : This is the original unarchived source directory for postgresql version ?.
#./Postgresql/? : This is the local installation of Postgresql version ?.

pgsql-version :=
pgdata :=
postgresql_base_installation_dir_okl := $(ExternalSources_installations)/Postgresql
postgresql_installation_directory_names := $(patsubst postgresql-%, %, $(postgresql_targets))
postgresql_installation_directory_paths := $(addprefix $(postgresql_base_installation_dir_okl)/,$(postgresql_installation_directory_names))
postgresql_base_doc_dir_okl := $(ExternalSources_doc)/Postgresql
postgresql-directories := $(postgresql_base_installation_dir_okl) $(postgresql_base_doc_dir_okl) $(postgresql_installation_directory_paths)
postgresql_timestamp_prefix := _
postgresql_tag_names:= $(addprefix $(postgresql_timestamp_prefix),$(postgresql_targets))
postgresql_tag_paths := $(addprefix $(postgresql_base_installation_dir_okl)/,$(postgresql_tag_names))



.PHONY : postgresql $(postgresql_targets) initialise-database create_postgresql_dirs

# ####################################
# Documentation
# ####################################

postgresql_doc : | $(postgresql_base_doc_dir_okl)
	- $(call unarchive,$(ExternalSources)/sources/Postgresql/$(postgresql_recommended),$(postgresql_base_doc_dir_okl))
# OK: shall be integrated into building

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
pgdata := $(postgresql_base_installation_dir_okl)/$(pgsql-version)/data
endif

$(postgresql_targets) : $(postgresql_tag_paths)

$(postgresql_tag_paths) : $(postgresql_installation_directory_paths)
	$(call unarchive,$(ExternalSources)/sources/Postgresql/$(addprefix postgresql-,$(notdir $<)),$(postgresql_base_installation_dir_okl))
	cd $(postgresql_base_installation_dir_okl)/$(addprefix postgresql-,$(notdir $<)); $(postcondition) \
	./configure CFLAGS='-Wl,-rpath,$(postgresql_base_installation_dir_okl)/$(notdir $<)/lib' --prefix=$(postgresql_base_installation_dir_okl)/$(notdir $<); $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition) \
	touch $@; $(postcondition) \

initialise-database :
	$(postgresql_base_installation_dir_okl)/$(pgsql-version)/bin/initdb -D $(pgdata)
	$(postcondition)
	$(postgresql_base_installation_dir_okl)/$(pgsql-version)/bin/pg_ctl -D $(pgdata) -l logfile start

# ####################################
# Cleaning
# ####################################

cleanallpostgresql :
	-rm -rf $(postgresql_base_installation_dir_okl)
