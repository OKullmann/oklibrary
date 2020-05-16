# Copyright 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version 0.0.2

SHELL := /bin/bash
.SUFFIXES :
.PHONY : all

.ONESHELL :

# translator1="valgrind -q --exit-on-first-error=yes ../autL1_debug"
translator1 := "valgrind -q ../AutarkiesL1_debug"
translator2 := "AutarkiesL1"
translators = $(translator1) $(translator2)

targetlist := E0 E1 E2

.PHONY : $(targetlist)


all : $(targetlist)

E0 :
	@for S in $(translators); do
	echo "" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 25 )); then
	echo "ERROR with translator \"$$S\" on empty input: wrong return-code $${returncode}."; exit 1
	fi
	done

E1 :
	@for S in $(translators); do
	echo -e "c\nc\nc" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 25 )); then
	echo "ERROR with translator \"$$S\" on empty line: wrong return-code $${returncode}."; exit 1
	fi
	done

E2 :
	@for S in $(translators); do
	echo " " | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 26 )); then
	echo "ERROR with translator \"$$S\" on bad comment-line: wrong return-code $${returncode}."; exit 1
	fi
	done
