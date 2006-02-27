# Tony H. Bao, 13.04.2005

ifdef OKSYSTEM
  OKSystem := -I$(OKSYSTEM)
else
  OKSystem := 
endif

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestBackup

programs := Backup

source_libraries = $(Boost) $(OKSystem)

boost_filesystem := -lboost_filesystem-gcc
boost_date_time := -lboost_date_time-gcc

link_libraries := $(boost_date_time) $(boost_filesystem)

Root := $(wildcard ../../..)
