// Oliver Kullmann, 11.2.2006 (Swansea)

/*!
  \file Messages_Testapplication.cpp
  \brief DEPRECATED. Temporary test application
*/

#include <iostream>
#include <memory>

#include <Transitional/Messages/MessagesMain.hpp>

#include <Transitional/Messages/messages/Languages.hpp>
#include <Transitional/Messages/messages/Levels.hpp>

namespace Implementation {

  OKLIB_USING_MESSAGES

  OKLIB_MESSAGES(M1) {
    OKLIB_MESSAGES_PRINT

      void print(std::ostream& out, L<en_GB>, S<Basic>) const {
      out << "M1: en_GB, Basic; pi = " << 3.14 << "\n";
    }
      void print(std::ostream& out, L<en_GB>, S<Full>) const {
      out << "M1: en_GB, Full; pi = " << 3.14 << "\n";
    }

      void print(std::ostream& out, L<de_DE>, S<Basic>) const {
      out << "M1: de_DE, Basic; pi = " << 3.14 << "\n";
    }
      void print(std::ostream& out, L<de_DE>, S<Full>) const {
      out << "M1: de_DE, Full; pi = " << 3.14 << "\n";
    }
    
      void print(std::ostream& out, L<fr_FR>, S<Basic>) const {
      out << "M1: fr_FR, Basic; pi = " << 3.14 << "\n";
    }
      void print(std::ostream& out, L<fr_FR>, S<Full>) const {
      out << "M1: fr_FR, Full; pi = " << 3.14 << "\n";
    }

      void print(std::ostream& out, L<fr_CA>, S<Basic>) const {
      out << "M1: fr_CA, Basic; pi = " << 3.14 << "\n";
    }
      void print(std::ostream& out, L<fr_CA>, S<Full>) const {
      out << "M1: fr_CA, Full; pi = " << 3.14 << "\n";
    }
  
  };

}

int main() {
  OKLIB_USING_MESSAGES;
  using Implementation::M1;

  {
    ::OKlib::Messages::messages::LanguageNames names;
    ::OKlib::Messages::messages::LevelNames l_names;
    for (unsigned int i = 0; i != OKlib::Messages::number_of_languages; ++i) {
      std::cout << "i = " << i << "\n";
      M1::set(std::cout, ::OKlib::Messages::Languages(i));
      for (unsigned int i2 = 0; i2 != OKlib::Messages::number_of_levels; ++i2) {
        std::cout << "i2 = " << i2 << "\n";
        M1::set(std::cout, ::OKlib::Messages::Strata(i2));
        for (unsigned int j = 0; j != OKlib::Messages::number_of_languages; ++j) {
          names.foreign_language() = ::OKlib::Messages::Languages(j);
          std::cout << names << "; ";
        }
        std::cout << "\n";
        for (unsigned int j = 0; j != OKlib::Messages::number_of_levels; ++j) {
          l_names.other_level() = ::OKlib::Messages::Strata(j);
          std::cout << l_names << "; ";
        }
        std::cout << "\n";
      }
    }
  }

  std::cout << "\nStart special output\n";
  std::cout << M1();
  M1::set(std::cout, de_DE);
  M1::set(std::cout, Full);
  std::cout << M1();
  M1::set(std::cout, en_GB);
  M1::set(std::cout, Extensive);
  std::cout << M1();
  M1::set(std::cout, Basic);
  std::cout << M1();
  M1::set(std::cout, fr_FR);
  std::cout << M1();
  M1::set(std::cout, fr_CA);
  std::cout << M1();

  std::auto_ptr<OKlib::Messages::MessagesBase> mp(new M1);
  std::cout << *mp;
  mp -> set(std::cout, de_AT);
  mp -> set(std::cout, Extensive);
  std::cout << *mp;
}
