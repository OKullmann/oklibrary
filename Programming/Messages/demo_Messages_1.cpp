// Oliver Kullmann, 3.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/demo_Messages_1.cpp
  \brief Demonstration of basic usage of Messages-components
*/

#include <sstream>
#include <cassert>

#include <OKlib/Programming/Messages/MessagesMain.hpp>
// this brings all basic functionality of Messages

namespace OKlib {
  namespace Messages {
    namespace demo {

      OKLIB_USING_MESSAGES
      // by this macro we get basic using-declarations for messages

      /*!
        \class Message_1
        \brief Speaks only english, in basic and full level.
      */
      struct Message_1 : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT
        // it is recommended to always begin like that for a message class
      
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          out << "English basic";
        }
        // L as "language", S as "strata"
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          out << "English full";
        }
      };

    }
  }
}

int main() {
  using namespace OKlib;
  std::stringstream out;
  const Messages::demo::Message_1 m;

  assert(Messages::MessagesBase::language(out) == Messages::en_GB);
  assert(Messages::MessagesBase::level(out) == Messages::Basic);

  out << m;
  assert(out.str() == "English basic");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Full);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "English full");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Extensive);
  assert(Messages::MessagesBase::level(out) == Messages::Extensive);
  out <<m;
  assert(out.str() == "English full");

  out.str("");
  Messages::MessagesBase::set(out, Messages::de_DE);
  // German as spoken in Germany; has no effect here
  assert(Messages::MessagesBase::language(out) == Messages::de_DE);
  out << m;
  assert(out.str() == "English full");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Full);
  assert(Messages::MessagesBase::level(out) == Messages::Full);
  out << m;
  assert(out.str() == "English full");

  out.str("");
  Messages::MessagesBase::set(out, Messages::Basic);
  assert(Messages::MessagesBase::level(out) == Messages::Basic);
  out << m;
  assert(out.str() == "English basic");

  assert(Messages::MessagesBase::language(out) == Messages::de_DE);
}

