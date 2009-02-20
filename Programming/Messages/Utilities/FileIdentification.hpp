// Oliver Kullmann, 17.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/Utilities/FileIdentification.hpp
  \brief Message class to provide source-code-file information
*/

#ifndef FILEIDENTIFICATION_GvCdSw2929Uy6
#define FILEIDENTIFICATION_GvCdSw2929Uy6

#include <string>

#include <OKlib/Programming/Messages/MessagesMain.hpp>
#include <OKlib/Programming/Messages/LineHandling.hpp>

namespace OKlib {
  namespace Messages {
    namespace Utilities {

      /*!
        \class FileIdentification
        \brief Basic source-code-file information

        Contains
         - file name
         - compilation date
         - compilation time
         - last change date w.r.t. version control
         - current version (in the version control system).

        Mainly to be used internally, as the underlying message-class for the macro
        OKLIB_FILE_ID (see for example TestSystem/TestBaseClass_DesignStudy.hpp).
      */

      OKLIB_USING_MESSAGES

      struct FileIdentification : ::OKlib::Messages::MessagesPrePost {
        OKLIB_MESSAGES_PRINT

        FileIdentification(
                           std::string const file,
                           std::string const date = std::string(),
                           std::string const time = std::string(),
                           std::string const change = std::string(),
                           std::string const version = std::string()) :
          file(file),
          date(date),
          time(time),
          change(change),
          version(version)
        {}

        template <class L>
        void print(std::ostream& out, L, S<Basic>) const {
          print_file(out, L());
        }
        template <class L>
        void print(std::ostream& out, L, S<Full>) const {
          print_file(out, L());
          print_compilation(out, L());
        }
        template <class L>
        void print(std::ostream& out, L, S<Extensive>) const {
          print_file(out, L());
          print_compilation(out, L());
          print_version(out, L());
        }

      private :

        void print_file(std::ostream& out, L<en_GB>) const {
          l_start(out) << "file name = " << file;
        }
        void print_file(std::ostream& out, L<de_DE>) const {
          l_start(out) << "Dateiname = " << file;
        }

        void print_compilation(std::ostream& out, L<en_GB>) const {
          if (not date.empty()) {
            l_end(out); l_start(out) << "compilation date = " << date;
            if (not time.empty())
              out << ", compilation time = " << time;
          }
          else if (not time.empty()) {
            l_end(out); l_start(out) << "compilation time = " << time;
          }
        }
        void print_compilation(std::ostream& out, L<de_DE>) const {
          if (not date.empty()) {
            l_end(out); l_start(out) << "Datum der Programm-Übersetzung = " << date;
            if (not time.empty())
              out << ", wobei Übersetzungszeit = " << time;
          }
          else if (not time.empty()) {
            l_end(out); l_start(out) << "Programm-Übersetzungszeit = " << time;
          }
        }

        void print_version(std::ostream& out, L<en_GB>) const {
          if (not change.empty()) {
            l_end(out); l_start(out) << "last change date = " << strip(change, "$" "Date: ", " $");
            if (not version.empty())
              out << ", version number = " << strip(version, "$" "Revision: ", " $");
          }
          else if (not version.empty()) {
            l_end(out); l_start(out) << "version number = " << strip(version, "$" "Revision: ", " $");
          }
        }
        void print_version(std::ostream& out, L<de_DE>) const {
          if (not change.empty()) {
            l_end(out); l_start(out) << "Datum der letzten Änderung = " << strip(change, "$" "Date: ", " $");
            if (not version.empty())
              out << ", Versionsnummer = " << strip(version, "$" "Revision: ", " $");
          }
          else if (not version.empty()) {
            l_end(out); l_start(out) << "Versionsnummer = " << strip(version, "$" "Revision: ", " $");
          }
        }
       

        static std::string strip(std::string s, const std::string begin, const std::string end) {
          assert(s.size() > begin.size() + end.size());
          assert(s.substr(0, begin.size()) == begin);
          assert(s.substr(s.size() - end.size()) == end);
          s.erase(0, begin.size());
          s.erase(s.size() - end.size());
          return s;
        }

        std::string file;
        std::string date;
        std::string time;
        std::string change;
        std::string version;
      };
      
    }
  }
}

#endif
