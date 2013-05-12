# Oliver Kullmann, 1.8.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Tests for external sources.
# To be executed *after* the configuration variables have been set.

# New variables for the configuration of building gcc (to be designed 
# and implemented):

gpp_system_call_okl ?= g++
gcc_system_call_okl ?= gcc

gcc_version_number_extraction_okl := awk '/[0-9]\.[0-9]\.[0-9]/{print $$3}'
# assumes that the output of "gcc --version" contains a line of the form
# (for example) "gcc (GCC) 3.4.3"
# Perhaps all such calls should be replaced by the use of shell pattern matching
# as shown in the setting of version_gpp_system_call_okl (as opposed to
# the setting of version_gcc_system_call_okl) ?

location_gpp_system_call_okl ?= $(shell (type -P $(gpp_system_call_okl)))
ifeq ($(location_gpp_system_call_okl),)
  gpp_system_call_ready_okl ?= NO
else
  version_gpp_system_call_okl ?= $(shell if [[ "$$($(gpp_system_call_okl) --version)" =~ ".*([0-9]\.[0-9]\.[0-9]).*" ]]; then echo $${BASH_REMATCH[1]}; else echo "Unknown"; fi)
# $(shell $(gpp_system_call_okl) --version | $(gcc_version_number_extraction_okl))
  ifeq ($(version_gpp_system_call_okl),$(gcc_recommended_version_number_okl))
    gpp_system_call_ready_okl ?= YES
  else
    gpp_system_call_ready_okl ?= MAYBE
  endif
endif
location_gcc_system_call_okl ?= $(shell (type -P $(gcc_system_call_okl)))
ifeq ($(location_gcc_system_call_okl),)
  gcc_system_call_ready_okl ?= NO
else
  version_gcc_system_call_okl ?= $(shell $(gcc_system_call_okl) --version| $(gcc_version_number_extraction_okl))
  ifeq ($(version_gcc_system_call_okl),$(gcc_recommended_version_number_okl))
    gcc_system_call_ready_okl ?= YES
  else
    gcc_system_call_ready_okl ?= MAYBE
  endif
endif

gpp_local_call_okl ?= $(gpp_call_okl)
gcc_local_call_okl ?= $(gcc_call_okl)

location_gpp_local_call_okl ?= $(shell (type -P $(gpp_local_call_okl)))
ifeq ($(location_gpp_local_call_okl),)
  gpp_local_call_ready_okl ?= NO
else
  version_gpp_local_call_okl ?= $(shell $(gpp_local_call_okl) --version | $(gcc_version_number_extraction_okl))
  ifeq ($(version_gpp_local_call_okl),$(gcc_recommended_version_number_okl))
    gpp_local_call_ready_okl ?= YES
  else
    gpp_local_call_ready_okl ?= ERROR
  endif
endif
location_gcc_local_call_okl ?= $(shell (type -P $(gcc_local_call_okl)))
ifeq ($(location_gcc_local_call_okl),)
  gcc_local_call_ready_okl ?= NO
else
  version_gcc_local_call_okl ?= $(shell $(gcc_local_call_okl) --version | $(gcc_version_number_extraction_okl))
  ifeq ($(version_gcc_local_call_okl),$(gcc_recommended_version_number_okl))
    gcc_local_call_ready_okl ?= YES
  else
    gcc_local_call_ready_okl ?= ERROR
  endif
endif

# the following construction needs to be generalised by some function
gcc_html_documentation_index_location_tag_okl ?= <a href="$(gcc_html_output_okl)">$(gcc_html_output_okl)</a>

# New variables for the configuration of building doxygen (to be designed 
# and implemented):

location_doxygen_call_okl ?= $(shell (type -P $(doxygen_call_okl)))
ifeq ($(location_doxygen_call_okl),)
  doxygen_call_ready_okl ?= NO
else
  version_doxygen_call_okl ?= $(shell $(doxygen_call_okl) --version)
  ifeq ($(version_doxygen_call_okl),$(doxygen_recommended_version_number_okl))
    doxygen_call_ready_okl ?= YES
  else
    doxygen_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
doxygen_html_documentation_index_location_tag_okl ?= <a href="$(doxygen_html_documentation_index_location_okl)">$(doxygen_html_documentation_index_location_okl)</a>

# New variables for the configuration of building ocaml (to be designed 
# and implemented):

ocaml_call ?= $(ocaml_bin_dir)/ocaml

ocaml_version_number_extraction := awk '/ [0-9]+\.[0-9]+(\.[0-9]+)?/{print $$6}'
# assumes that the output of "ocaml -version" contains a line of the form
# (for example) "The Objective Caml toplevel, version 3.10.0"

location_ocaml_call ?= $(shell (type -P $(ocaml_call)))
ifeq ($(location_ocaml_call),)
  ocaml_call_ready ?= NO
else
  version_ocaml_call ?= $(shell $(ocaml_call) -version | $(ocaml_version_number_extraction))
  ifeq ($(version_ocaml_call),$(ocaml_recommended_version_number))
    ocaml_call_ready ?= YES
  else
    ocaml_call_ready ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
ocaml_docu_page_tag ?= <a href="$(ocaml_docu_page)">Ocaml installation page</a>


# New variables for the configuration of building coq (to be designed 
# and implemented):

coq_call ?= $(ExternalSources)/Coq/$(coq_recommended_version_number)/bin/coqtop

coq_version_number_extraction := awk '/ [0-9]\.[0-9] /{print $$6}'
# assumes that the output of "coq --version" contains a line of the form
# (for example) "The Coq Proof Assistant, version 8.1 (Feb. 2007)"

location_coq_call ?= $(shell (type -P $(coq_call)))
ifeq ($(location_coq_call),)
  coq_call_ready ?= NO
else
  version_coq_call ?= $(shell $(coq_call) --version | $(coq_version_number_extraction))
  ifeq ($(version_coq_call),$(coq_recommended_version_number))
    coq_call_ready ?= YES
  else
    coq_call_ready ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
coq_html_documentation_index_location_tag ?= <a href="$(coq_html_output)">$(coq_html_output)</a>


# New variables for the configuration of building sage (to be designed 
# and implemented):

sage_call_okl ?= $(sage_installation_dir)/sage

sage_version_number_extraction_okl := > /dev/null; echo $$?
# sage doesn't allow to ask for the version number

location_sage_call_okl ?= $(shell (type -P $(sage_call_okl)))
ifeq ($(location_sage_call_okl),)
  sage_call_ready_okl ?= NO
else
  output_sage_call_okl ?=  $(shell $(sage_call_okl) -h $(sage_version_number_extraction_okl))
  ifeq ($(output_sage_call_okl),1)
    version_sage_call_okl ?= $(sage_recommended_version_number_okl)
  else
    version_sage_call_okl ?= UNKNOWN
  endif
  ifeq ($(version_sage_call_okl),$(sage_recommended_version_number_okl))
    sage_call_ready_okl ?= YES
  else
    sage_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
sage_html_documentation_index_location_tag_okl ?= <a href="$(sage_main_index_okl)">$(sage_main_index_okl)</a>


# New variables for the configuration of building git (to be designed 
# and implemented):

git_version_number_extraction_okl := awk '/ [0-9]\.[0-9](\.[0-9])+/{print $$3}'
# assumes that the output of "git --version" contains a line of the form
# (for example) "git version 1.5.2.4" or "git version 1.5.4"

location_git_call_okl ?= $(shell (type -P $(git_call_okl)))
ifeq ($(location_git_call_okl),)
  git_call_ready_okl ?= NO
else
  version_git_call_okl ?= $(shell $(git_call_okl) --version | $(git_version_number_extraction_okl))
  ifeq ($(version_git_call_okl),$(git_recommended_version_number_okl))
    git_call_ready_okl ?= YES
  else
    git_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
git_html_documentation_index_location_tag_okl ?= <a href="$(git_html_documentation_index_location_okl)">$(git_html_documentation_index_location_okl)</a>

# New variables for the configuration of building asciidoc (to be designed 
# and implemented):

asciidoc_version_number_extraction_okl := awk '/ [0-9]\.[0-9](\.[0-9])+/{print $$2}'
# assumes that the output of "asciidoc --version" contains a line of the form
# (for example) "asciidoc version 8.2.1"

location_asciidoc_call_okl ?= $(shell (type -P $(asciidoc_call_okl)))
ifeq ($(location_asciidoc_call_okl),)
  asciidoc_call_ready_okl ?= NO
else
  version_asciidoc_call_okl ?= $(shell $(asciidoc_call_okl) --version 2>&1 | $(asciidoc_version_number_extraction_okl))
  ifeq ($(version_asciidoc_call_okl),$(asciidoc_recommended_version_number_okl))
    asciidoc_call_ready_okl ?= YES
  else
    asciidoc_call_ready_okl ?= MAYBE
  endif
endif


# New variables for the configuration of building gmp (to be designed 
# and implemented):

check_gmp_header_okl ?= $(shell [[ -f $(gmp_source_library_okl)/gmp.h ]]; echo $$?)
ifneq ($(check_gmp_header_okl),0)
  gmp_ready_okl ?= NO
else
  version_gmp_okl ?= $(shell basename $$(dirname $(gmp_source_library_okl)))
  ifeq ($(version_gmp_okl),$(gmp_recommended_version_number_okl))
    gmp_ready_okl ?= YES
  else
    gmp_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
gmp_html_documentation_index_location_tag_okl ?= <a href="$(gmp_html_documentation_index_location_okl)">$(gmp_html_documentation_index_location_okl)</a>

# New variables for the configuration of building valgrind (to be designed 
# and implemented):

valgrind_call_okl ?= valgrind

valgrind_version_number_extraction_okl := sed 's/valgrind-\([0-9]\.[0-9]\.[0-9]\)/\1/'
# assumes that the output of "valgrind --version" is a line of the form
# (for example) "valgrind-3.2.3"

location_valgrind_call_okl ?= $(shell (type -P $(valgrind_call_okl)))
ifeq ($(location_valgrind_call_okl),)
  valgrind_call_ready_okl ?= NO
else
  version_valgrind_call_okl ?= $(shell $(valgrind_call_okl) --version | $(valgrind_version_number_extraction_okl))
  ifeq ($(version_valgrind_call_okl),$(valgrind_recommended_version_number_okl))
    valgrind_call_ready_okl ?= YES
  else
    valgrind_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
valgrind_html_documentation_index_location_tag_okl ?= <a href="$(valgrind_html_documentation_index_location_okl)">$(valgrind_html_documentation_index_location_okl)</a>

# New variables for the configuration of building postgresql (to be designed 
# and implemented):

postgresql_call ?= psql

postgresql_version_number_extraction := awk '/ [0-9]\.[0-9]\.[0-9]+/{print $$3}'
# assumes that the output of "postgresql --version" contains a line of the form
# (for example) "psql (PostgreSQL) 7.4.13"

location_postgresql_call ?= $(shell (type -P $(postgresql_call)))
ifeq ($(location_postgresql_call),)
  postgresql_call_ready ?= NO
else
  version_postgresql_call ?= $(shell $(postgresql_call) --version | $(postgresql_version_number_extraction))
  ifeq ($(version_postgresql_call),$(postgresql_recommended_version_number))
    postgresql_call_ready ?= YES
  else
    postgresql_call_ready ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
postgresql_html_documentation_index_location_tag ?= <a href="$(postgresql_html_documentation_index_location)">$(postgresql_html_documentation_index_location)</a>

# New variables for the configuration of building R (to be designed 
# and implemented):

R_version_number_extraction_okl := awk '/ [0-9]\.[0-9]+\.[0-9]/{print $$3}'
# assumes that the output of "R --version" contains a line of the form
# (for example) "R version 2.5.0 (2007-04-23)"

location_R_call_okl ?= $(shell (type -P $(R_call_okl)))
ifeq ($(location_R_call_okl),)
  R_call_ready_okl ?= NO
else
  version_R_call_okl ?= $(shell $(R_call_okl) --version | $(R_version_number_extraction_okl))
  ifeq ($(version_R_call_okl),$(R_recommended_version_number_okl))
    R_call_ready_okl ?= YES
  else
    R_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
R_html_documentation_index_location_tag_okl ?= <a href="$(R_html_documentation_index_location_okl)">$(R_html_documentation_index_location_okl)</a>


# New variables for the configuration of building clisp (to be designed 
# and implemented):

clisp_version_number_extraction_okl := awk '/ CLISP [0-9]+\.[0-9]+(\.[0-9])?/{print $$3}'
# assumes that the output of "clisp -version" contains a line of the form
# (for example) "GNU CLISP 2.43 (2007-11-18) (built 3407616533) (memory 3407616899)"
# where the version number can also be of the form "2.44.1".

location_clisp_call_okl ?= $(shell (type -P $(clisp_call_okl)))
ifeq ($(location_clisp_call_okl),)
  clisp_call_ready_okl ?= NO
else
  version_clisp_call_okl ?= $(shell $(clisp_call_okl) --version | $(clisp_version_number_extraction_okl))
  ifeq ($(version_clisp_call_okl),$(clisp_recommended_version_number_okl))
    clisp_call_ready_okl ?= YES
  else
    clisp_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
clisp_html_documentation_index_location_tag_okl ?= <a href="$(clisp_html_output_okl)">$(clisp_html_output_okl)</a>


# New variables for the configuration of building ecl (to be designed 
# and implemented):

location_ecl_call_okl ?= $(shell (type -P $(ecl_call_okl)))
ifeq ($(location_ecl_call_okl),)
  ecl_call_ready_okl ?= NO
else
  version_ecl_call_okl ?= $(shell $(ecl_call_okl) -eval "(progn (princ (lisp-implementation-version)) (terpri) (quit))")
  ifeq ($(version_ecl_call_okl),$(ecl_recommended_version_number_okl))
    ecl_call_ready_okl ?= YES
  else
    ecl_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
ecl_html_documentation_index_location_tag_okl ?= <a href="$(ecl_html_output_okl)">$(ecl_html_output_okl)</a>


# New variables for the configuration of building rlwrap (to be designed 
# and implemented):

rlwrap_version_number_extraction_okl := awk '/rlwrap [0-9]+\.[0-9]+(\.[0-9])?/{print $$2}'
# assumes that the output of "rlwrap -version" contains a line of the form
# (for example) "rlwrap 0.30"
# where the version number can also be of the form "2.44.1".

location_rlwrap_call_okl ?= $(shell (type -P $(rlwrap_call_okl)))
ifeq ($(location_rlwrap_call_okl),)
  rlwrap_call_ready_okl ?= NO
else
  version_rlwrap_call_okl ?= $(shell $(rlwrap_call_okl) --version | $(rlwrap_version_number_extraction_okl))
  ifeq ($(version_rlwrap_call_okl),$(rlwrap_recommended_version_number_okl))
    rlwrap_call_ready_okl ?= YES
  else
    rlwrap_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
rlwrap_html_documentation_index_location_tag_okl ?= <a href="$(rlwrap_html_output_okl)">$(rlwrap_html_output_okl)</a>



# New variables for the configuration of building maxima (to be designed 
# and implemented):

maxima_version_number_extraction_okl := awk '/Maxima/ {print $$2}'
# assumes that the output of "maxima --version" contains a line of the form
# (for example) "Maxima 5.14.0"

location_maxima_call_okl ?= $(shell (type -P $(maxima_call_okl)))
ifeq ($(location_maxima_call_okl),)
  maxima_call_ready_okl ?= NO
else
  version_maxima_call_okl ?= $(shell $(maxima_call_okl) --version | $(maxima_version_number_extraction_okl))
  ifeq ($(version_maxima_call_okl),$(maxima_recommended_version_number_okl))
    maxima_call_ready_okl ?= YES
  else
    maxima_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
maxima_html_documentation_index_location_tag_okl ?= <a href="$(maxima_html_output_okl)">$(maxima_html_output_okl)</a>
maxima_gf_manual_tag_okl ?= <a href="$(maxima_gf_manual_okl)">finite fields manual</a>


# New variables for the configuration of building ubcsat (to be designed 
# and implemented):

ubcsat_version_number_extraction_okl := awk '/UBCSAT version [0-9]+\.[0-9]+\.[0-9]+/{print $$4}'
# assumes that the output of "ubcsat --version" contains a line of the form
# (for example) "# UBCSAT version 1.0.0 (Grouse Mountain Release)"

location_ubcsat_call_okl ?= $(shell (type -P $(ubcsat_call_okl)))
ifeq ($(location_ubcsat_call_okl),)
  ubcsat_call_ready_okl ?= NO
else
  version_ubcsat_call_okl ?= $(shell $(ubcsat_call_okl) -hc | $(ubcsat_version_number_extraction_okl))
  ifeq ($(version_ubcsat_call_okl),$(subst -,.,$(ubcsat_recommended_version_number_okl)))
    ubcsat_call_ready_okl ?= YES
  else
    ubcsat_call_ready_okl ?= MAYBE
  endif
endif


# New variables for the configuration of building gap (to be designed 
# and implemented):

gap_version_number_extraction_okl := awk '/run the Groups/{print $$9}'
# assumes that the output of "gap -h" contains a line of the form
# (for example) "run the Groups, Algorithms and Programming system, Version 4.4.10"

location_gap_call_okl ?= $(shell (type -P $(gap_call_okl)))
ifeq ($(location_gap_call_okl),)
  gap_call_ready_okl ?= NO
else
  version_gap_call_okl ?= $(shell $(gap_call_okl) -h 2>&1 | $(gap_version_number_extraction_okl))
  ifeq ($(version_gap_call_okl),$(gap_extracted_version_number_okl))
    gap_call_ready_okl ?= YES
  else
    gap_call_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
gap_html_documentation_index_location_tag_okl ?= <a href="$(gap_html_output_okl)">$(gap_html_output_okl)</a>


# New variables for the configuration of building grasp (to be designed 
# and implemented):

grasp_version_number_extraction_okl := awk '/UNSATISFIABLE INSTANCE/{print $(grasp_recommended_version_number_okl)}'
grasp_example_program_okl := $(OKsystem)/OKlib/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/basic/DHNExample.cnf

location_grasp_call_okl ?= $(shell (type -P $(grasp_call_okl)))
ifeq ($(location_grasp_call_okl),)
  grasp_call_ready_okl ?= NO
else
  version_grasp_call_okl ?= $(shell $(grasp_call_okl) $(grasp_example_program_okl) | $(grasp_version_number_extraction_okl))
  ifeq ($(version_grasp_call_okl),$(grasp_recommended_version_number_okl))
    grasp_call_ready_okl ?= YES
  else
    grasp_call_ready_okl ?= ERROR
  endif
endif

grasp_html_documentation_index_location_tag_okl ?= <a href="$(grasp_man_okl)">$(grasp_man_okl)</a>


# New variables for the configuration of building picosat (to be designed 
# and implemented):

location_picosat_call_okl ?= $(shell (type -P $(picosat_call_okl)))
ifeq ($(location_picosat_call_okl),)
  picosat_call_ready_okl ?= NO
else
  version_picosat_call_okl ?= $(shell $(picosat_call_okl) --version)
  ifeq ($(version_picosat_call_okl),$(picosat_recommended_version_number_okl))
    picosat_call_ready_okl ?= YES
  else
    picosat_call_ready_okl ?= ERROR
  endif
endif


# New variables for the configuration of building mhash (to be designed 
# and implemented):

check_mhash_header_okl ?= $(shell [[ -f $(mhash_source_library_okl)/mhash.h ]]; echo $$?)
ifneq ($(check_mhash_header_okl),0)
  mhash_ready_okl ?= NO
else
  version_mhash_okl ?= $(shell basename $$(dirname $(mhash_source_library_okl)))
  ifeq ($(version_mhash_okl),$(mhash_recommended_version_number_okl))
    mhash_ready_okl ?= YES
  else
    mhash_ready_okl ?= MAYBE
  endif
endif

# the following construction needs to be generalised by some function
mhash_html_documentation_index_location_tag_okl ?= <a href="$(mhash_html_documentation_index_location_okl)">$(mhash_html_documentation_index_location_okl)</a>


# New variables for the configuration of building espresso (to be designed 
# and implemented):

espresso_version_number_extraction_okl := awk '/UC Berkeley, Espresso Version/ {print $$5}' | sed 's/[#,]//g'
# assumes that the output of "espresso --version" contains a line of the form
# (for example) "UC Berkeley, Espresso Version #2.3, Release date 01/31/88"

location_espresso_call_okl ?= $(shell (type -P $(espresso_call_okl)))
ifeq ($(location_espresso_call_okl),)
  espresso_call_ready_okl ?= NO
else
  version_espresso_call_okl ?= $(shell $(espresso_call_okl) --version | awk '/UC Berkeley, Espresso Version/ {print $$5}' | sed 's/[\#,]//g')
  ifeq ($(version_espresso_call_okl),$(espresso_recommended_version_number_okl))
    espresso_call_ready_okl ?= YES
  else
    espresso_call_ready_okl ?= ERROR
  endif
endif

# New variables for the configuration of building cryptominisat (to be designed 
# and implemented):

cryptominisat_version_number_extraction_okl ?= awk '/c This is CryptoMiniSat/ { print $$5 }'
# assumes that the output of "echo '' | cryptominisat" contains a line of the form
# (for example) "c This is CryptoMiniSat 2.9.0"

location_cryptominisat_call_okl ?= $(shell (type -P $(cryptominisat_call_okl)))
ifeq ($(location_cryptominisat_call_okl),)
  cryptominisat_call_ready_okl ?= NO
else
  version_cryptominisat_call_okl ?= $(shell echo '' | $(cryptominisat_call_okl) | $(cryptominisat_version_number_extraction_okl))
  ifeq ($(version_cryptominisat_call_okl),$(cryptominisat_recommended_version_number_okl))
    cryptominisat_call_ready_okl ?= YES
  else
    cryptominisat_call_ready_okl ?= $(version_cryptominisat_call_okl)
  endif
endif

# New variables for the configuration of building glucose (to be designed 
# and implemented):

glucose_version_number_extraction_okl ?= awk '/c This is glucose/ { print $$5 }'
# assumes that the output of "echo '' | glucose" contains a line of the form
# (for example) "c This is glucose 1.0 --  based on MiniSAT (Many thanks to MiniSAT team)"

location_glucose_call_okl ?= $(shell (type -P $(glucose_call_okl)))
ifeq ($(location_glucose_call_okl),)
  glucose_call_ready_okl ?= NO
else
  version_glucose_call_okl ?= $(shell echo '' | $(glucose_call_okl) 2>&1 | $(glucose_version_number_extraction_okl))
  ifeq ($(version_glucose_call_okl),$(glucose_recommended_version_number_okl))
    glucose_call_ready_okl ?= YES
  else
    glucose_call_ready_okl ?= $(version_glucose_call_okl)
  endif
endif
