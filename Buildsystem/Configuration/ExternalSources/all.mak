# Matthew Henderson, 30.10.2006 (Swansea)

# Configuration variables regarding installation and documentation of external sources

ExternalSources_doc ?= $(ExternalSources)/doc
ExternalSources_sources ?= $(ExternalSources)/sources

include $(OKconfiguration)/ExternalSources/doxygen.mak
include $(OKconfiguration)/ExternalSources/gcc.mak
include $(OKconfiguration)/ExternalSources/r.mak
include $(OKconfiguration)/ExternalSources/ocaml.mak
include $(OKconfiguration)/ExternalSources/coq.mak
include $(OKconfiguration)/ExternalSources/sage.mak
include $(OKconfiguration)/ExternalSources/git.mak
include $(OKconfiguration)/ExternalSources/gmp.mak
include $(OKconfiguration)/ExternalSources/boost.mak

# #################################
# Mhash
# #################################

mhash_recommended_version_number ?= 0.9.9
mhash_supported_not_recommended_version_numbers ?= 0.9.7.1
mhash_supported_version_numbers ?= $(mhash_supported_not_recommended_version_numbers) $(mhash_recommended_version_number)

mhash_prefix ?= mhash

mhash_homepage_url := http://mhash.sourceforge.net/
mhash_documentation_url := http://mhash.sourceforge.net/mhash.3.html

mhash_targets_prefix := $(mhash_prefix)-
mhash_targets := $(addprefix $(mhash_targets_prefix), $(mhash_supported_version_numbers))
# for example "mhash-0.9.7.1 mhash-0.9.9"
mhash_recommended := $(mhash_targets_prefix)$(mhash_recommended_version_number)
# for example "mhash-0.9.9"

ifndef Mhash
  ifdef MHASH
    Mhash_dir = $(MHASH)
  else
    Mhash_dir = $(ExternalSources)/Mhash
  endif
  Mhash = $(Mhash_dir)/$(mhash_recommended_version_number)
endif
# TODO: We must update and systematise the use of such variables!

# #################################
# Postgresql
# #################################

postgresql_recommended_version_number ?= 8.2.4
postgresql_supported_not_recommended_version_numbers ?= 
postgresql_supported_version_numbers ?= $(postgresql_supported_not_recommended_version_numbers) $(postgresql_recommended_version_number)

postgresql_prefix ?= postgresql
pgsql_html_documentation_index_location ?= $(ExternalSources_doc)/Postgresql/doc/postgresql/html/index.html

pgsql_homepage_url := http://www.postgresql.org/
pgsql_documentation_url := http://www.postgresql.org/docs/

postgresql_targets_prefix := $(postgresql_prefix)-
postgresql_targets := $(addprefix $(postgresql_targets_prefix), $(postgresql_supported_version_numbers))
postgresql_recommended := $(postgresql_targets_prefix)$(postgresql_recommended_version_number)

# #################################
# Valgrind
# #################################

valgrind_recommended_version_number ?= 3.2.3
valgrind_supported_not_recommended_version_numbers ?= 3.2.1
# OK: shouldn't this be 3.2.2 ? Or perhaps just empty?
valgrind_supported_version_numbers ?= $(valgrind_supported_not_recommended_version_numbers) $(valgrind_recommended_version_number)

valgrind_prefix ?= valgrind

valgrind_homepage_url := http://valgrind.org/
valgrind_documentation_url := http://valgrind.org/docs/manual/index.html

valgrind_targets_prefix := $(valgrind_prefix)-
valgrind_targets := $(addprefix $(valgrind_targets_prefix),$(valgrind_supported_version_numbers))
valgrind_recommended := $(valgrind_targets_prefix)$(valgrind_recommended_version_number)

# ################################
# UBCSAT
# ###############################

ubcsat_recommended_version_number ?= 1-0-0
ubcsat_supported_but_not_recommended_version_numbers ?= 
ubcsat_supported_version_numbers ?= $(ubcsat_recommended_version_number) $(ubcsat_supported_but_not_recommended_version_numbers)

ubcsat_prefix ?= ubcsat

ubcsat_homepage_url := http://www.satlib.org/ubcsat/
ubcsat_documentation_url := http://www.satlib.org/ubcsat/\#documentation

ubcsat_targets_prefix := $(ubcsat_prefix)-
ubcsat_targets := $(addprefix $(ubcsat_targets_prefix), $(ubcsat_supported_version_numbers))
ubcsat_recommended := $(ubcsat_targets_prefix)$(ubcsat_recommended_version_number)

ifndef Ubcsat
  ifdef UBCSAT
    Ubcsat_dir = $(UBCSAT)
  else
    Ubcsat_dir = $(ExternalSources)/Ubcsat
  endif
  Ubcsat = -I$(Ubcsat_dir)/$(ubcsat_recommended_version_number)/src
endif
# TODO: We must update and systematise the use of such variables.

# ################################
# Asciidoc
# ###############################

asciidoc_recommended_version_number ?= 8.2.1
asciidoc_supported_version_numbers ?= $(asciidoc_recommended_version_number)

asciidoc_prefix ?= asciidoc

asciidoc_targets_prefix := $(asciidoc_prefix)-
asciidoc_targets := $(addprefix $(asciidoc_targets_prefix), $(asciidoc_supported_version_numbers))
asciidoc_recommended := $(asciidoc_targets_prefix)$(asciidoc_recommended_version_number)

# ################################
# Cmake
# ###############################

cmake_recommended_version_number ?= 2.4.7
cmake_supported_version_numbers ?= $(cmake_recommended_version_number)

cmake_prefix ?= cmake

cmake_homepage_url := http://www.cmake.org/HTML/Index.html
cmake_documentation_url := http://www.cmake.org/HTML/Documentation.html

cmake_targets_prefix := $(cmake_prefix)-
cmake_targets := $(addprefix $(cmake_targets_prefix), $(cmake_supported_version_numbers))
cmake_recommended := $(cmake_targets_prefix)$(cmake_recommended_version_number)

# ################################
# mpfr
# ###############################

mpfr_recommended_version_number ?= 2.2.1
mpfr_supported_version_numbers ?= $(mpfr_recommended_version_number)

mpfr_prefix ?= mpfr

mpfr_targets_prefix := $(mpfr_prefix)-
mpfr_targets := $(addprefix $(mpfr_targets_prefix), $(mpfr_supported_version_numbers))
mpfr_recommended := $(mpfr_targets_prefix)$(mpfr_recommended_version_number)

