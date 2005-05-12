# Oliver Kullmann, 11.5.2005 (Swansea)

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
ifdef BOOST
  Boost := -I$(BOOST)
else
  Boost :=
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

General_options :=
Optimisation_options := -O3

test_program := TestGenerator

core_programs = OKgenerator interprete_descriptor

programs := $(core_programs) experiment_preparation process_experiment transfer_experiment

source_libraries := $(Bibliothek) $(Xerces_h) $(Loki) $(Boost)

link_libraries := -lpq

Root := $(wildcard ../../../../)
