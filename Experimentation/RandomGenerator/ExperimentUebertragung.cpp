// Oliver Kullmann, 9.4.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/ExperimentUebertragung.cpp
  \brief Implementations regarding transferring an experiment to the database
*/


#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include "ExperimentUebertragung.hpp"
#include "DatabaseHandler01.hpp"
#include "Tools.hpp"
#include "Kommandozeile.hpp"
#include "TimeHandling.hpp"


namespace {

  using namespace std;

  using namespace ExperimentUebertragung;

  char *Selbst = "transfer_experiment";

  const int number_languages = 2;
  KMZ::Sprachen language = KMZ::Englisch;

  const char * const Version = "1.45";
  const char * const Datum = "13.7.2002";
  const char * const Autor = "Oliver Kullmann (Swansea); O.Kullmann@Swansea.ac.uk";
  const char * const Uebersetzungsdatum = __DATE__ " " __TIME__;

  const char * const Meldungen[][number_languages] = {
    {"%s, Version %s vom %s; Autor  %s; Uebersetzungsdatum %s\n", // 0
     "%s, Version %s from %s; author %s; compilation date %s\n"},
    {"Programmname, Versionsnummer, Fertigstellungsdatum, Autor (mit e-Post-Adresse), Uebersetzungsdatum", // 1
     "Name of program, version number, date of completion, author (with e-mail-address), compilation date"},
    {"", // 2
     ""},
    {"Kommandozeilen-Fehler:", // 3
     "Command line error:"},
    {"Der (bearbeitete) Experiment-Ordner (mit der Datei \"experiment.xml\").", // 4
     "The (processed) experiment directory (with the file \"experiment.xml\")."},
    {"Das Experiment ist noch nicht in die Datenbank eingetragen worden.", // 5
     "The experiment has not been entered in the database."},
    {"Loeser-Autor", // 6
     "Solver author"},
    {"SAT-Loeser", // 7
     "SAT solver"},
    {"Rechner", // 8
     "Computer"},
    {"Experiment", // 9
     "Experiment"},
    {"In die Datenbank wird nur eingetragen, ob die Klauselmengen erfuellbar oder onerfuellbar sind", // 10
     "Only whether the formulas are satisfiable or not is entered into the database."},
    {"Falls die Klauselmenge schon in der Datenbank vorhanden ist, so wird das entsprechende Resultat ignoriert.", // 11
     "In case the clause-set is already present (possibly for another solver/computer), then that result is ignored."},
    {"Erfolgreiche Uebertragung von Experiment", // 12
     "Successful transfer of experiment"},
    {"Anzahl der Resultate insgesamt:", // 13
     "Number of results at all:"},
    {"Anzahl der uebertragenen Ergebnisse:", // 14
     "number of submitted results:"},
    {"gefunden", // 15
     "found"},
    {"Keine Rechenzeiten werden in die Datenbank eingetragen. (Negative Rechenzeiten werden immer ignoriert; diese Option ist fuer den Fall, dass auch positive Ergebnisse falsch sein koennten.)", // 16
     "Running times are not entered into the database. (Negative running times are always ignored; this option is for the case, where also positive running times could be false.)"},
    {"Information zu neuen Klauseln werden nicht in die Datenbank eingetragen.", // 17
     "Information about new clauses are not entered into the database."},
    {"Von der XML-Bibliothek:", // 18
     "From the XML library:"},
    {"Von der DatabaseHandler01-Schnittstelle:", // 19
     "From the DatabaseHandler01 interface:"},
    {"Von SAX-Parser:", // 20
     "From the SAX parser:"},
    {"Von der Tools-Bibliothek:", // 21
     "From the Tools-library:"},
    {"Von ExperimentUebertragung:", // 22
     "From ExperimentUebertragung:"},
    {"Anzahl gelesener Resultate: ", // 23
     "Number of results read: "},
    {"Geschaetzte verbleibende Zeit: ", // 24
     "Estimated remaining time: "},
    {"FEHLER", // 25
     "ERROR"},
    {"genauere Informationen wie folgt", // 26
     "error messages as follows"},
    {"mit Experiment", // 27
     "with experiment"},
    {"Normalerweise wird getestet, ob eine Klauselmenge oder zugehoerige Information schon in der Datenbank vorhanden ist, in welchem Falle dann nichts geschieht. Dieser Test wird nun ausgelassem (Doppeleintraege fuehren nun zu Fehlern und Abbruch).", // 28
     "By default it is tested, whether a clause-set or related information is already in the database, in which case then the item is ignored. Now this test is cancelled (and double entries now lead to errors and program abbort)."},
    {"Beginn der \"Ubertragung in die Datenbank", // 29
     "Start of transfer of results into the database"},
    {"um", // 30
     "at"},
    {"Beginn Phase 1", // 31
     "Begin Phase 1"},
    {"Fuer Phase 1 wurden", // 32
     "For phase 1"},
    {"Kern-Rechenzeit und", // 33
     "core running time has been used, and"},
    {"absolute Rechenzeit benoetigt.", // 34
     "absolute running time."},
    {"Beginn Phase 2", // 35
     "Begin Phase 2"},
    {"Fuer Phase 2 wurden", // 36
     "For phase 2"},
    {"Insgesamt", // 37
     "Total"},
    {"Informationen zu Autarkien einer Stufe 1 oder groesser werden nicht uebertragen.", // 38
     "Data about autarkies of level 1 or greater are not transferred."},
  };

}


namespace {

  using namespace ExperimentUebertragung;

  using namespace std;

  using namespace xercesc;

  const string OKDatabaseName = "OKRandGen";

  bool sat_only = false;
  bool unique = false;
  bool no_time = false;
  bool no_new_clauses = false;
  bool guaranteed_new = false;
  bool no_1autarky = false;

  unsigned int number_results; // determined in phase 1
  unsigned int number_submitted_results; // in phase 2
  unsigned int number_read_results; // in phase 2 (for error messages)
  const unsigned int progress_skip = 1000;

  const int Index0 = 0; // to make sure, 0 ist not interpreted as pointer

  inline string x2s (const XMLCh* const X) {
    const char * Z = XMLString::transcode(X);
    string E(Z);
    delete [] Z;
    return E;
  }

  TimeHandling::SystemTime AnfangPhase1(false);
  TimeHandling::SystemTime AnfangPhase2(false);
  TimeHandling::WallTime Phase1absolut(true);
  TimeHandling::WallTime Phase2absolut(false);


  // tools for string handling -----------------------------------------

  inline unsigned int x2ui(const XMLCh* const X) {
    const string sX = x2s(X);
    istringstream s(sX);
    unsigned int result;
    s >> result;
    if (s.fail())
      throw Error::not_a_number(sX);
    return result;
  }
  inline unsigned long int x2uli(const XMLCh* const X) {
    const string sX = x2s(X);
    istringstream s(sX);
    unsigned long int result;
    s >> result;
    if (s.fail())
      throw Error::not_a_number(sX);
    return result;
  }

  inline int x2i(const XMLCh* const X) {
    const string sX = x2s(X);
    istringstream s(sX);
    int result;
    s >> result;
    if (s.fail())
      throw Error::not_an_integer(sX);
    return result;
  }

  inline double x2d(const XMLCh* const X) {
    const string sX = x2s(X);
    istringstream s(sX);
    double result;
    s >> result;
    if (s.fail())
      throw Error::not_a_number(sX);
    return result;
  }
  
  inline ostream& operator << (ostream& S, const XMLCh* const X) {
    return (S << x2s(X));
  }


  // the error handler (used in both phases) -----------------------------


  class ErrMeldHandler : public HandlerBase {
    void error(const SAXParseException& f) { throw f; }
    // sonst geschieht nichts bei "nicht-fatalen" Fehlern;
    void warning(const SAXParseException& f) { throw f; }
    // ebenfalls hier wuerde sonst nichts geschehen;
  };


  // the parse-handler for phase 1 ---------------------------------------


  class Durchgang1Handler : public HandlerBase {

  public :

    Durchgang1Handler()
      : DH(OKDatabaseName),

	TExperiments(DH, "experiments"),
	Aexp_id(TExperiments, "exp_id"),
	Adate_b(TExperiments, "date_begin"),
	Adate_e(TExperiments, "date_end"),
	AEcomp_id(TExperiments, "comp_id"),
	AEsolv_id(TExperiments, "solv_id"),
	Alocal(TExperiments, "is_local"),

	TComputers(DH, "computers"),
	ACcomp_id(TComputers, "comp_id"),
	ACname(TComputers, "name"),
	Adomain(TComputers, "domain"),
	Aregistration(TComputers, "registration"),
	Acpu_name(TComputers, "cpu_name"),
	Acpu_mhz(TComputers, "cpu_mhz"),
	Acachel1(TComputers, "cache_l1_kb"),
	Acachel2(TComputers, "cache_l2_kb"),
	Amain_mem(TComputers, "main_mem_mb"),
	Asec_mem_type(TComputers, "sec_mem_type"),
	Asec_mem_gb(TComputers, "sec_mem_gb"),
	Amodel(TComputers, "model"),

	TSolvers(DH, "solvers"),
	ASsolv_id(TSolvers, "solv_id"),
	Abase_name(TSolvers, "base_name"),
	Aexec_name(TSolvers, "exec_name"),
	Aversion(TSolvers, "version"),
	Aoptions(TSolvers, "options"),
	ASperson_id(TSolvers, "person_id"),
	Ainternal(TSolvers, "internal"),

	TPersons(DH, "persons"),
	APperson_id(TPersons, "person_id"),
	Afirst_name(TPersons, "first_name"),
	Alast_name(TPersons, "last_name"),
	Acountry(TPersons, "country"),
	Ae_mail(TPersons, "e_mail"),
	Awww(TPersons, "www"),

	number_tables(4)

    {
      Alocal << "true";
      menu_tables.reserve(number_tables);
      menu_tables[tp] = make_pair(6, &TPersons);
      menu_tables[ts] = make_pair(7, &TSolvers);
      menu_tables[tc] = make_pair(8, &TComputers);
      menu_tables[te] = make_pair(9, &TExperiments);
    }

    void startElement(const XMLCh* const name, AttributeList& att) {
      const string na = x2s(name);

      if (na == "experiment-begin" or na == "experiment-end") {
	Tools::Timestamp t;
	for (int i = 0; i < att.getLength(); i++) {
	  const string attname = x2s(att.getName(i));
	  if (attname == "time_zone")
	    t.set_time_zone(x2s(att.getValue(i)));
	  else
	    t.set_field(attname, x2ui(att.getValue(i)));
	}
	if (na == "experiment-begin")
	  Adate_b << string(t);
	else
	  Adate_e << string(t);
      }

      else if (na == "special_instance_experiment" or na == "other_random_experiment" or na == "created" or na == "transformed" or na == "combinatorial" or na == "other_random")
	throw Error::not_implemented(na);

      else if (na == "year_of_purchase") {
	const XMLCh* yp = att.getValue("year");
	if (yp == 0)
	  throw Error::missing_attribute("year_of_purchase value");
	const int year = x2i(yp);
	const XMLCh* sp = att.getValue("specifier");
	Tools::bounds<int>::specifier spec;
	if (sp == 0)
	  spec = Tools::bounds<int>::upper;
	else {
	  const string sps = x2s(sp);
	  if (sps == "lower_bound")
	    spec =  Tools::bounds<int>::lower;
	  else if (sps == "upper_bound")
	    spec = Tools::bounds<int>::upper;
	  else if (sps == "exact")
	    spec = Tools::bounds<int>::exact;
	  else
	    throw Error::wrong_attribute_value("year_of_purchase specifier " + sps);
	}
	year_purchase.set(spec, year);
      }

      else if (na == "computer-name") {
	if (const XMLCh* np = att.getValue("name"))
	  ACname << x2s(np);
	else
	  throw Error::missing_attribute("computer-name name");
	if (const XMLCh* dp = att.getValue("domain"))
	  Adomain << x2s(dp);
      }

      else if (na == "computer-id") {
	if (const XMLCh* const vp = att.getValue("value")) {
	  const int v = x2i(vp);
	  ACcomp_id << v;
	  AEcomp_id << v;
	}
	else
	  throw Error::missing_attribute("computer-id value");
      }

      else if (na == "model") {
	if (const XMLCh* const np = att.getValue("name"))
	  if (const XMLCh* const mp = att.getValue("manufacturer"))
	    Amodel << (x2s(mp) + " " + x2s(np));
	  else
	    throw Error::missing_attribute("model manufacturer");
	else
	  throw Error::missing_attribute("model name");
      }

      else if (na == "cpu") {
	if (const XMLCh* const np = att.getValue("name"))
	  Acpu_name << x2s(np);
	if (const XMLCh* const mp = att.getValue("mhz"))
	  Acpu_mhz << x2d(mp);
      }

      else if (na == "cache") {
	enum {L1, L2} level = L1;
	if (const XMLCh* const lp = att.getValue("type")) {
	  const string lv = x2s(lp);
	  if (lv == "L1")
	    level = L1;
	  else if (lv == "L2")
	    level = L2;
	  else
	    throw Error::wrong_attribute_value("cache type " + lv);
	}
	if (const XMLCh* const sp = att.getValue("size_kb"))
	  switch (level) {
	  case L1 :
	    Acachel1 << x2i(sp); break;
	  case L2 :
	    Acachel2 << x2i(sp); break;
	  }
	else
	  throw Error::missing_attribute("cache size_kb");
      }

      else if (na == "main_memory") {
	if (const XMLCh* const sp = att.getValue("size_mb"))
	  Amain_mem << x2i(sp);
      }

      else if (na == "secondary_memory") {
	if (const XMLCh* const sp = att.getValue("size_gb"))
	  Asec_mem_gb << x2s(sp);
	if (const XMLCh* const tp = att.getValue("type"))
	  Asec_mem_type << x2s(tp);
      }

      else if (na == "solver-name") {
	if (const XMLCh* const bp = att.getValue("base-name"))
	  Abase_name << x2s(bp);
	else
	  throw Error::missing_attribute("solver-name base-name");
	if (const XMLCh* const fp = att.getValue("extended_name"))
	  Aexec_name << x2s(fp);
	if (const XMLCh* const vp = att.getValue("version"))
	  Aversion << x2s(vp);
      }

      else if (na == "solver-author") {
	if (const XMLCh* const lp = att.getValue("last_name"))
	  Alast_name << x2s(lp);
	else
	  throw Error::missing_attribute("solver-author last_name");
	if (const XMLCh* const fp = att.getValue("first_name"))
	  Afirst_name << x2s(fp);
	if (const XMLCh* const cp = att.getValue("country"))
	  Acountry << x2s(cp);
	if (const XMLCh* const ep = att.getValue("e-mail"))
	  Ae_mail << x2s(ep);
	if (const XMLCh* const wp = att.getValue("www"))
	  Awww << x2s(wp);
      }

      else if (na == "solver-id") {
	if (const XMLCh* const vp = att.getValue("value")) {
	  const string v = x2s(vp);
	  AEsolv_id << v;
	  ASsolv_id << v;
	}
	else
	  throw Error::missing_attribute("solver-id value");
      }

      else if (na == "url") {
	if (const XMLCh* const fp = att.getValue("for")) {
	  if (x2s(fp) == "executable")
	    if (const XMLCh* const vp = att.getValue("value"))
	      Aexec_name << Tools::core_name(x2s(vp));
	    else
	      throw Error::missing_attribute("url value");
	}
	else
	  throw Error::missing_attribute("url for");
      }
      
      else if (na == "options") {
	if (const XMLCh* const op = att.getValue("string"))
	  Aoptions << x2s(op);
	else
	  throw Error::missing_attribute("options string");
      }

      else if (na == "internal") {
	Ainternal << "true";
      }
      
      else if (na == "experiment-id") {
	if (const XMLCh* const vp = att.getValue("value"))
	  Aexp_id << x2s(vp);
	else
	  throw Error::missing_attribute("experiment-id value");
      }

      else if (na == "result") {
	++number_results;
      }
    }


    void endDocument() {
      bool change;
 
      do {
	do {
	  change = false;

	  if (TPersons.changed()) {
	    TPersons.reset_changed();
	    TPersons.completion();
	    if (TPersons.changed()) {
	      change = true;
	      if (! APperson_id.getValue().null) {
		ASperson_id << APperson_id.getValue().v;
	      }
	    }
	  }
	  if (TSolvers.changed()) {
	    TSolvers.reset_changed();
	    TSolvers.completion();
	    if (TSolvers.changed()) {
	      change = true;
	      if (! ASsolv_id.getValue().null) {
		AEsolv_id << ASsolv_id.getValue().v;
	      }
	    }
	  }
	  if (TComputers.changed()) {
	    TComputers.reset_changed();
	    TComputers.completion();
	    if (TComputers.changed()) {
	      change = true;
	      if (! ACcomp_id.getValue().null) {
		AEcomp_id << ACcomp_id.getValue().v;
	      }
	    }
	  }
	  if (TExperiments.changed()) {
	    TExperiments.reset_changed();
	    TExperiments.completion();
	    if (TExperiments.changed()) {
	      change = true;
	    }
	  }
	} while (change);

	show_data();
	if (! Aexp_id.getValue().null) {
	  exp_id = Aexp_id.getValue().v;
	  return;
	}
	{
	  bool new_entries = false;
	  for (unsigned int ti = 0; ti != number_tables; ti++)
	    if (menu_tables[ti].second -> get_last_nresults() == 0) {
	      new_entries = true;
	      break;
	    }
	  if (! new_entries)
	    throw Error::no_experiment_id();
	}
	cout << Meldungen[5][language] << endl;
	if (! submit_data())
	  throw Error::no_experiment_id();
      }
      while (true);
    }

    
  private :

    friend string extract_exp_id(const Durchgang1Handler&);

    DatabaseHandler01::DatabaseHandler DH;

    DatabaseHandler01::TableHandler TExperiments;
    DatabaseHandler01::AttributeObject Aexp_id;
    DatabaseHandler01::AttributeObject Adate_b;
    DatabaseHandler01::AttributeObject Adate_e;
    DatabaseHandler01::AttributeObject AEcomp_id;
    DatabaseHandler01::AttributeObject AEsolv_id;
    DatabaseHandler01::AttributeObject Alocal;

    DatabaseHandler01::TableHandler TComputers;
    DatabaseHandler01::AttributeObject ACcomp_id;
    DatabaseHandler01::AttributeObject ACname;
    DatabaseHandler01::AttributeObject Adomain;
    DatabaseHandler01::AttributeObject Aregistration;
    DatabaseHandler01::AttributeObject Acpu_name;
    DatabaseHandler01::AttributeObject Acpu_mhz;
    DatabaseHandler01::AttributeObject Acachel1;
    DatabaseHandler01::AttributeObject Acachel2;
    DatabaseHandler01::AttributeObject Amain_mem;
    DatabaseHandler01::AttributeObject Asec_mem_type;
    DatabaseHandler01::AttributeObject Asec_mem_gb;
    DatabaseHandler01::AttributeObject Amodel;

    DatabaseHandler01::TableHandler TSolvers;
    DatabaseHandler01::AttributeObject ASsolv_id;
    DatabaseHandler01::AttributeObject Abase_name;
    DatabaseHandler01::AttributeObject Aexec_name;
    DatabaseHandler01::AttributeObject Aversion;
    DatabaseHandler01::AttributeObject Aoptions;
    DatabaseHandler01::AttributeObject ASperson_id;
    DatabaseHandler01::AttributeObject Ainternal;

    DatabaseHandler01::TableHandler TPersons; // at this time only for solvers
    DatabaseHandler01::AttributeObject APperson_id;
    DatabaseHandler01::AttributeObject Afirst_name;
    DatabaseHandler01::AttributeObject Alast_name;
    DatabaseHandler01::AttributeObject Acountry;
    DatabaseHandler01::AttributeObject Ae_mail;
    DatabaseHandler01::AttributeObject Awww;

    Tools::bounds<int> year_purchase;

    string exp_id;

    const unsigned int number_tables;
    enum table_numbers {tp, ts, tc, te};
    vector< pair<int, DatabaseHandler01::TableHandler*> > menu_tables;

    void show_table(const DatabaseHandler01::TableHandler* t) const {
      typedef map<string, DatabaseHandler01::Value> sv;
      sv entries = t -> get_values();
      for (sv::iterator i = entries.begin(); i != entries.end(); ++i)
	cout << i -> first << " = " << i -> second << "\n";
    }
    void show_data() const {
      for (unsigned int ti = 0; ti != number_tables; ti++) {
	cout << "\n" << Meldungen[menu_tables[ti].first][language] << ": " << menu_tables[ti].second -> get_last_nresults() << " " << Meldungen[15][language] << "\n";
	show_table(menu_tables[ti].second);
      }
      cout << "\n";
    }
    bool submit_data() {
      for (unsigned int ti = 0; ti != number_tables; ++ti)
	if (menu_tables[ti].second -> get_last_nresults() == 0)
	  cout << "(" << ti << ") " << Meldungen[menu_tables[ti].first][language] << "\n";
      unsigned int input;
      cin >> input;
      if (input < number_tables and menu_tables[input].second -> get_last_nresults() == 0)
	if (input == tc) {
	  Aregistration << "now";
	  menu_tables[input].second -> submit();
	  Aregistration.clear();
	}
	else {
	  menu_tables[input].second -> submit();
	  return true;
	}
      else
	return false;
    }

  };
 
  string extract_exp_id(const Durchgang1Handler& h) {
    return h.exp_id;
  }


  // Auxiliary class to create all attribute objects for a table

  class AllAOs {

  public :

    struct wrong_name : Error::Internal_error {
      wrong_name(const string& s) : Internal_error(s) {}
    };

    typedef map<string, DatabaseHandler01::AttributeObject*> St2AOP;

    AllAOs(DatabaseHandler01::TableHandler& th) {
      typedef map<string, DatabaseHandler01::Attribute> St2At;
      const St2At& attlist(th.get_attributes());
      St2AOP::iterator pos(aos.begin());
      for (St2At::const_iterator i = attlist.begin(); i != attlist.end(); i++)
	pos = aos.insert(pos, make_pair(i -> first, new DatabaseHandler01::AttributeObject(th, i -> first)));
    }

    DatabaseHandler01::AttributeObject& operator() (const string& name) const {
      const St2AOP::const_iterator i = aos.find(name);
      if (i == aos.end())
	throw wrong_name(name);
      return *(i -> second);
    }

    ~AllAOs() {
      for_each(aos.begin(), aos.end(), Tools::DeleteObjectSecond());
    }

  private:

    St2AOP aos;
  };


  // the parse-handler for phase 2 ---------------------------------------


  class Durchgang2Handler : public HandlerBase {

  public :

    Durchgang2Handler(const string& experiment_id)
      : exp_id(experiment_id),
	DH(OKDatabaseName),

	TFrames(DH, "frames"), AFrames(TFrames),
	Tbcls(DH, "bcls"), Abcls(Tbcls),
	Tbcls_info(DH, "bcls_info"), Abcls_info(Tbcls_info),
	Tb_unit_reductions(DH, "b_unit_reductions"), Ab_unit_reductions(Tb_unit_reductions),
	Tb_autarky_reductions(DH, "b_autarky_reductions"), Ab_autarky_reductions(Tb_autarky_reductions),
	Tb_new_clauses(DH, "b_new_clauses"), Ab_new_clauses(Tb_new_clauses),
	command(DH), select(DH),
	ignore(false),
	bcls_submitted(false),
	Progress(cout, number_results, progress_skip, Meldungen[23][language], Meldungen[24][language])
    {

    }

    // Begin of element --------------------------------------------------

    void startElement(const XMLCh* const name, AttributeList& att) {
      const string na = x2s(name);

      if (na == "complete_result") {
	if (ignore)
	  sat_only = false;
	ignore = false;
	bcls_submitted = false;
	command.issue("BEGIN TRANSACTION;");
      }

      else if (na == "OKgenerator") {
	if (att.getValue("domain_size"))
	  throw Error::not_implemented(na + " domain_size");
	if (const XMLCh* p = att.getValue("seed_0"))
	  Abcls("key0") << Tools::uint4_to_int4(x2ui(p));
	if (const XMLCh* p = att.getValue("seed_1"))
	  Abcls("key1") << Tools::uint4_to_int4(x2ui(p));
	if (const XMLCh* p = att.getValue("number_0"))
	  Abcls("nr0") << Tools::uint4_to_int4(x2ui(p));
	if (const XMLCh* p = att.getValue("number_1"))
	  Abcls("nr1") << Tools::uint4_to_int4(x2ui(p));
	if (const XMLCh* p = att.getValue("variables")) {
	  Abcls("n") << x2s(p);
	  n = x2i(p);
	}
	else
	  throw Error::missing_attribute(na + " variables");
	if (n <= 0) {
	  ostringstream s; s << n;
	  throw Error::non_positive_n(s.str());
	}
      }

      else if (na == "g_number") {
	if (const XMLCh* lp = att.getValue("length"))
	  if (const XMLCh* cp = att.getValue("count")) {
	    const int l = x2i(lp);
	    if (l <= 0)
	      throw Error::non_positive_l(x2s(lp));
	    const int c = x2i(cp);
	    if (c <= 0)
	      throw Error::non_positive_c(x2s(cp));
	    l2std[l] = Tools::standardised_quotient(c, n);
	  }
	  else
	    throw Error::missing_attribute(na + " count");
	else
	  throw Error::missing_attribute(na + " length");
      }

      else if (na == "result") {
	if (const XMLCh* p = att.getValue("value")) {
	  const string s = x2s(p);
	  if (s == "unsatisfiable")
	    Abcls("sat") << "false";
	  else if (s == "satisfiable")
	    Abcls("sat") << "true";
	  else { // ignore this (complete) result
	    if (! sat_only) {
	      sat_only = true;
	      ignore = true;
	    }
	    return;
	  }

	  if (guaranteed_new or ! Tbcls.completion()) { // assumes that dim and m_dens_id are known by now
	    Tbcls.submit();
	    Tbcls.completion();
	    bcls_submitted = true;
	  }
	  else {
	    if (unique and not sat_only) { // ignore this (complete) result
	      sat_only = true;
	      ignore = true;
	      return;
	    }
	  }
	  cls_id = Abcls("cls_id").getValue().v;
	  // now cls_id is known
	}
	else
	  throw Error::missing_attribute(na + " value");
      }

      else if (na == "processing-description" and ! sat_only) {
	Abcls_info("cls_id") << cls_id;
	Abcls_info("exp_id") << exp_id;
      }

      else if (na == "running-time" and ! sat_only and ! no_time) {
	if (const XMLCh* tp = att.getValue("type")) {
	  if (x2s(tp) == "core")
	    if (const XMLCh* sp = att.getValue("sec")) {
	      if (x2d(sp) >= 0)
		Abcls_info("sec") << x2s(sp);
	    }
	    else
	      throw Error::missing_attribute(na + " sec");
	}
	else
	  throw Error::missing_attribute(na + " type");
      }

      else if (na == "tree-depth" and ! sat_only) {
	if (const XMLCh* tp = att.getValue("type")) {
	  if (x2s(tp) == "maximal")
	    if (const XMLCh* cp = att.getValue("count"))
	      Abcls_info("tree_depth") << x2s(cp);
	    else
	      throw Error::missing_attribute(na + " count");
	}
	else
	  throw Error::missing_attribute(na + " type");
      }

      else if (na == "autarky-reductions" and ! sat_only) {
	if (const XMLCh* wp = att.getValue("where")) {
	  if (x2s(wp) == "search-tree")
	    if (const XMLCh* cp = att.getValue("count"))
	      if (const XMLCh* lp = att.getValue("level")) {
		const string l = x2s(lp);
		if (l == "pure")
		  Abcls_info("pure_literals") << x2s(cp);
		else if (l == "non-pure")
		  Abcls_info("real_autarkies") << x2s(cp);
	      }
	      else
		throw Error::missing_attribute(na + " level");
	    else
	      throw Error::missing_attribute(na + " count");
	}
	else
	  throw Error::missing_attribute(na + " where");
      }

      else if (na == "nodes" and ! sat_only) {
	if (const XMLCh* cp = att.getValue("count")) {
	  const unsigned long int c = x2uli(cp);
	  if (const XMLCh* tp = att.getValue("type")) {
	    const string t = x2s(tp);
	    if (t == "node")
	      Abcls_info("nodes") << c;
	    else if (t == "single")
	      Abcls_info("single_nodes") << c;
	    else if (t == "quasi-single")
	      Abcls_info("quasi_s_nodes") << c;
	    else if (t == "missed_single")
	      Abcls_info("missed_s_nodes") << c;
	  }
	  else
	    throw Error::missing_attribute(na + " type");
	}
	else
	  throw Error::missing_attribute(na + " count");
      }

      else if (na == "generalised_unit_reductions" and ! sat_only) {
	Ab_unit_reductions("b_info_id") << b_info_id;
	if (const XMLCh* wp = att.getValue("where")) {
	  if (x2s(wp) == "search-tree")
	    if (const XMLCh* cp = att.getValue("count"))
	      if (const XMLCh* lp = att.getValue("level")) {
		Ab_unit_reductions("u_level") << x2i(lp);
		Ab_unit_reductions("u_count") << x2uli(cp);
		if (guaranteed_new or ! Tb_unit_reductions.completion())
		  Tb_unit_reductions.submit();
		Ab_unit_reductions("u_level").clear();
		Ab_unit_reductions("u_count").clear();
	      }
	      else
		throw Error::missing_attribute(na + " level");
	    else
	      throw Error::missing_attribute(na + " count");
	}
	else
	  throw Error::missing_attribute(na + " where");
      }

       else if (na == "generalised_autarky-reductions" and ! sat_only and ! no_1autarky) {
	Ab_autarky_reductions("b_info_id") << b_info_id;
	if (const XMLCh* wp = att.getValue("where")) {
	  if (x2s(wp) == "search-tree")
	    if (const XMLCh* sp = att.getValue("specifier")) {
	      if (x2s(sp) == "total")
		if (const XMLCh* cp = att.getValue("count"))
		  if (const XMLCh* lp = att.getValue("level")) {
		    Ab_autarky_reductions("a_level") << x2i(lp);
		    Ab_autarky_reductions("a_count") << x2uli(cp);
		    if (guaranteed_new or ! Tb_autarky_reductions.completion())
		      Tb_autarky_reductions.submit();
		    Ab_autarky_reductions("a_level").clear();
		    Ab_autarky_reductions("a_count").clear();
		  }
		  else
		    throw Error::missing_attribute(na + " level");
		else
		  throw Error::missing_attribute(na + " count");
	    }
	    else
	      throw Error::missing_attribute(na + " specifier");
	}
	else
	  throw Error::missing_attribute(na + " where");
       }

      else if (na == "numbers_of_new_clauses" and ! sat_only and ! no_new_clauses) {
	Ab_new_clauses("b_info_id") << b_info_id;
	if (const XMLCh* tp = att.getValue("type"))
	  if (const XMLCh* sp = att.getValue("specifier")) {
	    const string t = x2s(tp); const string s = x2s(sp);
	    if (t == "local")
	      Ab_new_clauses("type") << "l";
	    else if (t == "global")
	      Ab_new_clauses("type") << "g";
	    else
	      throw Error::wrong_attribute_value(na + " type = " + t);
	    if (s == "maximum")
	      Ab_new_clauses("specifier") << "m";
	    else if (s == "total")
	      Ab_new_clauses("specifier") << "t";
	    else
	      throw Error::wrong_attribute_value(na + " specifier = " + s);
	  }
	  else
	    throw Error::missing_attribute(na + " specifier");
	else
	  throw Error::missing_attribute(na + " type");
      }

      else if (na == "nc_number" and ! sat_only and ! no_new_clauses) {
	const XMLCh* sp = att.getValue("specifier");
	if (! sp or x2s(sp) == "exact")
	  if (const XMLCh* lp = att.getValue("length"))
	    if (const XMLCh* cp = att.getValue("count")) {
	      Ab_new_clauses("length") << x2i(lp);
	      Ab_new_clauses("n_count") << x2uli(cp);
	      if (guaranteed_new or ! Tb_new_clauses.completion())
		Tb_new_clauses.submit();
	      Ab_new_clauses("length").clear();
	      Ab_new_clauses("n_count").clear();
	    }
	    else
	      throw Error::missing_attribute(na + " count");
	  else
	    throw Error::missing_attribute(na + " length");
      }

      else if (na == "measures" and ! sat_only) {
	no_reduction = false;
      }

      else if (na == "none" and ! sat_only)
	no_reduction = true;
  
      else if (na == "number_of_variables" and ! sat_only) {
	const XMLCh* sp = att.getValue("specifier");
	if (! sp or x2s(sp) == "exact")
	  if (const XMLCh* cp = att.getValue("count"))
	    Abcls_info("an") << x2i(cp);
	  else
	    throw Error::missing_attribute(na + " count");
      }

    }

    // End of element ----------------------------------------------------

    void endElement(const XMLCh* const name) {
      const string na = x2s(name);

      if (na == "OKgenerator") {
	const int dimi = l2std.size();
	AFrames("dim") << dimi;
	dim = AFrames("dim").getValue().v;
	Abcls("dim") << dim;
	{
	  ostringstream p;
	  typedef Tools::map_constIteratorFirst<int, string> itf;
	  copy(itf(l2std.begin()), itf(l2std.end()), ostream_iterator<int>(p, ","));
	  string p2 = p.str();
	  p2.erase(p2.rfind(",")); // l2std ist not empty
	  AFrames("l") << "{" + p2 + "}";
	}
	if (! TFrames.completion()) {
	  TFrames.submit();
	  TFrames.completion();
	}
	frame_id = AFrames("frame_id").getValue().v;
	const string bd = "bd_" + dim;
	if (DH.getTableNames().count(bd) == 0) {
	  string cbd = "CREATE TABLE " + bd + "(m_dens_id SERIAL NOT NULL,frame_id INT REFERENCES Frames,CHECK(frame_id IS NOT NULL),";
	  for (int i = 1; i <= dimi; i++) {
	    ostringstream s; s << i;
	    cbd += "d" + s.str() + " NUMERIC(20,10) NOT NULL,CHECK(d" + s.str() + ">0),";
	  }
	  cbd += "PRIMARY KEY(m_dens_id),UNIQUE(frame_id";
	  for (int i = 1; i <= dimi; i++) {
	    ostringstream s; s << i;
	    cbd += ",d" + s.str();
	  }
	  cbd += "));";
	  command.issue(cbd);
	  DH.refreshTableNames();
	}
	DatabaseHandler01::TableHandler Tbd(DH, bd);
	const AllAOs Abd(Tbd);
	Abd("frame_id") << frame_id;
	{
	  int j = 1;
	  for (map<int, string>::const_iterator i = l2std.begin(); i != l2std.end(); i++, j++) {
	  ostringstream p; p << j;
	  Abd("d" + p.str()) << i -> second;
	  }
	}
	if (! Tbd.completion()) {
	  Tbd.submit();
	  Tbd.completion();
	}
	m_dens_id = Abd("m_dens_id").getValue().v;
	Abcls("m_dens_id") << m_dens_id;
      }


      else if (na == "basic") {
	if (guaranteed_new or ! Tbcls_info.completion()) {
	  Tbcls_info.submit();
	  Tbcls_info.completion();
	}
	b_info_id = Abcls_info("b_info_id").getValue().v;
	// now b_info_id is known
      }


      else if (na == "numbers_of_new_clauses" and ! sat_only and ! no_new_clauses) {
	Ab_new_clauses("type").clear();
	Ab_new_clauses("specifier").clear();
      }

      else if (na == "measures" and ! sat_only) {
	if (no_reduction and ! Abcls_info("an").getValue().null)
	  command.issue("UPDATE bcls_info SET an = " + string(Abcls_info("an").getValue()) + " WHERE b_info_id = " + b_info_id + ";");
      }

      else if (na == "complete_result") {

	command.issue("END TRANSACTION;");
	if (bcls_submitted)
	  ++number_submitted_results;
	++number_read_results;
	Progress();
	TFrames.clear(); Tbcls.clear(); Tbcls_info.clear(); Tb_unit_reductions.clear(); Tb_autarky_reductions.clear(); Tb_new_clauses.clear();
	l2std.clear();
      }
    }

    // Private data --------------------------------------------------------

  private :

    const string exp_id;

    DatabaseHandler01::DatabaseHandler DH;
    
    DatabaseHandler01::TableHandler TFrames;
    const AllAOs AFrames;
    DatabaseHandler01::TableHandler Tbcls;
    const AllAOs Abcls;
    DatabaseHandler01::TableHandler Tbcls_info;
    const AllAOs Abcls_info;
    DatabaseHandler01::TableHandler Tb_unit_reductions;
    const AllAOs Ab_unit_reductions;
    DatabaseHandler01::TableHandler Tb_autarky_reductions;
    const AllAOs Ab_autarky_reductions;
    DatabaseHandler01::TableHandler Tb_new_clauses;
    const AllAOs Ab_new_clauses;

    DatabaseHandler01::Command command;
    DatabaseHandler01::Select select;
    int n; map<int, string> l2std;
    string frame_id; string m_dens_id; string dim; string cls_id; string b_info_id;
    bool no_reduction;
    bool ignore;
    bool bcls_submitted;

    TimeHandling::Progress Progress;
  };

}


namespace ExperimentUebertragung {

  void transfer(const string& dirname) {

    ErrMeldHandler errhandler;
 
    SAXParser parser;

    parser.setValidationScheme(SAXParser::Val_Auto);
    parser.setErrorHandler(&errhandler);

    const string& filename = dirname + "/experiment.xml";
    string exp_id;

    AnfangPhase1.reset();
    cout << Meldungen[31][language] << endl;

    {
      Durchgang1Handler handler1;
      parser.setDocumentHandler(&handler1);
      parser.parse(filename.c_str());

      cout << Meldungen[32][language] << " " << TimeHandling::output_seconds(AnfangPhase1) << " " << Meldungen[33][language] << " " << TimeHandling::output_seconds(Phase1absolut) << " " << Meldungen[34][language] << endl;

      exp_id = extract_exp_id(handler1);
    }

    cout << Meldungen[13][language] << " " << number_results << "\n" << endl;

    AnfangPhase2.reset();
    Phase2absolut.reset();
    cout << Meldungen[35][language] << endl;

    {
      Durchgang2Handler handler2(exp_id);
      parser.setDocumentHandler(&handler2);
      parser.parse(filename.c_str());

      cout << "\n" << Meldungen[36][language] << " " << TimeHandling::output_seconds(AnfangPhase2) << " " << Meldungen[33][language] << " " << TimeHandling::output_seconds(Phase2absolut) << " " << Meldungen[34][language] << "\n";
      cout <<Meldungen[37][language] << " " << TimeHandling::output_seconds(AnfangPhase1) << " " << Meldungen[33][language] << " " << TimeHandling::output_seconds(Phase1absolut) << " " << Meldungen[34][language] << "\n";

    }
  }

}


namespace {

  string exp_directory;
  bool transfering = false;

  using namespace KMZ;

  bool AktionVersion(const GenArg * const A) {
    printf(Meldungen[0][language], Selbst, Version, Datum, Autor, Uebersetzungsdatum);
    return true;
  }
  void EVersion() {
    cout << Meldungen[1][language];
  }
  bool AktionExperiment(const GenArg * const A) {
    exp_directory = A -> s;
    transfering = true;
    return true;
  }
  void EExperiment() {
    cout << Meldungen[4][language];
  }
  bool AktionSo(const GenArg * const A) {
    sat_only = true;
    return true;
  }
  void ESo() {
    cout << Meldungen[10][language];
  }
  bool AktionU(const GenArg * const A) {
    unique = true;
    return true;
  }
  void EU() {
    cout << Meldungen[11][language];
  }
  bool AktionNoTime(const GenArg * const A) {
    no_time = true;
    return true;
  }
  void ENoTime() {
    cout << Meldungen[16][language];
  }
  bool AktionNoNewClauses(const GenArg * const A) {
    no_new_clauses = true;
    return true;
  }
  void ENoNewClauses() {
    cout << Meldungen[17][language];
  }
  bool AktionGuaranteedNew(const GenArg * const A) {
    guaranteed_new = true;
    return true;
  }
  void EGuaranteedNew() {
    cout << Meldungen[28][language];
  }
  bool AktionNo1Autarky(const GenArg * const A) {
    no_1autarky = true;
    return true;
  }
  void ENo1Autarky() {
    cout << Meldungen[38][language];
  }
  


  Argumentstruktur options[] = {
    {"-v", "--version", KEin, 0, AktionVersion, EVersion},
    {"exp", "--experiment-directory", ZEin, 0, AktionExperiment, EExperiment},
    {"-so", "--sat-only-transfer", KEin, 0, AktionSo, ESo},
    {"-u", "--unique-cls", KEin, 0, AktionU, EU},
    {"-nt", "--no_time", KEin, 0, AktionNoTime, ENoTime},
    {"-nnc", "--no_new_clauses", KEin, 0, AktionNoNewClauses, ENoNewClauses},
    {"-gn", "--guaranteed_new", KEin, 0, AktionGuaranteedNew, EGuaranteedNew},
    {"-n1a", "--no_1-autarkies", KEin, 0, AktionNo1Autarky, ENo1Autarky},
  };
  const int number_options = sizeof(options) / sizeof(options[0]);

}


// Begin and end of error messages

void err_begin() {
  cerr << "\n" << Meldungen[25][language] << "[" << Selbst << "] " << Meldungen[27][language] << " " << exp_directory << "; " << Meldungen[26][language] << "\n";
}

void err_end() {
  cerr << "\n";
  cerr << Meldungen[23][language] << " " << number_read_results << "; " << Meldungen[14][language] << " " << number_submitted_results << endl;
}


// --------------------------------------------------------------


int main (const int argc, const char * const argv[]) {

  if (! KMZ::ArgumentBearbeitung(Selbst, options, number_options, argv + 1, argc - 1, language))
    return 1;

  if (! transfering)
    return 0;

  cout << "[" << Selbst << "] " << Meldungen[29][language] << " " << OKDatabaseName << " " << Meldungen[30][language] << " " << TimeHandling::currentDateTime() << endl;

  bool initialised = false;
  struct termination_control {
    bool& initialised;
    termination_control(bool& initialised) : initialised(initialised) {}
    ~termination_control() {
      if (initialised) {
	try {
	  XMLPlatformUtils::Terminate();
	}
	catch(...) {}
      }
    }
  } tc(initialised);

  try {
    XMLPlatformUtils::Initialize();
    initialised = true;
    transfer(exp_directory);
  }

  catch (const XMLException& f) {
    err_begin();
    cerr << Meldungen[18][language] << "\n";
    cerr << f.getMessage() << " " << f.getSrcFile() << " " << f.getSrcLine() << "\n";
    err_end();
    return 1;
  }
  catch (const DatabaseHandler01::Error::General& f) {
    err_begin();
    cerr << Meldungen[19][language] << "\n";
    cerr << f.what() << "\n";
    err_end();
    return 1;
  }
  catch (const SAXParseException& f) {
    err_begin();
    cerr << Meldungen[20][language] << "\n";
    cerr << f.getMessage() << " " << f.getSystemId() << " " << f.getColumnNumber() << " " << f.getLineNumber() << "\n";
    err_end();
    return 1;
  }
  catch (const Tools::Error::General& f) {
    err_begin();
    cerr << Meldungen[21][language] << "\n";
    cerr << f.what() << "\n";
    err_end();
    return 1;
  }
  catch (const ExperimentUebertragung::Error::General& f) {
    err_begin();
    cerr << Meldungen[22][language] << "\n";
    cerr << f.what() << "\n";
    err_end();
    return 1;
  }
  catch(...) {
    err_begin();
    cerr << "Unbekannter Fehler!\n";
    err_end();
    return 1;
  }

  cout << "\n";
  cout << Meldungen[12][language] << " " << exp_directory << "\n";
  cout << Meldungen[13][language] << " " << number_results << "; " << Meldungen[14][language] << " " << number_submitted_results << endl;
  return 0;
}

