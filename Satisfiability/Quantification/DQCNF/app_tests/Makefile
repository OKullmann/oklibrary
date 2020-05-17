# Copyright 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version 0.2.1

SHELL := /bin/bash
.SUFFIXES :
.PHONY : all

.ONESHELL :

# translator1=valgrind -q --exit-on-first-error=yes ../autL1_debug
translator1 :=valgrind -q ../AutarkiesL1_debug
translator2 :="AutarkiesL1"

transcall=@S="$(subst T2,$(translator2), $(subst T1,$(translator1),$(1)))"
errcommand=returncode=$$?; if (( returncode != CODE )); then echo "ERROR with translator \"$$S\" on MESSAGE: wrong return-code $${returncode}."; exit 1
errout=$(subst MESSAGE,$(2), $(subst CODE,$(1),$(errcommand)))


targetlist_0 := E0 E1 E2 E3 E4 E5 E6 E7 E8 E9 E10 E11 E12 E13 E14
ct = $(addsuffix _T1, $(1)) $(addsuffix _T2, $(1))
targetlist := $(call ct,$(targetlist_0))

.PHONY : $(targetlist)


all : $(targetlist)

$(call ct,E0) : E0_% :
	$(call transcall,$*)
	echo "" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,25,"empty input"); fi

$(call ct,E1) : E1_% :
	$(call transcall,$*)
	echo -e "c\nc\nc" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,25,"empty line"); fi

$(call ct,E2) : E2_% :
	$(call transcall,$*)
	echo " " | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,26,"bad comment-line"); fi

$(call ct,E3) : E3_% :
	$(call transcall,$*)
	echo "p0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,3,"bad p-line \(no space\)"); fi

$(call ct,E4) : E4_% :
	$(call transcall,$*)
	echo "p" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,3,"bad p-line"); fi

$(call ct,E5) : E5_% :
	$(call transcall,$*)
	echo "p " | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,3,"bad p-line \(no \"cnf\"\)"); fi

$(call ct,E6) : E6_% :
	$(call transcall,$*)
	echo "p cnf" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,3,"bad p-line \(no n\)"); fi

$(call ct,E7) : E7_% :
	$(call transcall,$*)
	echo "p cnf x" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,3,"bad p-line \(bad n\)"); fi

$(call ct,E8) : E8_% :
	$(call transcall,$*)
	echo "p cnf 3000000000" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,4,"bad p-line \(large n\)"); fi

$(call ct,E9) : E9_% :
	$(call transcall,$*)
	echo "p cnf 0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,27,"bad p-line \(bad c\)"); fi

$(call ct,E10) : E10_% :
	$(call transcall,$*)
	echo "p cnf 0 0 " | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,3,"bad p-line \(trailing space\)"); fi

$(call ct,E11) : E11_% :
	$(call transcall,$*)
	echo -e "p cnf 0 0\nc" | $$S "-cin" -nil -nil n 2> /dev/null
	$(call errout,28,"illegal comment"); fi

$(call ct,E12) : E12_% :
	$(call transcall,$*)
	echo -e "p cnf 0 0\nc" | $$S "-cin" -nil -nil g
	$(call errout,0,"legal comment"); fi

$(call ct,E13) : E13_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\na 1 0\na 0" | $$S "-cin" -nil -nil n 2> /dev/null
	$(call errout,11,"repeated a-line"); fi

$(call ct,E14) : E14_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\na 1 0\na 0" | $$S "-cin" -nil -nil g
	$(call errout,0,"legal repeated \(empty\) a-line"); fi
