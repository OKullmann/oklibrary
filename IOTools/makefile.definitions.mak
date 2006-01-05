# Oliver Kullmann, 13.4.2005 (Swansea)

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
ifdef LOKI
  Loki := -I$(LOKI)
else
  Loki :=
endif

General_options := -g
Optimisation_options := -O3

test_program := TestIOTools

programs :=

source_libraries = $(OKTestsystem) $(Bibliothek) $(Loki) $(Boost)

link_libraries :=

Root := $(wildcard ../../..)
