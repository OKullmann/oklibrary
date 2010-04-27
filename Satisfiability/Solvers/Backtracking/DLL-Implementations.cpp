// Oliver Kullmann, 26.2.2002 (Swansea)
/* Copyright 2002 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/Backtracking/DLL-Implementations.cpp
  \brief Old, very simple DLL SAT solver (for boolean CNF).

  Reads a CNF in Dimacs format from standard input.

  Links with Satisfiability/Algorithms/Backtracking/DLL_Algorithms.cpp (which
  thus needs to be compiled before).

  \deprecated
*/

#include <iostream>
#include <string>
#include <set>
#include <ctime>

#include <OKlib/Satisfiability/ProblemInstances/Variables/VarSet.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/Literal.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/Clause.hpp>
#include <OKlib/Satisfiability/ProblemInstances/ClauseSets/ClauseSet.hpp>
#include <OKlib/Satisfiability/Assignments/PartialAssignments/PartAssign.hpp>
#include <OKlib/Satisfiability/Algorithms/Backtracking/DLL_Algorithms.hpp>

#include <OKlib/General/Kommandozeile.hpp>

namespace {

  using namespace Variables;
  using namespace Literals;
  using namespace Clauses;
  using namespace Clausesets;
  using namespace PartAssignments;

  const std::string Selbst = "DLL-Implementations";

  const char * const Version = "1.0.1";
  const char * const Datum = "27.4.2010";
  const char * const Autor = "Oliver Kullmann (Swansea); O.Kullmann@Swansea.ac.uk";
  const char * const Uebersetzungsdatum = __DATE__ " " __TIME__; // Gnu-Uebersetzung  
  


  KMZ::Sprachen Language = KMZ::Englisch;

  const int NumberLanguages = 2;

  const char * const messages[][NumberLanguages] = {
    {"Es wurde versucht, einen leeren Variablennamen zu benutzen.", // 0
     "There has been an attempt to use an empty variable name."},
    {"Es wurde eine Zuweisung zwischen Variablenmengen versucht.", // 1
     "There has been an attempt to do an assignment for variable sets."},
    {"Es wurde versucht, eine tautologische Klausel zu erzeugen.", // 2
     "There has been an attempt to create a tautological clause."},
    {"Es wurde versucht, den Wert einer partiellen Belegung fuer ein nicht im Definitionsbereich vorzufindendes Element zu bestimmen.", // 3
     "There has been an attempt to evaluate a partial assignment for an argument which is not in the domain."},
    {"Es wurde versucht, eine partielle Belegung in inkonsistenter Weise zu erweitern.", // 4
     "There has been an attempt to extend a partial assignment in an inconsistent way."},
    {"FEHLER", // 5
     "ERROR"},
    {"Zeitverbrauch (in Sekunden) fuer die SAT-Entscheidung = ", // 6
     "Running time (in seconds) for SAT decision = "},
    {"Zeitverbrauch (in Sekunden) fuer das Einlesen = ", // 7
     "Running time (in seconds) for reading the input = "},
  };

  const char * message(int i) {
    return messages[i][Language];
  }

  const std::string standarderr = message(5) + Selbst + "]: ";
  
  void output_litset(const Litset& L) {
    for (std::set<Lit>::iterator i = L.ls.begin(); i != L.ls.end(); i++) {
      Lit l = *i;
      std::cout << " ";
      if (l.val() == true)
	std::cout << "-";
      std::cout << l.var().get_name();
    }
    std::cout << "\n";
  }
  
  void output_varset(const std::set<Var>& V) {
    for (std::set<Var>::iterator i = V.begin(); i != V.end(); i++) {
      std::cout << " " << i -> get_name();
    }
    std::cout << "\n";
  }
 
}


int main (const int argc, const char * const argv[]) {

  enum choices {A_DLL_1};
  choices choice = A_DLL_1; // default
  if (argc >= 2) {
    if (std::string(argv[1]) == "DLL_1")
      choice = A_DLL_1;
  }
  DLL_Algorithms::SAT_Algorithms* algorithms[] = {DLL_Algorithms::DLL_1};

  Var_Set V;
  Cls F;

  try {
    std::clock_t time_stored, time_new;

    time_stored = std::clock();
    F.read(V);
    time_new = std::clock();

    std::cout << message(7) << (double) (time_new - time_stored) / CLOCKS_PER_SEC << "\n";

    std::cout << "n = " << F.n() << ", c = " << F.c() << ", l = " << F.l() << ", pmin = " << F.pmin() << ", pmax = " << F.pmax() << "\n";

    time_stored = std::clock();
    DLL_Algorithms::Result r = algorithms[choice](F);
    time_new = std::clock();
    
    std::cout << "sat = " << r.sat << "\n";
    std::cout << message(6) << (double) (time_new - time_stored) / CLOCKS_PER_SEC << "\n";
  }

  catch (Variables::Error::empty_name) {
    std::cout << standarderr << message(0) << "\n";
    return 1;
  }
  catch (Variables::Error::invalid_assignment) {
    std::cout << standarderr << message(1) << "\n";
    return 1;
  }
  catch (Clauses::Error::not_a_clause) {
    std::cout << standarderr << message(2) << "\n";
    return 1;
  }
  catch (PartAssignments::Error::not_in_domain) {
    std::cout << standarderr << message(3) << "\n";
    return 1;
  }
  catch (PartAssignments::Error::inconsistent_extension) {
    std::cout << standarderr << message(4) << "\n";
    return 1;
  }
}
