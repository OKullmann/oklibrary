// TO DO: Licence

#ifndef MESSAGESUSERINTERFACE_danc6dhhMmi

#define MESSAGESUSERINTERFACE_danc6dhhMmi

#include "Messages_Models_Definitions.hpp"

#include <boost/mpl/vector.hpp>

namespace OKlib {
  
  namespace Messages {

    namespace UserInterface {

      extern const char deutsch[] = "Deutsch";
      extern const char english[] = "English";
      extern const char chinese[] = "Chinese";
      extern const char french[] = "French";
      extern const char dutch[] = "Dutch";
      extern const char portuguese[] = "Portuguese";
      extern const char japanese[] = "Japanese";
      extern const char italian[] = "Italian";
      extern const char hungarian[] = "Hungarian";
      
      typedef boost::mpl::vector<Messages::W<deutsch> > min_languages;
      
      typedef Messages::MessageService<english, min_languages> MessageService;

      typedef Messages::MessageService<english, min_languages, char, std::char_traits<char>, std::string> MsgService;

    }
  }
}

#endif
