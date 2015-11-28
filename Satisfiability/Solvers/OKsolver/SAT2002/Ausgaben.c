// Oliver Kullmann, 19.1.2001 (Toronto)
/* Copyright 2001 - 2007, 2008, 2009, 2011, 2015 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Ausgaben.c
  \brief Messages, and implementations for functions outputting messages
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* 19.1.2001, UofT */

/* Einheit: Ausgaben */

/* Ausgaben (Fehler, Ergebnisse und Zustandsmeldungen) */

#include <stdio.h>
#include <assert.h>

#include "OK.h"
#include "Ausgaben.h"



const char* Meldungen[ANZSPRACHEN][63] =
{
  {"Fehler in der Definition der Konstanten! Abbruch.", /* 0 */
   "Kombination von BAUMRES mit LOKALLERNEN zur Zeit noch nicht moeglich.", /* 1 */
   "Version", /* 2 */
   "Verwendung von OKsolver: Optionen, Dateiname1, Optionen, Dateiname2, ...", /* 3 */
   "Die Eingabedatei existiert nicht oder ist lesegeschuetzt:", /* 4 */
   "Virtueller Speicher nicht ausreichend! Abbruch.", /* 5 */
   "Datum der Uebersetzung", /* 6 */
   "Die Groesse der Eingabe-Datei kann nicht bestimmt werden:", /* 7 */
   "Syntax-Fehler!", /* 8 */
   "Syntax-Fehler! Kommentarzeilen muessen auch am Dateiende mit dem Zeilenende-Zeichen abgeschlossen werden.", /* 9 */
   "Syntax-Fehler! Jede Klausel muss mit dem Klausel-End-Symbol abgeschlossen werden --- auch die leere Klausel am Dateiende.", /* 10 */
   "Position in der Eingabe-Datei und fehlerhaftes Zeichen:", /* 11 */
   "Zeilennummer:", /* 12 */
   "Syntax-Fehler! Der Beginn eines Variablennamens wird erwartet.", /* 13 */
   "Syntax-Fehler! Jede Klausel muss mit dem Klausel-End-Symbol abgeschlossen werden --- auch am Dateiende.", /* 14 */
   "Syntax-Fehler! Es muss ein neues Literal folgen am Dateiende.", /* 15 */
   "Syntax-Fehler! Es muss ein neues Literal folgen.", /* 16 */
   "Syntaxfehler in", /* 17 */
   "Der Option --language= muss eine Nummer folgen.", /* 18 */
   "Nummern fuer Sprachen sind 0, ..,", /* 19 */
   "Der Option --standard= muss eine Nummer folgen.", /* 20 */
   "Nummern fuer Standards sind 1, ..,", /* 21 */
   "Die folgende Option wurde nicht erkannt:", /* 22 */
   "Position in der Zeile:", /* 23 */
   "OKsolver: 5.3.1998 -", /* 24 */
   "Oliver Kullmann (Frankfurt, Toronto, Swansea); O.Kullmann@Swansea.ac.uk", /* 25 */
   "OKsolver wird mit einer Liste von Argumenten aufgerufen, die der Reihe nach abgearbeitet werden. Neben Dateinamen (fuer Klauselmengen) koennen dies die folgenden Optionen sein:\n--version --author --help -O -F -M -P -R -B --info -RA --language=i --standard=i -Di -MAXN= -MAXK= -MAXL= --timeout=i -seed= -quot=\nwobei i fuer eine natuerliche Zahl steht.", /* 26 */
   "Die folgende Datei konnte nicht zum Einschreiben der erfuellenden Belegung geoeffnet werden:", /* 27 */
   "Name der Datei, Beobachtungstiefe und Anzahl der Beobachtungsknoten:", /* 28 */
   "Die folgende Datei konnte nicht zum Einschreiben der Ueberwachungsmeldungen geoeffnet werden:", /* 29 */
   "Die folgende Datei konnte nicht zum Eintragen der Ergebnisse geoeffnet werden:", /* 30 */
   "Der Option -D muss eine natuerliche Zahl folgen.", /* 31 */
   "Das Beobachtungsniveau kann nicht negativ sein.", /* 32 */
   "Der Option -MAXN= muss eine natuerliche Zahl folgen.", /* 33 */
   "Die maximale Variablenanzahl kann nicht negativ sein.", /* 34 */
   "Die maximale Variablenanzahl wurde ueberschritten:", /* 35 */
   "Der Option -MAXL= muss eine natuerliche Zahl folgen.", /* 36 */
   "Die maximale Anzahl von Literalvorkommen kann nicht negativ sein.", /* 37 */
   "Die maximale Anzahl von Literalvorkommen wurde ueberschritten.", /* 38 */
   "Der Option -MAXK= muss eine natuerliche Zahl folgen.", /* 39 */
   "Die maximale Klauselanzahl kann nicht negativ sein.", /* 40 */
   "Die maximale Klauselnanzahl wurde ueberschritten.", /* 41 */
   "Der Option --timeout= muss eine natuerliche Zahl folgen.", /* 42 */
   "Zeit wird durch eine natuerliche Zahl (>=0) angegeben (Sekunden).", /* 43 */
   "Uebersetzer-Optionen:", /* 44 */
   "Datei mit Abstandsfunktion:", /* 45 */
   "Datei mit Projektionsfunktion:", /* 46 */
   "Interpretation der Ausgabezeile:", /* 47 */
   "SAT, P0, N0, K0, L0, Zeit, Knoten, Single-Knoten, Quasi-Single-Knoten, 2-Reduktionen, pure Literale, Autarkien, verpasste Single-Knoten, maximale Suchbaumtiefe, Ueberschreitung2, P0 - P, N0 - N, K0 - K, L0 - L, Fastautarkien, initiale 1-Reduktionen, neue 2-Klauseln (insgesamt), neue 2-Klauseln maximal, initiale 2-Klauseln, Dateiname", /* 48 */
   "FASTAUTARKIE setzt LOKALLERNEN voraus!", /* 49 */
   "Im Standard1 sind keine leeren Klauseln erlaubt.", /* 50 */
   "Im Standard1 muss zwischen dem durch 0 markierten Klauselende und einem folgenden Kommentar oder neuer Klausel ein Leerzeichen stehen.", /* 51 */
   "Der Option -seed= muss eine Zahl folgen.", /* 52 */
   "Der Option -quot= muss eine Zahl folgen.", /* 53 */
   "Die folgende Datei konnte nicht zum Einschreiben der Baumdaten geoeffnet werden: (XML; Makro OUTPUTTREEDATAXML definiert)", /* 54 */
   "Niveau: Anzahl Knoten, durchschnittliche Anzahl Knoten, geschaetzte totale Knotenzahl, Zeitverbrauch des Beobachtungsknotens, durchschnittliche Zeit, geschaetzte Restzeit, Singleknoten, Autarkien, Suchbaumtiefe", /* 55 */
   "    level  nodes ave_nodes      time  ave_time singles autarkies depth ave_reductions", /* 56 */
   "Verzeichnisname (fuer Paralllelisierungs-Verzeichnis) darf nicht leer sein.", /* 57 */
   "Ausgabe partieller Belegungen muss aktiviert sein fuer Option -S.", /* 58 */
   "Die Datei fuer eine Aufspaltungsinstanz konnte nicht erzeugt werden.", /* 59 */
   "Die Datei fuer die Aufspaltungsinstanzen konnte nicht erzeugt werden:", /* 60 */
   "Vorzeitiger Abbruch des Aufspaltungsmodus.", /* 61 */
   "Die Datei fuer die Entscheidungen der Aufspaltungsinstanzen konnte nicht erzeugt werden:", /* 62 */
  },

  {"Error in the definition of the constants! Abbort.", /* 0 */
   "Combination of BAUMRES and LOKALLERNEN yet not possible.", /* 1 */
   "version", /* 2 */
   "When calling OKsolver, exactly one parameter is needed, the name of the input clause-set.", /* 3 */
  "The input file doesn't exist or is read-protected:", /* 4 */
   "Virtual memory exhausted! Exit.", /* 5 */
   "Date of compilation", /* 6 */
   "The size of the input file can not be determined:", /* 7 */
   "Syntax error!", /* 8 */
   "Syntax error! Comment lines must be closed with the end-of-line character also at the end of the input file.", /* 9 */
   "Syntax error! Each clause must be completed with the clause-end-symbol --- also the empty clause at the end of the file.", /* 10 */
   "The position in the input file and the wrong character are:", /* 11 */
   "Line number:", /* 12 */
   "Syntax error! The begin of a name of a variable is expected.", /* 13 */
   "Syntax error! Each clause must be completed with the clause-end-symbol --- also at the end of the file.", /* 14 */
   "Syntax error! There must be another literal at the end of file.", /* 15 */
   "Syntax error! There must be another literal.", /* 16 */
   "Syntax error in", /* 17 */
   "Option --language= must be followed by a number", /* 18 */
   "Numbers for languages are 0, ...,", /* 19 */
   "Option --standard= must be followed by a number", /* 20 */
   "Numbers for standards are 1, ...,", /* 21 */
   "The following option has not been recognized:", /* 22 */
   "position in the line:", /* 23 */
   "OKsolver: 5.3.1998 -", /* 24 */
   "Oliver Kullmann (Frankfurt, Toronto, Swansea); o.kullmann@swansea.ac.uk", /* 25 */
   "OKsolver is called with a list of arguments, which will be processed consecutively. The arguments can be names of files (for clause-sets) as well as the following options:\n--version --author --help -O -F -M -P -R -B --info -RA --language=i --standard=i -Di -MAXN= -MAXK= -MAXL= --timeout=i -seed= -quot=\nwhere i is a non-negative integer.", /* 26 */
   "The following file could not be opened for writing the satisfying partial assignment:", /* 27 */
   "Name of file, monitoring depth and number of monitoring nodes:", /* 28 */
   "The following file could not be opened for writing the monitor messages:", /* 29 */
   "The following file could not be opened for adding the results:", /* 30 */
   "Option -D must be followed by a non-negative integer.", /* 31 */
   "The monitor level can not be negative.", /* 32 */
   "Option -MAXN must be followed by a non-negative integer.", /* 33 */
   "The maximal number of variables can not be negative.", /* 34 */
   "The maximal number of variables has been exceeded:", /* 35 */
   "Option -MAXL must be followed by a non-negative integer.", /* 36 */
   "The maximal number of literal occurrences can not be negative.", /* 37 */
   "The maximal number of literal occurrences has been exceeded:", /* 38 */
   "Option -MAXK must be followed by a non-negative integer.", /* 39 */
   "The maximal number of clauses can not be negative.", /* 40 */
   "The maximal number of clauses has been exceeded:", /* 41 */
   "Option --timeout= must be followed by a non-negative integer.", /* 42 */
   "Time must be given by a non-negative integer (seconds).", /* 43 */
   "Compiler options:", /* 44 */
   "File with distance function:", /* 45 */
   "File with projection function:", /* 46 */
   "Interpretation of the output line:", /* 47 */
   "SAT, P0, N0, K0, L0, time, nodes, single nodes, quasi single nodes, 2-reductions, pure literals, autarkies, missed single nodes, maximal depth, Ueberschreitung2, P0 - P, N0 - N, K0 - K, L0 - L, near-autarkies, initial 1-reductions, new 2-clauses (total), new 2-clauses maximal, initial 2-clauses, file name", /* 48 */
   "FASTAUTARKIE needs LOKALLERNEN!", /* 49 */
   "No empty clauses are allowed with Standard1.", /* 50 */
   "In Standard1 after a clause-end marked by 0 there must be a space before another clause or comment.", /* 51 */
   "Option -seed= must be followed by a number.", /* 52 */
   "Option -quot= must be followed by a number.", /* 53 */
   "The following file could not be opened for writing the tree data (xml; Macro OUTPUTTREEDATAXML defined):", /* 54 */
   "level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth", /* 55 */
   "    level  nodes ave_nodes      time  ave_time singles autarkies depth ave_reductions", /* 56 */
   "Directory name (for splitting-directory) can not be empty.", /* 57 */
   "Output of partial assignments must be enabled with option -S.", /* 58 */
   "Could not create file with splitting-instance.", /* 59 */
   "Could not create the file for the splitting-instance:", /* 60 */
   "Abortion of splitting computation.", /* 61 */
   "Could not create the file for the decisions of the splitting-instance:", /* 62 */
  },
};


//! helper function for the messsage depending on the current language
const char* Meldung(const unsigned int i) {
  assert(i < 63);
  return Meldungen[Sprache][i];
}

//! checking the text messages
char Konstantenfehler() {
#ifdef BAUMRES
# ifdef LOKALLERNEN
  fprintf(stderr, "%s\n", Meldung(1)); return 1;
# endif
#endif
#ifdef FASTAUTARKIE
# ifndef LOKALLERNEN
  fprintf(stderr, "%s\n", Meldung(49)); return 1;
# endif
#endif
  if ((Sprache < 0) || (Sprache >= ANZSPRACHEN)) {
    fprintf(stderr, "%s %2d\n", Meldung(19), ANZSPRACHEN - 1); return 1;
  }
  if ((Standard < 0) || (Sprache > ANZSTANDARDS)) {
    fprintf(stderr, "%s %2d\n", Meldung(21), ANZSTANDARDS); return 1;
  }
  return 0;
}
