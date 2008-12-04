// Oliver Kullmann, 4.4.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/ExperimentVorbereitung.cpp
  \brief Implementations regarding preparing an experiment
*/


#include <cstdio>

#include "Kommandozeile.hpp"
#include "XMLHandling.hpp"

#include "DeskriptorVerarbeitung.hpp"
#include "ExperimentVorbereitung.hpp"


namespace {

  using namespace ExperimentVorbereitung;
  using namespace ExperimentVorbereitung::Fehler;

  char *Selbst = "experiment_preparation";

  const int AnzahlSprachen = 2;
  KMZ::Sprachen Sprache = KMZ::Englisch;

  const char * const Version = "1.02";
  const char * const Datum = "18.5.2003";
  const char * const Autor = "Oliver Kullmann (Swansea); O.Kullmann@Swansea.ac.uk";
  const char * const Uebersetzungsdatum = __DATE__ " " __TIME__; // Gnu-Uebersetzung

  const char * const Meldungen[][AnzahlSprachen] = {
    {"%s, Version %s vom %s; Autor  %s; Uebersetzungsdatum %s\n", // 0
     "%s, Version %s from %s; author %s; compilation date %s\n"},
    {"Programmname, Versionsnummer, Fertigstellungsdatum, Autor (mit e-Post-Adresse), Uebersetzungsdatum", // 1
     "Name of program, version number, date of completion, author (with e-mail-address), compilation date"},
    {"Name der Datei, die den Experiment-Deskriptor enthaelt.", // 2
     "Name of the file containing the experiment-descriptor."},
    {"Name der Datei, die das xml-Element zur Computer-Beschreibung enthaelt.", // 3
     "Name of the file containing the xml-element for the computer description."},
    {"Name des SAT-Algorithmuss' (der auf das Kommandozeilen-Argument \"-specification\" mit der Ausgabe des xml-Elements zur Solver-Spezifikation reagieren muss).", // 4
     "Name of the SAT solver (which for command line argument \"-specification\" must output the xml-element for the solver specification)."},
    {"Erzeugung und Vorbereitung des Experiment-Ordners", // 5
     "Preparation of the experiment directory"},
    {"", // 6
     ""},
    {"Das Experiment-Verzeichnis konnte nicht angelegt werden.", // 7
     "The experiment directory could not be created."},
    {"FEHLER", // 8
     "ERROR"},
    {"Dateifehler :", // 9
     "File error :"},
    {"Die Datei mit dem Experiment-Deskriptor konnte nicht gelesen werden.", // 10
     "The file containing the experiment descriptor is not readable."},
    {"Die Datei, die die Computer-Beschreibung enthaelt (im xml-Format), ist nicht lesbar.", // 11
     "The file containing the computer-xml-element is not readable."},
    {"Die Datei mit der Liste der Kommandos konnte im Experiment-Ordner nicht angelegt werden.", // 12
     "The file \"task_list\" could not be created in the experiment directory."},
    {"Die Datei mit der Experiment-Beschreibung (im xml-Format) konnte im Experiment-Ordner nicht angelegt werden.", // 13
     "The file \"experiment.xml\" could not be created in the experiment directory."},
    {"Die SAT-Prozedur konnte nicht aufgerufen werden zur Ermittlung der Spezifikation.", // 14
     "The SAT solver could not be called for the specification."},
    {"Kommandozeilen-Fehler:", // 15
     "Command line error:"},
    {"Es wurde keine Deskriptor-Datei angegeben.", // 16
     "No descriptor file has been given."},
    {"Es wurde keine Datei fuer die Computer-xml-Beschreibung angegeben.", // 17
     "No computer-xml-file has been given."},
    {"Keine SAT-Prozedur wurde angegeben.", // 18
     "No SAT solver has been given."},
    {"Die Datei mit der Anzahl der bereits bearbeiteten Zeilen konnte im Experiment-Ordner nicht angelegt werden.", // 19
     "The file \"lines_read\" could not be created in the experiment directory."},
    {"Es wird kein Experiment-Ordner angelegt.", // 20
     "No experiment directory is created."},
    {"Die Datei mit der Loeser-Url konnte im Experiment-Ordner nicht angelegt werden.", // 21
     "The file \"url_solver\" could not be created in the experiment directory."},
    {"Datei mit der Dokument-Typ-Definition fuer das xml-Element \"experiment\".", // 22
     "File with the dtd (document type definition) for the xml-element \"experiment\"."},
    {"Mindestens eine der Dateien \"unlisted_sat\", \"unlisted_unsat\", \"unlisted_unknown\" fuer die Anzahl nicht aufgelisteter Formeln konnte nicht erzeugt werden", // 23
     "At least one of the files \"unlisted_sat\", \"unlisted_unsat\", \"unlisted_unknown\" for the number of unlisted formulas could not be created."},
    {"Die Datei \"first_last.xml\" fuer die Beschreibung von erster und letzter Formel konnte nicht angelegt werden.", // 24
     "The file \"first_last.xml\" containing results for the first and the last created formula could not be created."},
  };
     
}


#include <sys/stat.h> // fuer mkdir

#include <fstream>

#include <sstream>
#include <libgen.h> // fuer basename
#include <cstdlib> // fuer system
#include <iostream>

namespace ExperimentVorbereitung {

  using namespace std;

  namespace Fehler {

    void ExperimentVorbereitungFehler::Ausgabe() const {
      cerr << Meldungen[8][Sprache] << "[" << Selbst << "] ";
    }
    void Dateifehler::Ausgabe() const {
      ExperimentVorbereitungFehler::Ausgabe();
      cerr << Meldungen[9][Sprache] << "\n";
    }
    void ExpDesk_lesen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[10][Sprache] << "\n";
    }
    void CompXml_lesen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[11][Sprache] << "\n";
    }
    void Verzeichnis_anlegen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[7][Sprache] << "\n";
    }
    void lines_read_anlegen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[19][Sprache] << "\n";
    }
    void unlisted_anlegen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[23][Sprache] << "\n";
    }
    void firstlast_anlegen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[24][Sprache] << "\n";
    }
    void url_solver_anlegen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[21][Sprache] << "\n";
    }
    void tasklist_anlegen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[12][Sprache] << "\n";
    }
    void expxml_anlegen::Ausgabe() const {
      Dateifehler::Ausgabe();
      cerr << Meldungen[13][Sprache] << "\n";
    }

    void Ausfuehrungsfehler::Ausgabe() const {
      ExperimentVorbereitungFehler::Ausgabe();
    }
    void LoeserSpezifikation::Ausgabe() const {
      Ausfuehrungsfehler::Ausgabe();
      cerr << Meldungen[14][Sprache] << "\n";
    }

    void Kommandozeilenfehler::Ausgabe() const {
      ExperimentVorbereitungFehler::Ausgabe();
      cerr << Meldungen[15][Sprache] << "\n";
    }
    void Deskriptor_fehlt::Ausgabe() const {
      Kommandozeilenfehler::Ausgabe();
      cerr << Meldungen[16][Sprache] << "\n";
    }
    void CompXml_fehlt::Ausgabe() const {
      Kommandozeilenfehler::Ausgabe();
      cerr << Meldungen[17][Sprache] << "\n";
    }
    void Loeser_fehlt::Ausgabe() const {
      Kommandozeilenfehler::Ausgabe();
      cerr << Meldungen[18][Sprache] << "\n";
    }

  }

  string Namenskern(string s) {
    char *temp = new char[s.size() + 1];
    strcpy(temp, s.c_str());
    s = basename(temp);
    return s.substr(0, s.find('.'));
  }

  Experiment::Experiment(const string DeskriptorDatei, const string ComputerXmlDatei, const string Loeser, const string dtdDatei, const string& generator, const string Verzeichnis)
    : Desk(DeskriptorDatei), Comp(ComputerXmlDatei), Loes(Loeser), dtd(dtdDatei), gen(generator), aktZeit(time(0)), aktDatum(*gmtime(&aktZeit)), Verzeichnisname(Verzeichnis) {
    
    if (Verzeichnis == "") {
      Verzeichnisname = "Experiment-" + Namenskern(Loeser) + "-" + Namenskern(DeskriptorDatei) + "-" + Namenskern(ComputerXmlDatei) + "-";
      ostringstream Datum;
      Datum << aktDatum.tm_mday << "_" << aktDatum.tm_mon + 1 << "_" << aktDatum.tm_year + 1900 << "GMT" << aktDatum.tm_hour << "_" << aktDatum.tm_min;
      Verzeichnisname += Datum.str();
    }
  }

  void Experiment::anlegen() const {

    // Anlegen des Experiment-Ordners

    const string addition = " -T1800"; // 30 minutes

    if (mkdir(Verzeichnisname.c_str(), S_IRWXU) != 0)
      throw Verzeichnis_anlegen();

    // Anlegen der Datei mit der jeweils aktuellen Zeilennummer
    {
      ofstream aktuelleNummer((Verzeichnisname + "/lines_read").c_str());
      if (! aktuelleNummer)
	throw lines_read_anlegen();
      aktuelleNummer << 0 << endl;
      aktuelleNummer.close();
    }

    { // Creation of files with the numbers of unlisted instances
      struct CreateFile0 {
	const string& directory;
	CreateFile0(const string& dir) : directory(dir) {}
	void operator () (const string& name) {
	  ofstream file((directory + "/" + name).c_str());
	  if (! file)
	    throw unlisted_anlegen();
	  file << 0 << endl;
	  file.close();
	}
      } createfile(Verzeichnisname);
      createfile("unlisted_sat");
      createfile("unlisted_unsat");
      createfile("unlisted_unknown");
    }

    // Creation of the file for the first and last formula description
    {
      ofstream firstlast((Verzeichnisname + "/first_last.xml").c_str());
      firstlast << XMLHandling::ext_dtd_decl("list_complete_results", dtd);
      firstlast << "<list_complete_results>\n" << endl;
      if (! firstlast)
	throw firstlast_anlegen();
      firstlast.close();
    }

    // Anlegen der Datei mit der Url fuer den Loeser
    {
      ofstream LoeserUrl((Verzeichnisname + "/url_solver").c_str());
      if (! LoeserUrl)
	throw url_solver_anlegen();
      LoeserUrl << Loes + addition << endl;
      LoeserUrl.close();
    }

    // Anlegen der Datei mit den Befehlen
    const string Befehlsliste_Name(Verzeichnisname + "/task_list");
    ofstream Befehlsliste(Befehlsliste_Name.c_str());
    if (! Befehlsliste)
      throw tasklist_anlegen();

    // Anlegen der Datei fuer die Experiment-Daten
    const string Experiment_Daten_Name(Verzeichnisname + "/experiment.xml");
    ofstream Experiment_Daten(Experiment_Daten_Name.c_str());
    if (! Experiment_Daten)
      throw expxml_anlegen();

    // Oeffnen der Experiment-Deskriptor-Datei
    ifstream Exp_Desk(Desk.c_str());
    if (! Exp_Desk)
      throw ExpDesk_lesen();

    // Erzeugung der Kommando-Liste
    if (gen == "")
      Deskriptorverarbeitung::Verarbeitung(Exp_Desk, Befehlsliste);
    else
      Deskriptorverarbeitung::Verarbeitung(Exp_Desk, Befehlsliste, gen);
    Exp_Desk.clear();

    // Oeffnen der Computer-xml-Datei
    ifstream Comp_xml(Comp.c_str());
    if (! Comp_xml)
      throw CompXml_lesen();

    // Schreiben der Experiment-Daten
    Experiment_Daten << "<?xml version = \"1.0\" standalone = \"" << (dtd.empty() ? "yes" : "no") << "\" ?>\n";
    if (! dtd.empty())
      Experiment_Daten << "<!DOCTYPE experiment SYSTEM \"" << dtd << "\" >\n";
    Experiment_Daten << "<experiment>\n\n";
    Experiment_Daten << "  <experiment-begin time_zone = \"GMT\" minute = \"" << aktDatum.tm_min << "\" hour = \"" << aktDatum.tm_hour << "\" day = \"" << aktDatum.tm_mday << "\" month = \"" << aktDatum.tm_mon + 1 << "\" year = \"" << aktDatum.tm_year + 1900 << "\" />\n\n";
    Experiment_Daten << "  <experiment-type>\n";
    Experiment_Daten << "    <random_experiment>\n";
    Experiment_Daten << "      <OKgenerator_experiment>\n";
    Experiment_Daten << "        <descriptor>\n\n";
    Exp_Desk.seekg(0, ios::beg);
    if (! Exp_Desk)
      throw ExpDesk_lesen();
    Experiment_Daten << Exp_Desk.rdbuf() << endl;
    Exp_Desk.close();
    Experiment_Daten << "        </descriptor>\n";
    Experiment_Daten << "      </OKgenerator_experiment>\n";
    Experiment_Daten << "    </random_experiment>\n";
    Experiment_Daten << "  </experiment-type>\n\n";

    // die Computer-Daten uebertragen
    Experiment_Daten << Comp_xml.rdbuf() << endl;
    Comp_xml.close();

    // die Loeser-Spezifikation uebertragen
    Experiment_Daten.close();
    Experiment_Daten.clear();
    string Loeser_Aufruf(Loes + " -specification >> " + Experiment_Daten_Name);
    if (std::system(Loeser_Aufruf.c_str()) != 0)
      throw LoeserSpezifikation();

    // eine trennende Zeile (zu den Loeser-Ausgaben) schreiben
    Experiment_Daten.open(Experiment_Daten_Name.c_str(), ios::out | ios::app);
    if (! Experiment_Daten)
      throw expxml_anlegen();
    Experiment_Daten << endl;
  }

}


namespace {

  string Deskriptordatei;
  string ComputerXmlDatei;
  string Loeser;
  string dtdDatei;
  bool Vorbereitung = true;

  string special_generatorname;


  void Argumente_testen() throw(Kommandozeilenfehler) {
    if (Deskriptordatei == "")
      throw Deskriptor_fehlt();
    else if (ComputerXmlDatei == "")
      throw CompXml_fehlt();
    else if (Loeser == "")
      throw Loeser_fehlt();
  }

  using namespace KMZ;

  bool AktionVersion(const GenArg * const A) {
    printf(Meldungen[0][Sprache], Selbst, Version, Datum, Autor, Uebersetzungsdatum);
    return true;
  }
  void EVersion() {
    cout << Meldungen[1][Sprache];
  }
  bool AktionDeskriptor(const GenArg * const A) {
    Deskriptordatei = A -> s;
    return true;
  }
  void EDeskriptor() {
    cout << Meldungen[2][Sprache];
  }
  bool AktionComputer(const GenArg * const A) {
    ComputerXmlDatei = A -> s;
    return true;
  }
  void EComputer() {
    cout << Meldungen[3][Sprache];
  }
  bool AktionSolver(const GenArg * const A) {
    Loeser = A -> s;
    return true;
  }
  void ESolver() {
    cout << Meldungen[4][Sprache];
  }
  bool Aktiondtd(const GenArg * const A) {
    dtdDatei = A -> s;
    return true;
  }
  void Edtd() {
    cout << Meldungen[22][Sprache];
  }
  bool AktionKeineVorbereitung(const GenArg *) {
    Vorbereitung = false;
  }
  void EKeineVorbereitung() {
    cout << Meldungen[20][Sprache];
  }
  bool AktionGenerator(const GenArg * const A) {
    special_generatorname = A -> s;
  }
  void EGenerator() {
    cout << Meldungen[42][Sprache];
  }


  Argumentstruktur Optionen[] = {
    {"-v", "--version", KEin, 0, AktionVersion, EVersion},
    {"desc", "--descriptor-file", ZEin, 0, AktionDeskriptor, EDeskriptor},
    {"comp", "--computer-xml-file", ZEin, 0, AktionComputer, EComputer},
    {"solv", "--solver", ZEin, 0, AktionSolver, ESolver},
    {"dtd", "--experiment-dtd-file", ZEin, 0, Aktiondtd, Edtd},
    {"-np", "--no_preparation", KEin, 0, AktionKeineVorbereitung, EKeineVorbereitung},
    {"g", "--generator", ZEin, 0, AktionGenerator, EGenerator},
  };
  const int AnzahlOptionen = sizeof(Optionen) / sizeof(Optionen[0]);

}



int main(const int argc, const char * const argv[]) {

  if (! KMZ::ArgumentBearbeitung(Selbst, Optionen, AnzahlOptionen, argv + 1, argc - 1, Sprache))
    return 1;

  if (! Vorbereitung)
    return 0;

  using namespace ExperimentVorbereitung;

  try {
    Argumente_testen();
  }
  catch (ExperimentVorbereitung::Fehler::Kommandozeilenfehler& f) {
    f.Ausgabe();
    return 1;
  }
  
  Experiment Exp(Deskriptordatei, ComputerXmlDatei, Loeser, dtdDatei, special_generatorname);
  cout << Meldungen[5][Sprache] << " " << Exp.Verzeichnis() << endl;
  try {
    Exp.anlegen();
  }
  catch (ExperimentVorbereitung::Fehler::ExperimentVorbereitungFehler& f) {
    f.Ausgabe();
    return 1;
  }
  catch (Deskriptorverarbeitung::Fehler::DeskriptorVerarbeitung& f) {
    f.Ausgabe();
    return 1;
  }
  return 0;
}

