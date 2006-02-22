# Oliver Kullmann, 13.4.2005 (Swansea)

ifdef LOKI
  Loki := -I$(LOKI)
else
  Loki :=
endif
ifdef XERCES_H
  Xerces_h := -I$(XERCES_H)
else
  Xerces_h :=
endif
ifdef XERCES_SO
  Xerces_so := -I$(XERCES_SO)
else
  Xerces_so :=
endif
ifdef OKSYSTEM
  OKSystem := -I$(OKSYSTEM)
else
  OKSystem := 
endif

OKTestsystem := -I$(OKSYSTEM)/Transitional/Testsystem

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestAutarkySearch

programs :=

source_libraries =  $(OKSystem) $(Boost) $(OKTestsystem) 

link_libraries := 

Root := $(wildcard ../../..)
