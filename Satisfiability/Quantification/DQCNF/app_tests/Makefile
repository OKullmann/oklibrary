# Copyright 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version 0.2.0

SHELL := /bin/bash
.SUFFIXES :
.PHONY : all

.ONESHELL :

# translator1=valgrind -q --exit-on-first-error=yes ../autL1_debug
translator1 :=valgrind -q ../AutarkiesL1_debug
translator2 :="AutarkiesL1"

transcall=$(subst T2,$(translator2), $(subst T1,$(translator1),$(1)))
errcommand=returncode=$$?; if (( returncode != CODE )); then echo "ERROR with translator \"$$S\" on MESSAGE: wrong return-code $${returncode}."; exit 1
errout=$(subst MESSAGE,$(2), $(subst CODE,$(1),$(errcommand)))


targetlist_0 := E0 E1 E2 E3 E4 E5 E6 E7 E8 E9 E10 E11 E12 E13 E14
ct = $(addsuffix _T1, $(1)) $(addsuffix _T2, $(1))
targetlist := $(call ct,$(targetlist_0))

.PHONY : $(targetlist)


all : $(targetlist)

$(call ct,E0) : E0_% :
	@S="$(call transcall,$*)"
	echo "" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,25,"empty input"); fi

$(call ct,E1) : E1_% :
	@S="$(call transcall,$*)"
	echo -e "c\nc\nc" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,25,"empty line"); fi

$(call ct,E2) : E2_% :
	@S="$(call transcall,$*)"
	echo " " | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,26,"bad comment-line"); fi

$(call ct,E3) : E3_% :
	@S="$(call transcall,$*)"
	echo "p0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,3,"bad p-line \(no space\)"); fi

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

$(call ct,E8) : E8_% :
	@S="$(call transcall,$*)"
	echo "p cnf 3000000000" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 4 )); then
	echo "ERROR with translator \"$$S\" on bad p-line (large n): wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E9) : E9_% :
	@S="$(call transcall,$*)"
	echo "p cnf 0" | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 27 )); then
	echo "ERROR with translator \"$$S\" on bad p-line (bad c): wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E10) : E10_% :
	@S="$(call transcall,$*)"
	echo "p cnf 0 0 " | $$S "-cin" -nil -nil g 2> /dev/null
	returncode=$$?
	if  (( returncode != 3 )); then
	echo "ERROR with translator \"$$S\" on bad p-line (trailing): wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E11) : E11_% :
	@S="$(call transcall,$*)"
	echo -e "p cnf 0 0\nc" | $$S "-cin" -nil -nil n 2> /dev/null
	returncode=$$?
	if  (( returncode != 28 )); then
	echo "ERROR with translator \"$$S\" on illegal comment: wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E12) : E12_% :
	@S="$(call transcall,$*)"
	echo -e "p cnf 0 0\nc" | $$S "-cin" -nil -nil g
	returncode=$$?
	if  (( returncode != 0 )); then
	echo "ERROR with translator \"$$S\" on legal comment: wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E13) : E13_% :
	@S="$(call transcall,$*)"
	echo -e "p cnf 1 0\na 1 0\na 0" | $$S "-cin" -nil -nil n 2> /dev/null
	returncode=$$?
	if  (( returncode != 11 )); then
	echo "ERROR with translator \"$$S\" on repeated a-line: wrong return-code $${returncode}."; exit 1
	fi

$(call ct,E14) : E14_% :
	@S="$(call transcall,$*)"
	echo -e "p cnf 1 0\na 1 0\na 0" | $$S "-cin" -nil -nil g
	returncode=$$?
	if  (( returncode != 0 )); then
	echo "ERROR with translator \"$$S\" on legal repeated (empty) a-line: wrong return-code $${returncode}."; exit 1
	fi

