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

ifdef OKTESTSYSTEM
  OKTestsystem := -I$(OKTESTSYSTEM)
else
  OKTestsystem :=
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

test_program := TestLaTeX

programs := 

source_libraries := $(Boost) $(OKTestsystem)

link_libraries := 

Root := $(wildcard ../../..)
