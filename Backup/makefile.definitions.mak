# Tony H. Bao, 13.04.2005

ifdef OKTESTSYSTEM
  OKTestsystem := -I$(OKTESTSYSTEM)
else
  OKTestsystem :=
endif

ifdef BIBLIOTHEK
  Bibliothek := -I$(BIBLIOTHEK)
else
  Bibliothek :=
endif

ifdef OKMESSAGES
  OKMessages := -I$(OKMESSAGES)
else
  OKMessages :=
endif

ifdef OKIOTOOLS
  OKIOtools := -I$(OKIOTOOLS)
else
  OKIOtools :=
endif

General_options := -g
Optimisation_options := -O3

test_program := TestBackup

programs := Backup

source_libraries = $(OKTestsystem) $(Bibliothek) $(Loki) $(Boost) $(OKMessages) $(OKIOtools)

boost_filesystem := -lboost_filesystem-gcc
boost_date_time := -lboost_date_time-gcc

link_libraries := $(boost_date_time) $(boost_filesystem)

Root := $(wildcard ../../..)
