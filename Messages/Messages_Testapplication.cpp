#include <iostream>
#include <string>
#include <cassert>

#include "Messages_Models_Definitions.hpp"
#include "Messages_UserInterface.hpp"

namespace Messages_UserInterface = ::OKlib::Messages::UserInterface;
namespace Messages = ::OKlib::Messages;

typedef Messages_UserInterface::MessageService Service;
typedef Service::Messages_base message_type;
typedef Service::string_type string_type;
typedef Service::Language<Messages_UserInterface::deutsch> Deutsch_type;
typedef Service::Language<Messages_UserInterface::english> English_type;
typedef Service::Language<Messages_UserInterface::chinese> Chinese_type;

struct Test_message : message_type {
  const Test_message* identity() const {
    return this;
  }
  string_type translate(const Deutsch_type*) const {
    return "Deutsche Meldung!";
  }
  string_type translate(const English_type*) const {
    return "English message!";
  }
//   string_type translate(const Service::Chinese_type*) const {
//     return "Chinese message!";
//   }  
};

typedef Messages::MessageService<Messages_UserInterface::english, Messages_UserInterface::min_languages, char, std::char_traits<char>, std::string> ServiceS;
typedef ServiceS::Messages_base message_typeS;
typedef ServiceS::string_type string_typeS;
typedef ServiceS::Language<Messages_UserInterface::deutsch> Deutsch_typeS;
typedef ServiceS::Language<Messages_UserInterface::english> English_typeS;
typedef ServiceS::Language<Messages_UserInterface::chinese> Chinese_typeS;

struct Test_messageS : message_typeS {
  const Test_messageS* identity() const {
    return this;
  }
  string_typeS translate(const Deutsch_typeS*) const {
    return "Deutsche MeldungS!";
  }
  string_typeS translate(const English_typeS*) const {
    return "English messageS!";
  }
  string_typeS translate(const Chinese_typeS*) const {
    return "Chinese message!";
  }  
};

extern const char new_language[] = "See!";

int main() {
  std::cout << Deutsch_type::ptr() -> name() << std::endl;
  const Test_message tm;
  std::cout << tm << "\n";
  std::cout << Deutsch_type::ptr();
  std::cout << tm << "\n";
  std::cout << tm << "\n";
  // std::cout << Chinese_type::ptr() << tm << "\n";
  std::cout << "End const char*." << std::endl;

  std::cout << Deutsch_typeS::ptr() -> name() << std::endl;
  const Test_messageS tmS;
  std::cout << tmS << "\n";
  std::cout << Deutsch_typeS::ptr();
  std::cout << tmS << "\n";
  std::cout << tmS << "\n";
  std::cout << English_type::ptr() << tmS << tm << "\n";
  std::cout << ServiceS::Languages::language(Deutsch_typeS::ptr() -> name()) << tmS << "\n";
  std::cout << "End string." << std::endl;

  typedef ServiceS::Languages::iterator iteratorS;
  ServiceS::Languages::iterator po = ServiceS::Languages::begin();
  assert(po == ServiceS::Languages::begin());
  std::cout << (*po) -> name() << " " << *po << tmS << "\n"; //Deutsch
  std::cout << (*(po++)) -> name() << "\n"; //Deutsch
  std::cout << (*po) -> name() << "\n"; //English
  po = ServiceS::Languages::begin(); //Deutsch
  std::cout << (*(++po)) -> name() << "\n"; //English
  std::cout << "_--------------------------_\n";
  std::cout << (*(--po)) -> name() << "\n"; //Deutsch
  std::cout << (*(++po)) -> name() << "\n"; //English
  std::cout << (*(po--)) -> name() << "\n"; //English
  std::cout << (*po) -> name() << "\n"; //Deutsch
  
  assert(po != ServiceS::Languages::end());
  ServiceS::Languages::iterator po2(po);
  assert(po == po2);
  for (iteratorS i = ServiceS::Languages::begin(); i != ServiceS::Languages::end(); ++i)
    std::cout << (*i) -> name() << ", ";
  std::cout << "\n";

  
}
