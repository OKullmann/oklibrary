// Oliver Kullmann, 6.4.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/ExperimentDurchfuehrung.cpp
  \brief Implementations regarding executing an experiment
*/


#include <cstdlib> // fuer system
#include <cstdio> // wg. Kommandozeile und Dateioperationen
#include <limits>
#include <stdexcept>

#include "ExperimentDurchfuehrung.hpp"
#include "Kommandozeile.hpp"
#include "ErrorHandling.hpp"
#include "StringHandling.hpp"

namespace {

  using namespace std;

  using namespace ExperimentDurchfuehrung;
  using namespace ExperimentDurchfuehrung::Fehler;

  char *Selbst = "process_experiment";

  const int AnzahlSprachen = 2;
  KMZ::Sprachen Sprache = KMZ::Englisch;

  const char * const Version = "1.03";
  const char * const Datum = "18.5.2003";
  const char * const Autor = "Oliver Kullmann (Swansea); O.Kullmann@Swansea.ac.uk";
  const char * const Uebersetzungsdatum = __DATE__ " " __TIME__; // Gnu-Uebersetzung

  const char * const Meldungen[][AnzahlSprachen] = {
    {"%s, Version %s vom %s; Autor  %s; Uebersetzungsdatum %s\n", // 0
     "%s, Version %s from %s; author %s; compilation date %s\n"},
    {"Programmname, Versionsnummer, Fertigstellungsdatum, Autor (mit e-Post-Adresse), Uebersetzungsdatum", // 1
     "Name of program, version number, date of completion, author (with e-mail-address), compilation date"},
    {"FEHLER", // 2
     "ERROR"},
    {"Kommandozeilen-Fehler:", // 3
     "Command line error:"},
    {"Es wurde kein Experiment-Ordner angegeben.", // 4
     "No experiment directory has been given."},
    {"Es wurde kein SAT-Loeser angegeben.", // 5
     "No SAT solver has been given."},
    {"Der (vorbereitete) Experiment-Ordner (mit Dateien \"experiment.xml\", \"tasklist\" und \"current_line\").", // 6
     "The (prepared) experiment directory (with files \"experiment.xml\", \"tasklist\" and \"current_line\")."},
    {"Die (ausfuehrbare) Datei, die den SAT-Algorithmus implementiert (wird mit zwei Argumenten aufgerufen: Die Option \"-XO\" fuer Ausgabe im xml-Format, undder Name einer Datei mit der KNF im Dimacs-Format).", // 7
     "The executable file for the SAT solver (called with two arguments, the option \"-XO\" for output in xml format and the name of the file containing a CNF in Dimacs format)."},
    {"Es findet keine Bearbeitung des Experimentes statt.", // 8
     "The experiment directory is not processed."},
    {"Datei-Fehler:", // 9
     "File error:"},
    {"Die Anzahl der bereits bearbeiteten Zeilen konnte nicht ausgelesen werden.", // 10
     "The number of lines read could not be read from file \"lines_read\"."},
    {"Die Datei mit der Kommandoliste konnte nicht geoeffnet werden.", // 11
     "The file \"task_list\" could not be opened."},
    {"Es konnte keine temporaere Datei im Experimentordner erzeugt werden.", // 12
     "No temporary file could be created in the experiment directory."},
    {"Die temporaere Datei im Experimentordner konnte nicht gelesen werden.", // 13
     "The temporary file in the experiment file could not be read."},
    {"Bearbeitungs-Fehler:", // 14
     "Processing error:"},
    {"Die im Experiment-Ordner vorgefundene Anzahl von schon bearbeiteten Kommandos ist zu gross.", // 15
     "The number in \"lines_read\" is too big."},
    {"Die Experiment-xml-Datei kann nicht beschrieben werden.", // 16
     "The file \"experiment.xml\" is not writeable."},
    {"Die Datei mit der Kommandoliste konnte nicht zu Ende gelesen werden", // 17
     "The file \"task_list\" could not be read until the end."},
    {"Der Systemaufruf", // 18
     "The system call"},
    {"scheiterte.", // 19
     "failed."},
    {"Fehler beim Schliessen des Experimentes:", // 20
     "Error at closing the experiment:"},
    {"Die Hilfsdateien konnten nicht geloescht werden.", // 21
     "The auxiliary files could not be removed."},
    {"Der Experimentordner konnte nicht archiviert werden.", // 22
     "The experiment directory could not be archived."},
    {"Die Loeser-Url konnte dem Experiment-Verzeichnis nicht entnommen werden.", // 23
     "The solver url could not be read from the file \"url_solver\"."},
    {"Die Nummer der gerade bearbeiteten Zufalls-Klauselmenge wird ausgegeben.", // 24
     "The number of the currently processed random formula is print out."},
    {"Bearbeitung des Experiment-Ordners ", // 25
     "Processing of experiment directory "},
    {"Nummer der gerade bearbeiteten Zufalls-Klauselmenge =", // 26
     "Number of current random formula ="},
    {"Unbekannte Spezifikation", // 27
     "Unknown specification"},
    {"in Zeile", // 28
     "in line"},
    {"Die Ausgabe des SAT-Loesers enthaelt keines der Schluesselwoerter \"unsatisfiable\", \"satisfiable\" oder \"unknown\".", // 29
     "The solver output does not contain one of the key words  \"unsatisfiable\", \"satisfiable\" or \"unknown\"."},
    {"Die Datei \"first_last.xml\" fuer die Ergebnisse der ersten und letzten Formel konnte nicht beschrieben werden.", // 30
     "The file \"first_last.xml\" for the results of the first and last formula is not writeable."},
  };

  bool Kontrollmeldungen = false;

}


#include <fstream>
#include <sstream>


namespace {

  class DateiZeile {

    // Fuer ein Objekt 
    // DateiZeile Zeile(Name) (auf die erste Zeile von Name gesetzt)
    // gibt es die folgenden Operationen:

    // bool(Zeile) ist wahr, wenn es eine aktuelle vollstaendige, d.h. mit "\n" beendete Zeile gibt;
    // *Zeile liest die aktuelle Zeile (moeglicherweise wurde diese nicht mit "\n" beendet, falls vorher das Dateiende erreicht wurde);
    // ++Zeile geht eine Zeile weiter (falls moeglich);
    // Zeile.weiter(n) geht n Zeilen weiter;
    // Zeile.eof() ist wahr, falls das Dateiende erreicht worden ist.

    // oeffnen wird geworfen, falls die Datei "Name" nicht geoeffnet werden konnte.
    // Schritt wird geworfen, falls weiter(n) nicht n Zeilen findet.

  public :

    struct oeffnen : Dateifehler {};
    struct Schritt : Ausfuehrungsfehler {};

    DateiZeile(const string& Dateiname)
      : Datei(Dateiname.c_str()) {
      if (! Datei)
	throw oeffnen();
      getline(Datei, aktuelleZeile);
    }

    DateiZeile& operator ++ () {
      if (Datei)
	getline(Datei, aktuelleZeile);
      return *this;
    }

    string operator * () const {return aktuelleZeile;}

    DateiZeile& weiter(unsigned long int n) {
      if (n == 0) return *this;
      unsigned long int i(1);
      while (i < n && Datei) {
	Datei.ignore(numeric_limits<int>::max(), '\n');
	++i;
      }
      if (Datei) {
	getline(Datei, aktuelleZeile);
	i++;
      }

      if (! Datei && i <= n)
	throw Schritt();
      return *this;
    }

    operator bool() const {return Datei;}

    bool eof() const {return Datei.eof();}

  private :

    ifstream Datei;
    string aktuelleZeile;

    DateiZeile(DateiZeile&);
    DateiZeile& operator = (DateiZeile&);

  };


  class DateiZaehler {
    // Verwaltung eines Zaehlers, der in einer Datei gespeichert wird, und
    // um 1 hochgezaehlt werden kann.
  public :

    struct lesen : Dateifehler {};
    struct schreiben : Dateifehler {};

    DateiZaehler(const string& Dateiname)
      : Datei(Dateiname.c_str(), ios::in | ios::out) {
      Datei >> Zaehler;
      if (! Datei)
	throw lesen();
      // Falls (z.B.) die Datei nicht vorhanden war, so ergibt dies hier einen
      // Fehler, es wird die Datei aber (leer) erzeugt.
    }

    unsigned long int count() const {return Zaehler;}

    DateiZaehler& operator ++() {
      Datei.seekg(0);
      Datei << ++Zaehler << endl;
      if (! Datei)
	throw schreiben();
      return *this;
    }

  private :

    fstream Datei;
    unsigned long int Zaehler;

    DateiZaehler(DateiZaehler&);
    DateiZaehler& operator = (DateiZaehler&);

  };


  class System {

  public :

    struct erzeugen : Ausfuehrungsfehler {};
    struct oeffnen : Ausfuehrungsfehler {};
    struct aufrufen : Ausfuehrungsfehler {
      string Kommando;
      aufrufen(const string& K) : Kommando(K) {}
    };

    System(const string& Name)
      : tempName(Name) {
      ofstream Datei(tempName.c_str());
      if (! Datei)
	throw erzeugen();
    }

    string operator () (const string& Kommando) {
      const string& VollKommando = Kommando + " > " + tempName;
      if (system(VollKommando.c_str()) != 0)
	throw aufrufen(VollKommando);
      ifstream Datei(tempName.c_str());
      if (! Datei)
	throw oeffnen();
      ostringstream s;
      s << Datei.rdbuf();
      return s.str();
    }

    ~System() {
      remove(tempName.c_str());
    }

  private :

    string tempName;

    System(System&);
    System& operator = (System&);

  };

}


#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctime>

namespace ExperimentDurchfuehrung {

  using namespace std;

  namespace Fehler {

    void ExperimentDurchfuehrungsfehler::Ausgabe() const {
      cerr << Meldungen[2][Sprache] << "[" << Selbst << "] ";
    }

    void Kommandozeilenfehler::Ausgabe() const {
      ExperimentDurchfuehrungsfehler::Ausgabe();
      cerr << Meldungen[3][Sprache] << "\n";
    }
    void Experiment_Ordner_fehlt::Ausgabe() const {
      Kommandozeilenfehler::Ausgabe();
      cerr << Meldungen[4][Sprache] << "\n";
    }

    void Dateifehler::Ausgabe() const {
      ExperimentDurchfuehrungsfehler::Ausgabe();
      cerr << Meldungen[9][Sprache] << "\n";
    }
    void url_solver_lesen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[23][Sprache] << "\n";
    }
    void lines_read_lesen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[10][Sprache] << "\n";
    }
    void tasklist_oeffnen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[11][Sprache] << "\n";
    }
    void tasklist_unbekannt::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[17][Sprache] << "\n";
    }
    void temp_Datei_erzeugen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[12][Sprache] << "\n";
    }
    void temp_Datei_oeffnen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[13][Sprache] << "\n";
    }
    void firstlast_schreiben::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[30][Sprache] << "\n";
    }
    void expxml_schreiben::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[16][Sprache] << "\n";
    }

    void Ausfuehrungsfehler::Ausgabe() const {
      ExperimentDurchfuehrungsfehler::Ausgabe();
      cerr << Meldungen[14][Sprache] << "\n";
    }
    void zuvieleZeilen::Ausgabe() const {
      Ausfuehrungsfehler::Ausgabe();
      cerr << Meldungen[15][Sprache] << "\n";
    }
    void Systemaufruf::Ausgabe() const {
      Ausfuehrungsfehler::Ausgabe();
      cerr << Meldungen[18][Sprache] << " \"" << Kommando << "\" " << Meldungen[19][Sprache] << "\n";
    }

    void Aufraeumfehler::Ausgabe() const {
      ExperimentDurchfuehrungsfehler::Ausgabe();
      cerr << Meldungen[20][Sprache] << "\n";
    }
    void DateienLoeschen::Ausgabe() const {
      Aufraeumfehler::Ausgabe();
      cerr << Meldungen[21][Sprache] << "\n";
    }
    void Archivieren::Ausgabe() const {
      Aufraeumfehler::Ausgabe();
      cerr << Meldungen[22][Sprache] << "\n";
    }

  }

  void Durchfuehrung (const string& ExperimentVerzeichnis) {
    try {

      string Loeser;
      {
	ifstream Loeserdatei((ExperimentVerzeichnis + "/url_solver").c_str());
	Loeserdatei >> Loeser;
	if (! Loeserdatei)
	  throw url_solver_lesen();
      }

      DateiZaehler bearbeitet(ExperimentVerzeichnis + "/lines_read");
      DateiZaehler unlisted_sat(ExperimentVerzeichnis + "/unlisted_sat");
      DateiZaehler unlisted_unsat(ExperimentVerzeichnis + "/unlisted_unsat");
      DateiZaehler unlisted_unknown(ExperimentVerzeichnis + "/unlisted_unknown");
      if (bearbeitet.count() < unlisted_sat.count() + unlisted_unsat.count() + unlisted_unknown.count())
	throw InconsistentCounters("Number of processed formulas is less than the number of unlisted formulas.");

      DateiZeile Kommandozeilen(ExperimentVerzeichnis + "/task_list");
      Kommandozeilen.weiter(bearbeitet.count());

      System SystemAufruf(ExperimentVerzeichnis + "/temp_file");

      ofstream firstlastDatei((ExperimentVerzeichnis + "/first_last.xml").c_str(), ios::out | ios::app);
      if (! firstlastDatei)
	throw firstlast_schreiben();
      ofstream ExperimentDatei((ExperimentVerzeichnis + "/experiment.xml").c_str(), ios::out | ios::app);
      if (! ExperimentDatei)
	throw expxml_schreiben();

      cout << Meldungen[25][Sprache] << ExperimentVerzeichnis << endl;
      if (Kontrollmeldungen)
	cout << Meldungen[26][Sprache] << flush;

      while (Kommandozeilen) {

	if (Kontrollmeldungen)
	  cout << " " << bearbeitet.count() << flush;

	string Kommando = *Kommandozeilen;
	enum {nurUNSAT, nurSAT, alle, keine} Selektion = alle;
	{
	  string::size_type i;
	  i = Kommando.find("|");
	  if (i != string::npos) {
	    const string Sel = Kommando.substr(i+1);
	    Kommando.erase(i);
	    if (Sel == "0")
	      Selektion = nurUNSAT;
	    else if (Sel == "1")
	      Selektion = nurSAT;
	    else if (Sel == "count_only")
	      Selektion = keine;
	    else {
	      throw SyntaxError(string(Meldungen[27][Sprache]) + " \"" + Sel + "\" " + Meldungen[28][Sprache] + " " + StringHandling::toString(bearbeitet.count()));
	    }
	  }
	}

	string KlmSpez(Kommando);
	KlmSpez = SystemAufruf(KlmSpez.substr(0, KlmSpez.find("-o")) + "-X -nf -o");

	string tempKlmName(Kommando);
	tempKlmName.erase(remove(tempKlmName.begin(), tempKlmName.end(), ' '), tempKlmName.end());
	tempKlmName.erase(remove(tempKlmName.begin(), tempKlmName.end(), '/'), tempKlmName.end());
	tempKlmName = ExperimentVerzeichnis + "/" + tempKlmName;

	string LoeserAusgabe;
	{
	  System AufrufKlm(tempKlmName);
	  AufrufKlm(Kommando);
	  LoeserAusgabe = SystemAufruf(Loeser + " -XO " + tempKlmName);
	}

	{
	  enum {sat, unsat, unknown} result;
	  // ASSUMPTION: Solver output contains exactly one of
	  // "\"unsatisfiable\"", "\"satisfiable\"" or "\"unknown\"".
	  if (LoeserAusgabe.find("\"unsatisfiable\"") != string::npos)
	    result = unsat;
	  else if (LoeserAusgabe.find("\"satisfiable\"") != string::npos)
	    result = sat;
	  else if (LoeserAusgabe.find("\"unknown\"") != string::npos)
	    result = unknown;
	  else
	    throw Fehler::SolverOutput(Meldungen[29][Sprache]);
	  if ((Selektion == nurUNSAT or Selektion == keine) and result == sat)
	    ++unlisted_sat;
	  else if ((Selektion == nurSAT or Selektion == keine) and result == unsat)
	    ++unlisted_unsat;
	  else if (Selektion != alle and result == unknown)
	    ++unlisted_unknown;
	  else {
	    ExperimentDatei << "  <complete_result>\n";
	    ExperimentDatei << KlmSpez;
	    ExperimentDatei << LoeserAusgabe;
	    ExperimentDatei << "  </complete_result>\n" << endl;
	    if (! ExperimentDatei)
	      throw expxml_schreiben();
	  }
	}

	++Kommandozeilen;
	++bearbeitet;

	if (bearbeitet.count() == 1 or not Kommandozeilen) {
	  firstlastDatei << "  <complete_result>\n";
	  firstlastDatei << KlmSpez;
	  firstlastDatei << LoeserAusgabe;
	  firstlastDatei << "  </complete_result>\n" << endl;
	  if (! firstlastDatei)
	    throw firstlast_schreiben();
	}

      }
      if (Kontrollmeldungen)
	cout << endl;
      if (! Kommandozeilen.eof())
	throw tasklist_unbekannt();

      {
	const unsigned long int n_unlisted_sat = unlisted_sat.count();
	const unsigned long int n_unlisted_unsat = unlisted_unsat.count();
	const unsigned long int n_unlisted_unknown = unlisted_unknown.count();
	if (n_unlisted_sat != 0 or n_unlisted_unsat != 0 or n_unlisted_unknown != 0) {
	  ExperimentDatei << "  <unlisted_results number_unknown_results = \"" << StringHandling::toString(n_unlisted_unknown) << "\" number_sat_results = \"" << StringHandling::toString(n_unlisted_sat) << "\" number_unsat_results = \"" << StringHandling::toString(n_unlisted_unsat) << "\" />\n\n";
	}
      }
      {
	firstlastDatei << "</list_complete_results>" << endl;
	if (! firstlastDatei)
	    throw firstlast_schreiben();
	firstlastDatei.close();
      }
      time_t aktZeit(time(0));
      tm aktDatum(*gmtime(&aktZeit));
      ExperimentDatei << "  <experiment-end time_zone = \"GMT\" minute = \"" << aktDatum.tm_min << "\" hour = \"" << aktDatum.tm_hour << "\" day = \"" << aktDatum.tm_mday << "\" month = \"" << aktDatum.tm_mon + 1 << "\" year = \"" << aktDatum.tm_year + 1900 << "\" />\n\n";
      
      ExperimentDatei << "</experiment>\n" << endl;
      if (! ExperimentDatei)
	  throw expxml_schreiben();
    }
    catch (DateiZaehler::lesen) {
      throw Fehler::lines_read_lesen();
    }
    catch (DateiZeile::oeffnen) {
      throw Fehler::tasklist_oeffnen();
    }
    catch (DateiZeile::Schritt) {
      throw zuvieleZeilen();
    }
    catch (System::erzeugen) {
      throw Fehler::temp_Datei_erzeugen();
    }
    catch (System::oeffnen) {
      throw Fehler::temp_Datei_oeffnen();
    }
    catch (System::aufrufen& f) {
      throw Fehler::Systemaufruf(f.Kommando);
    }

    // Aufraeumen nach Beendigung des Experiments

    if (remove((ExperimentVerzeichnis + "/task_list").c_str()) != 0 || remove((ExperimentVerzeichnis + "/lines_read").c_str()) != 0 || remove((ExperimentVerzeichnis + "/url_solver").c_str()) != 0 or remove((ExperimentVerzeichnis + "/unlisted_sat").c_str()) != 0 || remove((ExperimentVerzeichnis + "/unlisted_unsat").c_str()) != 0 || remove((ExperimentVerzeichnis + "/unlisted_unknown").c_str()) != 0)
      throw Fehler::DateienLoeschen();
    string ExpVerz(ExperimentVerzeichnis);
    if (*(ExpVerz.end() - 1) == '/')
      ExpVerz.erase(ExpVerz.size() - 1);
    if (system(("tar -cvz -f " + ExpVerz + ".tar.gz " + ExpVerz).c_str()) != 0)
      throw Fehler::Archivieren();
    
  }

}


namespace {

  string Verzeichnis;
  bool Bearbeitung = true;

  void Argumente_testen() throw(Kommandozeilenfehler) {
    if (Verzeichnis == "")
      throw Experiment_Ordner_fehlt();
  }

  using namespace KMZ;

  bool AktionVersion(const GenArg * const A) {
    printf(Meldungen[0][Sprache], Selbst, Version, Datum, Autor, Uebersetzungsdatum);
    return true;
  }
  void EVersion() {
    cout << Meldungen[1][Sprache];
  }
  bool AktionExperiment(const GenArg * const A) {
    Verzeichnis = A -> s;
    return true;
  }
  void EExperiment() {
    cout << Meldungen[6][Sprache];
  }
  bool AktionMeldungen(const GenArg *) {
    Kontrollmeldungen = true;
  }
  void EMeldungen() {
    cout << Meldungen[24][Sprache];
  }
  bool AktionKeineVorbereitung(const GenArg *) {
    Bearbeitung = false;
  }
  void EKeineVorbereitung() {
    cout << Meldungen[8][Sprache];
  }

  Argumentstruktur Optionen[] = {
    {"-vb", "--verbose", KEin, 0, AktionMeldungen, EMeldungen},
    {"-v", "--version", KEin, 0, AktionVersion, EVersion},
    {"exp", "--experiment-directory", ZEin, 0, AktionExperiment, EExperiment},
    {"-np", "--no_processing", KEin, 0, AktionKeineVorbereitung, EKeineVorbereitung},
  };
  const int AnzahlOptionen = sizeof(Optionen) / sizeof(Optionen[0]);

}


int main(const int argc, const char * const argv[]) {

  if (! KMZ::ArgumentBearbeitung(Selbst, Optionen, AnzahlOptionen, argv + 1, argc - 1, Sprache))
    return 1;

  if (! Bearbeitung)
    return 0;

  using namespace ExperimentDurchfuehrung;

  try {
    Argumente_testen();
  }
  catch (ExperimentDurchfuehrung::Fehler::Kommandozeilenfehler& f) {
    f.Ausgabe();
    return 1;
  }
  catch (const ErrorHandling::Error& e) {
    cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (const std::exception& e) {
    cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }

  try {
    Durchfuehrung(Verzeichnis);
  }
  catch (ExperimentDurchfuehrung::Fehler::ExperimentDurchfuehrungsfehler& f) {
    f.Ausgabe();
    return 1;
  }
  catch (const ErrorHandling::Error& e) {
    cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (const std::exception& e) {
    cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }


  return 0;

}

