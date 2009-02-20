// Oliver Kullmann, 17.11.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ERRORHANDLINGWAECHTER

#define ERRORHANDLINGWAECHTER

#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <new> // for bad_alloc
#include <typeinfo> // for bad_cast and bad_typeid
#include <ios> // for ios_base::failure

#include <OKlib/General/MessageHandling.hpp>

namespace ErrorHandling {

  class Error {
  public :
    Error(const std::string& what) : what_(what) {}
    std::string what() const { return what_; }
    virtual ~Error() {}
  protected :
    std::string what_;
  };

  const int AnzahlSprachen = 2;
  const char * const Meldungen_Error [][AnzahlSprachen] = {
      {"FEHLER (abgeleitet von ErrorHandling::Error)!", // 0
       "ERROR (derived from ErrorHandling::Error)!"},
      {"Der Typ-Identifikator des Fehler-Typs ist ", // 1
       "The type identifier of the exception type is "},
      {"Die Fehlerbeschreibung (\"what\") ist:", // 2
       "The error message (\"what\") is:"},
      {"FEHLER (abgeleitet von std::exception)!", // 3
       "ERROR (derived from std::exception)!"},
      {"Ausnahmetyp ist weder von std::logic_error noch von std::runtime_error abgeleitet.", // 4
       "Exception type is neither derived from std::logic_error nor from std::runtime_error."},
      {"Ausnahmetyp abgeleitet von std::runtime_error.", // 5
       "Exception type derived from std::runtime_error."},
      {"Ausnahmetyp abgeleitet von std::logic_error.", // 6
       "Exception type derived from std::logic_error."},
      {"std::bad_alloc wurde geworfen (Operator new war nicht erfolgreich).", // 7
       "std::bad_alloc thrown (operator new failed)."},
      {"std::bad_cast wurde geworden (vom Operator dynamic_cast).", // 8
       "std::bad_cast thrown (by operator dynamic_cast)."},
      {"std::bad_typeid wurde geworden (vom Operator typeid).", // 9
       "std::bad_typeid thrown (by operator typeid)."},
      {"std::bad_exception wurde geworden (von der Funktion unexpected()).", // 10
       "std::bad_exception thrown (by function unexpected())."},
      {"std::ios_base::failure wurde geworden.", // 11
       "std::ios_base::failure thrown."},
      {"std::range_error wurde geworden (Bereichsfehler in interner Berechnung; moeglicherweise von der Standard-Bibliothek).", // 12
       "std::range_error thrown (range error in internal computation; possibly from the Standard Library)."},
      {"std::overflow_error wurde geworden.", // 13
       "std::overflow_error thrown."},
      {"std::underflow_error wurde geworden.", // 14
       "std::underflow_error thrown."},
      {"Die geworfene Ausnahme ist keiner der Typen std::range_error, std::overflow_error oder std::underflow_error.", // 15
       "Exception thrown is none of std::range_error, std::overflow_error or std::underflow_error."},
      {"std::domain_error wurde geworden.", // 16
       "std::domain_error thrown."},
      {"std::invalid_argument wurde geworden (moeglicherweise von der Standard-Bibliothek).", // 17
       "std::invalid_argument thrown (possibly by the Standard Library)."},
      {"std::length_error wurde geworden.", // 18
       "std::length_error thrown."},
      {"std::out_of_range wurde geworden (moeglicherweise von der Standard-Bibliothek).", // 19
       "std::out_of_range thrown (possibly by the Standard Library)."},
      {"Die geworfene Ausnahme ist keiner der Typen std::domain_error, std::invalid_argument, std::length_error oder std::out_of_range.", // 20
       "Exception thrown is none of std::domain_error, std::invalid_argument, std::length_error or std::out_of_range."},
    };
 
  
  inline const std::string Error2string(const Error& e, MessageHandling::Sprachen L = MessageHandling::English) {
    MessageHandling::Messages<AnzahlSprachen> M(Meldungen_Error, L);
    std::ostringstream o;
    o << M(0) << "\n" << M(1) << typeid(e).name() << "\n" << M(2) << "\n" << e.what() << "\n";
    return o.str();
  }

  inline std::string Error2string(const std::exception& e, MessageHandling::Sprachen L = MessageHandling::English) {
    // ToDo: Using message objects in new module "Messages"
    // ToDo: Writing a concept and tests
    MessageHandling::Messages<AnzahlSprachen> M(Meldungen_Error, L);
    using namespace std;
    ostringstream o;
    o << M(3) << "\n" << M(1) << typeid(e).name() << "\n" << M(2) << "\n" << e.what() << "\n";
    if (typeid(e) == typeid(bad_alloc))
      o << M(7) << "\n";
    else if (typeid(e) == typeid(bad_cast))
      o << M(8) << "\n";
    else if (typeid(e) == typeid(bad_typeid))
      o << M(9) << "\n";
    else if (typeid(e) == typeid(bad_exception))
      o << M(10) << "\n";
    else if (typeid(e) == typeid(ios_base::failure))
      o << M(11) << "\n";
    else {
      try {
	dynamic_cast<const logic_error&>(e);
      }
      catch (...) { // not derived from logic_error
	try {
	  dynamic_cast<const runtime_error&>(e);
	}
	catch (...) { // not derived from runtime_error
	  o << M(4) << "\n";
	  goto Exit;
	} // catch runtime_error
	// derived from runtime_error
	o << M(5) << "\n";
	if (typeid(e) == typeid(range_error))
	  o << M(12) << "\n";
	else if (typeid(e) == typeid(overflow_error))
	  o << M(13) << "\n";
	else if (typeid(e) == typeid(underflow_error))
	  o << M(14) << "\n";
	else
	  o << M(15) << "\n";
	goto Exit;
      } // catch logic_error
      // derived from logic_error
      o << M(6) << "\n";
      if (typeid(e) == typeid(domain_error))
	o << M(16) << "\n";
      else if (typeid(e) == typeid(invalid_argument))
	o << M(17) << "\n";
      else if (typeid(e) == typeid(length_error))
	o << M(18) << "\n";
      else if (typeid(e) == typeid(out_of_range))
	o << M(19) << "\n";
      else
	o << M(20) << "\n";
    }
    Exit :
      return o.str();
  }
  

}


#endif
