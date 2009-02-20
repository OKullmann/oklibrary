// Oliver Kullmann, 3.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/demo_Messages_2.cpp
  \brief Further demonstration of basic usage of Messages-components
*/

#include <sstream>
#include <cassert>

#include <OKlib/Programming/Messages/MessagesMain.hpp>

namespace OKlib {
  namespace Messages {
    namespace demo {

      OKLIB_USING_MESSAGES

      /*!
        \class Message_2
        \brief Speaks english and german, both only in basic level.
      */
      struct Message_2 : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT;
      
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          out << "English basic";
        }
        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          out << "Deutsch einfach"; // "German basic"
        }
      };

    }
  }
}

int main() {
  using namespace OKlib;
  std::stringstream out;
  const Messages::demo::Message_2 m;

  assert(Messages::MessagesBase::language(out) == Messages::en_GB);
  assert(Messages::MessagesBase::level(out) == Messages::Basic);

  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Full);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Extensive);
  assert(Messages::MessagesBase::level(out) == Messages::Extensive);
  out <<m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::de_DE);
  assert(Messages::MessagesBase::language(out) == Messages::de_DE);
  out << m;
  assert(out.str() == "Deutsch einfach");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Full);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "Deutsch einfach");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Basic);
  assert(Messages::MessagesBase::level(out) == Messages::Basic);
  out << m;
  assert(out.str() == "Deutsch einfach");

  out.str("");
  Messages::MessagesBase::set(out, Messages::fr_FR); // french as in France
  assert(Messages::MessagesBase::language(out) == Messages::fr_FR);
  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Full);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::de_AT); // german as in Austria
  assert(Messages::MessagesBase::language(out) == Messages::de_AT);
  out << m;
  assert(out.str() == "Deutsch einfach");
}

