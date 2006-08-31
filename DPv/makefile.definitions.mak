# Tony H. Bao, 13.04.2005



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

ifdef OKSYSTEM
  OKSystem := -I$(OKSYSTEM)
else
  OKSystem := 
endif

OKTestSystem := -I$(OKSYSTEM)/Transitional/TestSystem

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestDPv

programs := DPv

source_libraries = $(OKTestSystem) $(Bibliothek) $(Boost) $(OKMessages) $(OKIOtools) $(OKSystem)

boost_filesystem := -lboost_filesystem-gcc
boost_date_time := -lboost_date_time-gcc

link_libraries := $(boost_date_time) $(boost_filesystem)


