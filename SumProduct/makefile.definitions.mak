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

OKTestSystem := -I$(OKSYSTEM)/Transitional/TestSystem

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestSumProduct

programs :=

source_libraries =  $(Boost) $(OKTestSystem) $(OKSystem)

link_libraries := 

Root := $(wildcard ../../..)
