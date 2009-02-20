// Oliver Kullmann, 3.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/demo_Messages_3.cpp
  \brief Demonstration of usage of Messages-components (with potential compilation error in case of missing implementations)
*/

#include <sstream>
#include <cassert>

#include <OKlib/Programming/Messages/MessagesMain.hpp>

namespace OKlib {
  namespace Messages {
    namespace demo {

      OKLIB_USING_MESSAGES

      /*!
        \class Message_3
        \brief Speaks english and german, both in basic level, and english also in full level : IMPOSSIBLE.

        The problem here is that for example a request to print a message in language de_DE with level Full
        cannot be resolved: Shall it choose full english or basic german?! So we get a compilation error
        (an overload disambiguity).
        
        In this case the class must provide german also in both levels (if at all).
      */
      struct Message_3 : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT;
      
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          out << "English basic";
        }
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          out << "English full";
        }
        // void print(std::ostream& out, L<de_DE>, S<Basic>) const {
        //   out << "Deutsch einfach"; // "German basic"
        // }
        
      };

      /*!
        \class Message_3_variation
        \brief Speaks english in basic level and german in full level

        This (somewhat strange) combination does not lead to ambiguities.
      */
      struct Message_3_variation : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT;
      
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          out << "English basic";
        }
        void print(std::ostream& out, L<de_DE>, S<Full>) const {
          out << "Deutsch vollständig"; // "German full"
        }
        
      };

    }
  }
}

int main() {
  using namespace OKlib;
  std::stringstream out;
  const Messages::demo::Message_3_variation m;

  assert(Messages::MessagesBase::language(out) == Messages::en_GB);
  assert(Messages::MessagesBase::level(out) == Messages::Basic);

  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Full);
  assert(Messages::MessagesBase::language(out) == Messages::en_GB);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Extensive);
  assert(Messages::MessagesBase::language(out) == Messages::en_GB);
  assert(Messages::MessagesBase::level(out) == Messages::Extensive);
  out <<m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::de_DE);
  assert(Messages::MessagesBase::language(out) == Messages::de_DE);
  assert(Messages::MessagesBase::level(out) == Messages::Extensive);
  out << m;
  assert(out.str() == "Deutsch vollständig");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Full);
  assert(Messages::MessagesBase::language(out) == Messages::de_DE);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "Deutsch vollständig");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Basic);
  assert(Messages::MessagesBase::language(out) == Messages::de_DE);
  assert(Messages::MessagesBase::level(out) == Messages::Basic);
  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::fr_FR); // french as in France
  assert(Messages::MessagesBase::language(out) == Messages::fr_FR);
  assert(Messages::MessagesBase::level(out) == Messages::Basic);
  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Full);
  assert(Messages::MessagesBase::language(out) == Messages::fr_FR);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::de_AT); // german as in Austria
  assert(Messages::MessagesBase::language(out) == Messages::de_AT);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "Deutsch vollständig");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Extensive);
  assert(Messages::MessagesBase::language(out) == Messages::de_AT);
  assert(Messages::MessagesBase::level(out) == Messages::Extensive);
  out <<m;
  assert(out.str() == "Deutsch vollständig");
}

