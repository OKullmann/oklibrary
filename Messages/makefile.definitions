# Tony H. Bao, 13.04.2005 (Swansea)

ifdef BIBLIOTHEK
  Bibliothek := -I$(BIBLIOTHEK)
else
  Bibliothek :=
endif

ifdef OKIOTOOLS
  OKIOtools := -I$(OKIOTOOLS)
else
  OKIOtools :=
endif

ifdef LOKI
  Loki := -I$(LOKI)
else
  Loki :=
endif

ifdef BOOST
  Boost := -I$(BOOST)
else
  Boost :=
endif

General_options := -g
Optimisation_options := -O3

test_program := TestMessages

programs := Messages_Testapplication

source_libraries := $(Bibliothek) $(OKIOtools) $(Loki) $(Boost) 

link_libraries := 

Root := $(wildcard ../../..)
