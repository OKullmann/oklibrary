# Matthew Henderson, 19.7.2006 (Paderborn)
# filename : Buildsystem/ExternalSources/makefile_postgresql.mak

# ####################################
# Usage
# ####################################

# PostgreSQL

# make postgresql (recommended version)
# make postgresql-?

# Initialise a database and start the server (? is version number of postgresql):
# Use the default location (postgresql-base-directory/?/data):
# 	make pgsql-version=? initialise-database
# Use a different location (% is the absolute path of the location to place the database):
# 	make pgsql-version=? pgdata=% initialise-database

# ####################################
# Targets
# ####################################

include $(OKbuildsystem)/external_sources_versions.mak

# ####################################
# Directory Structure
# #################################### 

#In the following text, ? denotes the PostgreSql version number.
#./Postgresql : Contain locally installed various versions of PostgreSQL Database System.
#./Postgresql/postgresql-? : This is the original unarchived source directory for postgresql version ?.
#./Postgresql/? : This is the local installation of Postgresql version ?.

pgsql-version :=
pgdata :=
postgresql-base-directory := $(prefix)/Postgresql
postgresql-directories := $(postgresql-base-directory)

.PHONY : postgresql $(postgresql_targets) initialise-database create_postgresql_dirs

# ####################################
# The main targets for PostgreSQL
# ####################################

$(postgresql-directories) : % : 
	mkdir $@

create_postgresql_dirs : $(postgresql-directories)

postgresql : $(postgresql_recommended)

ifeq ($(pgdata), )
pgdata := $(postgresql-base-directory)/$(pgsql-version)/data
endif

$(postgresql_targets) : create_postgresql_dirs
	$(call unarchive,$@,$(postgresql-base-directory))
	cd $(postgresql-base-directory)/$@; $(postcondition) \
	./configure CFLAGS='-Wl,-rpath,$(postgresql-base-directory)/$*/lib' --prefix=$(postgresql-base-directory)/$*; $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition) \

initialise-database :
	$(postgresql-base-directory)/$(pgsql-version)/bin/initdb -D $(pgdata)
	$(postcondition)
	$(postgresql-base-directory)/$(pgsql-version)/bin/pg_ctl -D $(pgdata) -l logfile start

# ####################################
# Cleaning
# ####################################

cleanallpostgresql :
	-rm -rf $(postgresql-base-directory)
