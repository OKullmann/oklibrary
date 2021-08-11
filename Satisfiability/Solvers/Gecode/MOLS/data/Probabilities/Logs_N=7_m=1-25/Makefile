# Oleg Zaikin, 6.8.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Generate random partially filled pairs of Latin squares, solve the Euler problem
# for them and output statistics.

# Version 0.0.2

SHELL := /bin/bash
.SUFFIXES :
.PHONY : all run beginrun runjobs endrun transfer clean cleanall

MAKE_PID := $(shell echo $$PPID)
JOB_FLAG := $(filter -j%, $(subst -j ,-j,$(shell ps T | grep "^\s*$(MAKE_PID).*$(MAKE)")))
ifeq ($(JOB_FLAG),)
  JOBS := 1
else ifeq ($(JOB_FLAG),-j)
  JOBS := 96
else
  JOBS := $(subst -j,,$(JOB_FLAG))
endif

.ONESHELL :

N := 7
solver := Euler
solver_params := 0 0 mind,val,one -head,+stat,-sol
solver_onlyheader_params := 2 2 "" +head,-stat,-sol
symmetriser := python symmetry-breaking-pair.py
resfile := results_N$N

oklib_id := 1609092523835210350
experiment_id := 1625864956250569213
machine_id := 0
basic_seeds := $(oklib_id),$(experiment_id),$(machine_id)

joblist := $(shell for (( i=1; i<=$(JOBS); ++i )); do echo $$i; done)
joblist := $(addprefix experiments/,$(joblist))
.PHONY : $(joblist)

all : run transfer
run : beginrun runjobs endrun

beginrun :
	@echo -e "\nBegin run, PID=$(MAKE_PID)" >> Log
	date -Ins >> Log
	echo "Parallel: j=$(JOBS)" >> Log

runjobs : | beginrun
runjobs : $(joblist)

endrun : | runjobs
endrun :
	@echo >> Log
	ps -o "%t" --pid $(MAKE_PID) | tee -a Log
	echo "End run, PID=$(MAKE_PID)" >> Log
	date -Ins >> Log


$(joblist) : experiments/% :
	@cd experiments
	seed=0
	ofile=N$N_m$*.out
	while [[ ! -f ../END ]]; do echo -n $$seed" "$*" " >> $$ofile; LSRG $N,2 "-co" "1*0,0,$*;1*0,0,0" $(basic_seeds),$$seed | $(symmetriser) | $(solver) $(solver_params) >> $$ofile; ((++seed)); done

transfer : | run
	echo -n "seed m " > $(resfile)
	$(solver) $(solver_onlyheader_params) >> $(resfile)
	find ./experiments -name "*.out" -print0 | xargs -0 -I file cat file >> $(resfile)

clean :
	rm -f END $(resfile)

cleanall : clean
	rm -r -f experiments/*
	echo -en "\ncleanall:\n" >> Log
	date >> Log
