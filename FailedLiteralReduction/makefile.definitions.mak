# Oliver Kullmann, 13.4.2005 (Swansea)

ifdef BIBLIOTHEK
  Bibliothek := -I$(BIBLIOTHEK)
else
  Bibliothek :=
endif
ifdef OKTESTSYSTEM
  OKTestsystem := -I$(OKTESTSYSTEM)
else
  OKTestsystem :=
endif

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestFailedLiteralReduction

programs :=

source_libraries =  $(Boost) $(OKTestsystem)

link_libraries := 

Root := $(wildcard ../../..)
