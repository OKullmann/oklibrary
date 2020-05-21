# Copyright 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version 0.2.5

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

maxerrortests = 36
targetlist_0 := $(shell for ((i=0; i<=$(maxerrortests); ++i )); do echo -n " E$$i"; done)
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

$(call ct,E15) : E15_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\na x 0" | $$S "-cin" -nil -nil n 2> /dev/null
	$(call errout,13,"bad a-read"); fi

$(call ct,E16) : E16_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\na 2 0" | $$S "-cin" -nil -nil n 2> /dev/null
	$(call errout,7,"too big a-read"); fi

$(call ct,E17) : E17_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\na 1 1 0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,15,"repeated a-read"); fi

$(call ct,E18) : E18_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\na 0" | $$S "-cin" -nil -nil n 2> /dev/null
	$(call errout,19,"empty a-line"); fi

$(call ct,E19) : E19_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\na 0" | $$S "-cin" -nil -nil g
	$(call errout,0,"legal empty a-line"); fi

$(call ct,E20) : E20_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\ne 1 0\ne 0" | $$S "-cin" -nil -nil n 2> /dev/null
	$(call errout,12,"repeated e-line"); fi

$(call ct,E21) : E21_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\ne 1 0\ne 0" | $$S "-cin" -nil -nil g
	$(call errout,0,"legal repeated \(empty\) e-line"); fi

$(call ct,E22) : E22_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\ne x 0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,14,"bad e-read"); fi

$(call ct,E23) : E23_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\na 2 0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,7,"too large a-read"); fi

$(call ct,E24) : E24_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\ne 1 1 0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,16,"repeated e-read"); fi

$(call ct,E25) : E25_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\ne 0" | $$S "-cin" -nil -nil n 2> /dev/null
	$(call errout,21,"empty e-line"); fi

$(call ct,E26) : E26_% :
	$(call transcall,$*)
	echo -e "p cnf 1 0\ne 0" | $$S "-cin" -nil -nil g
	$(call errout,0,"legal empty e-line"); fi

$(call ct,E27) : E27_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd x" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,29,"bad e-read in d-line"); fi

$(call ct,E28) : E28_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 3" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,7,"too large e-read in d-line"); fi

$(call ct,E29) : E29_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,22,"empty d-line"); fi

$(call ct,E30) : E30_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 1 0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,30,"repeated e-variable-read in d-line"); fi

$(call ct,E31) : E31_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 2 0" | $$S "-cin" -nil -nil vs
	$(call errout,0,"legal \(empty\) DQCNF"); fi

$(call ct,E32) : E32_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 2 0\nd 2 0" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,30,"repeated e-variable-read in d-line"); fi

$(call ct,E33) : E33_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 2 x 0\n" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,31,"bad a-variable-read in d-line"); fi

$(call ct,E34) : E34_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 2 3 0\n" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,7,"too big a-read in d-line"); fi

$(call ct,E35) : E35_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 2 1 0" | $$S "-cin" -nil -nil vs
	$(call errout,0,"legal \(empty\) DQCNF"); fi

$(call ct,E36) : E36_% :
	$(call transcall,$*)
	echo -e "p cnf 2 0\na 1 0\nd 2 2 0\n" | $$S "-cin" -nil -nil g 2> /dev/null
	$(call errout,23,"non-a-variable-read in d-line"); fi

