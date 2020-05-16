# Copyright 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version 0.1.0

SHELL := /bin/bash
.SUFFIXES :
.PHONY : all

.ONESHELL :

# translator1=valgrind -q --exit-on-first-error=yes ../autL1_debug
translator1 :=valgrind -q ../AutarkiesL1_debug
translator2 :="AutarkiesL1"

transcall =$(subst T2,$(translator2), $(subst T1,$(translator1),$(1)))

targetlist_0 := E0 E1 E2 E3 E4 E5 E6 E7
ct = $(addsuffix _T1, $(1)) $(addsuffix _T2, $(1))
targetlist := $(call ct,$(targetlist_0))

.PHONY : $(targetlist)


all : $(targetlist)

$(call ct,E0) : E0_% :
	@S="$(call transcall,$*)"
	echo "" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 25 )); then
	echo "ERROR with translator \"$$S\" on empty input: wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E1) : E1_% :
	@S="$(call transcall,$*)"
	echo -e "c\nc\nc" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 25 )); then
	echo "ERROR with translator \"$$S\" on empty line: wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E2) : E2_% :
	@S="$(call transcall,$*)"
	echo " " | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 26 )); then
	echo "ERROR with translator \"$$S\" on bad comment-line: wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E3) : E3_% :
	@S="$(call transcall,$*)"
	echo "p0" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 3 )); then
	echo "ERROR with translator \"$$S\" on bad p-line (no space): wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E4) : E4_% :
	@S="$(call transcall,$*)"
	echo "p" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 3 )); then
	echo "ERROR with translator \"$$S\" on bad p-line: wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E5) : E5_% :
	@S="$(call transcall,$*)"
	echo "p " | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 3 )); then
	echo "ERROR with translator \"$$S\" on bad p-line (no \"cnf\"): wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E6) : E6_% :
	@S="$(call transcall,$*)"
	echo "p cnf" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 3 )); then
	echo "ERROR with translator \"$$S\" on bad p-line (no n): wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E7) : E7_% :
	@S="$(call transcall,$*)"
	echo "p cnf x" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 3 )); then
	echo "ERROR with translator \"$$S\" on bad p-line (bad n): wrong return-code $${returncode}."; exit 1
	fi
