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


General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestMessages

programs := Messages_Testapplication

source_libraries = $(OKTestsystem) $(Bibliothek) $(Boost) 

link_libraries := 

Root := $(wildcard ../../..)
