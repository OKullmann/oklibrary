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

OKTestSystem := -I$(OKSystem)/OKlib/TestSystem

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestBibliothek

programs := NumberTheory_Application_gcd Algebra_Applications_ModulareExponentiation TimeHandling_Applications_DaysDifference TimeHandling_Applications_DifferenceList Algebra_Applications_RSA

source_libraries = $(OKTestSystem) $(Xerces_h) $(Loki) $(boost_include_option_okl) 

link_libraries := $(boost_link_option_okl) -lboost_date_time $(Xerces_so)


