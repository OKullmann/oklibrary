// Oliver Kullmann, 6.4.2002 (Swansea)

#ifndef EXPERIMENTDURCHFUEHRUNGWAECHTER

#include <string>

#include "ErrorHandling.hpp"

namespace ExperimentDurchfuehrung {

  namespace Fehler {

    struct ExperimentDurchfuehrungsfehler {
       virtual void Ausgabe() const;
    };

    struct Kommandozeilenfehler : ExperimentDurchfuehrungsfehler {
      void Ausgabe() const;
    };
    struct Experiment_Ordner_fehlt : Kommandozeilenfehler {
      void Ausgabe() const;
    };

    struct Ausfuehrungsfehler : ExperimentDurchfuehrungsfehler {
      void Ausgabe() const;
    };
    struct zuvieleZeilen : Ausfuehrungsfehler {
      void Ausgabe() const;
    };
    class Systemaufruf : public Ausfuehrungsfehler {
      std::string Kommando;
    public :
      Systemaufruf(const std::string& K) : Kommando(K) {}
      void Ausgabe() const;
    };

    struct Dateifehler : ExperimentDurchfuehrungsfehler {
      void Ausgabe() const;
    };
    struct url_solver_lesen : Dateifehler {
      void Ausgabe() const;
    };
    struct firstlast_schreiben : Dateifehler {
      void Ausgabe() const;
    };
    struct expxml_schreiben : Dateifehler {
      void Ausgabe() const;
    };
    struct tasklist_oeffnen : Dateifehler {
      void Ausgabe() const;
    };
    struct tasklist_unbekannt : Dateifehler {
      void Ausgabe() const;
    };
    struct lines_read_lesen : Dateifehler {
      void Ausgabe() const;
    };
    struct lines_read_schreiben : Dateifehler {
      void Ausgabe() const;
    };
    struct temp_Datei_erzeugen : Dateifehler {
      void Ausgabe() const;
    };
    struct temp_Datei_oeffnen : Dateifehler {
      void Ausgabe() const;
    };

    struct Aufraeumfehler : ExperimentDurchfuehrungsfehler {
      void Ausgabe() const;
    };
    struct DateienLoeschen : Aufraeumfehler {
      void Ausgabe() const;
    };
    struct Archivieren : Aufraeumfehler {
      void Ausgabe() const;
    };


    struct SyntaxError : ErrorHandling::Error {
      SyntaxError(const std::string& s) : ErrorHandling::Error(s) {}
    };
    struct SolverOutput : ErrorHandling::Error {
      SolverOutput(const std::string& s) : ErrorHandling::Error(s) {}
    };
    struct InconsistentCounters : ErrorHandling::Error {
      InconsistentCounters(const std::string& s) : ErrorHandling::Error(s) {}
    };

  }

  void Durchfuehrung (const std::string& ExperimentVerzeichnis);

}


#endif

#define EXPERIMENTDURCHFUEHRUNGWAECHTER
