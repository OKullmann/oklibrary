// Oliver Kullmann, 11.2.2006 (Swansea)

/*!
  \file Messages_Testapplication_DesignStudy.cpp
  \brief Temporary test application
*/

#include <iostream>
#include <memory>

#include "Messages_DesignStudy.hpp"

OKLIB_MESSAGES(M1) {
  OKLIB_MESSAGES_PRINT

  template <class Stream>
    void print(Stream& out, OKlib::Messages::L<OKlib::Messages::en_GB>, OKlib::Messages::S<OKlib::Messages::Basic>) const {
    out << "M1: en_GB, Basic; pi = " << 3.14 << "\n";
  }
  template <class Stream>
    void print(Stream& out, OKlib::Messages::L<OKlib::Messages::en_GB>, OKlib::Messages::S<OKlib::Messages::Full>) const {
    out << "M1: en_GB, Full; pi = " << 3.14 << "\n";
  }

  template <class Stream>
    void print(Stream& out, OKlib::Messages::L<OKlib::Messages::de_DE>, OKlib::Messages::S<OKlib::Messages::Basic>) const {
    out << "M1: de_DE, Basic; pi = " << 3.14 << "\n";
  }
  template <class Stream>
    void print(Stream& out, OKlib::Messages::L<OKlib::Messages::de_DE>, OKlib::Messages::S<OKlib::Messages::Full>) const {
    out << "M1: de_DE, Full; pi = " << 3.14 << "\n";
  }

  template <class Stream>
    void print(Stream& out, OKlib::Messages::L<OKlib::Messages::fr_FR>, OKlib::Messages::S<OKlib::Messages::Basic>) const {
    out << "M1: fr_FR, Basic; pi = " << 3.14 << "\n";
  }
  template <class Stream>
    void print(Stream& out, OKlib::Messages::L<OKlib::Messages::fr_FR>, OKlib::Messages::S<OKlib::Messages::Full>) const {
    out << "M1: fr_FR, Full; pi = " << 3.14 << "\n";
  }
  
};

int main() {
  using ::OKlib::Messages::translate;
  using ::OKlib::Messages::L;
  using ::OKlib::Messages::S;

  std::cout << translate(L<OKlib::Messages::en_US>(), L<OKlib::Messages::de_AT>()) << "\n";
  std::cout << translate(L<OKlib::Messages::en_US>(), L<OKlib::Messages::fr_CH>()) << "\n";
  std::cout << translate(L<OKlib::Messages::de_CH>(), L<OKlib::Messages::en_US>()) << "\n";
  std::cout << translate(L<OKlib::Messages::fr_CH>(), L<OKlib::Messages::de_AT>()) << "\n";

  std::cout << translate(S<OKlib::Messages::Basic>(), L<OKlib::Messages::en_US>()) << "\n";
  std::cout << translate(S<OKlib::Messages::Full>(), L<OKlib::Messages::de_AT>()) << "\n";

  std::cout << "\n";
  std::cout << M1();
  M1::set(std::cout, OKlib::Messages::de_DE);
  M1::set(std::cout, OKlib::Messages::Full);
  std::cout << M1();
  M1::set(std::cout, OKlib::Messages::en_GB);
  M1::set(std::cout, OKlib::Messages::Extensive);
  std::cout << M1();
  M1::set(std::cout, OKlib::Messages::Basic);
  std::cout << M1();
  M1::set(std::cout, OKlib::Messages::fr_FR);
  std::cout << M1();

  std::auto_ptr<M1> mp(new M1);
  std::cout << *mp;
  mp -> set(std::cout, OKlib::Messages::de_AT);
  mp -> set(std::cout, OKlib::Messages::Extensive);
  std::cout << *mp;
}
