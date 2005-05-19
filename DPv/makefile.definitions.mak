# Tony H. Bao, 13.04.2005

ifdef BIBLIOTHEK
  Bibliothek := -I$(BIBLIOTHEK)
else
  Bibliothek :=
endif

ifdef BOOST
  Boost := -I$(BOOST)
else
  Boost :=
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

test_program := TestDPv

programs := DPv

source_libraries := $(Bibliothek) $(Loki) $(Boost) $(OKMessages) $(OKIOtools)

boost_filesystem := -lboost_filesystem-gcc
boost_date_time := -lboost_date_time-gcc

link_libraries := $(boost_date_time) $(boost_filesystem)

Root := $(wildcard ../../..)
