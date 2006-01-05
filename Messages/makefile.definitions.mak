# Tony H. Bao, 13.04.2005 (Swansea)

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

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestMessages

programs := Messages_Testapplication

source_libraries = $(OKTestsystem) $(Bibliothek) $(OKIOtools) $(Loki) $(Boost) 

link_libraries := 

Root := $(wildcard ../../..)
