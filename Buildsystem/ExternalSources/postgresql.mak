# Matthew Henderson, 19.7.2006 (Paderborn)
# filename : Buildsystem/ExternalSources/postgresql.mak

# ################################################################
# Original definitions of OKplatform and OKbuildsystem, are 
# in Transtional/Buildsystem/generic.mak and cut-and-pasted
# to :
#  Transitional/Buildsystem/ExternalSources.mak
#  Transitional/Buildsystem/ExternalSources/boost.mak
#  Transitional/Buildsystem/ExternalSources/doxygen.mak
#  Transitional/Buildsystem/ExternalSources/gcc.mak
#  Transitional/Buildsystem/ExternalSources/mhash.mak
#  Transitional/Buildsystem/ExternalSources/postgresql.mak
#  Transitional/Buildsystem/ExternalSources/ubcsat.mak
#  Transitional/Buildsystem/ExternalSources/valgrind.mak
#  Transitional/Buildsystem/makefile
#  Transitional/Buildsystem/OKsystem.mak
#  Transitional/Buildsystem/recursive.mak
#  Transitional/Buildsystem/Transitional.mak
#  Transitional/Buildsystem/Annotations.mak
# ################################################################

ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKsystem
  ifdef OKSYSTEM
    OKsystem := $(OKSYSTEM)
  else
    OKsystem := $(OKplatform)/OKsystem
  endif
endif

ifndef OKbuildsystem
  ifdef OKBUILDSYSTEM
    OKbuildsystem := $(OKBUILDSYSTEM)
  else
    OKbuildsystem := $(OKsystem)/Transitional/Buildsystem
  endif
endif

# ######################################################################

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
postgresql_doc_dir := $(external_sources_doc_base_dir)/Postgresql
postgresql-directories := $(postgresql-base-directory) $(postgresql_doc_dir)


.PHONY : postgresql $(postgresql_targets) initialise-database create_postgresql_dirs

# ####################################
# Documentation
# ####################################

postgresql_doc : | $(postgresql_doc_dir)
	- $(call unarchive,$(postgresql_recommended),$(postgresql_doc_dir))

# ####################################
# The main targets for PostgreSQL
# ####################################

$(postgresql-directories) : % : 
	mkdir $@

create_postgresql_dirs : $(postgresql-directories)

postgresql : $(postgresql_recommended)

ifeq ($(pgsql-version), )
pgsql-version := $(postgresql_recommended_version_number)
endif
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
