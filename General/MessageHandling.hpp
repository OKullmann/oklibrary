// Oliver Kullmann, 19.11.2002 (Swansea)

#ifndef MESSAGEHANDLINGWAECHTER

#define MESSAGEHANDLINGWAECHTER

#include <string>
#include <map>
#include <utility>

// #include <iostream> // debugging

namespace MessageHandling {

  enum Sprachen {Deutsch = 0, English = 1};

}

namespace MessageHandling {

  // Very simple class for simplified handling of a single message catalogue
  // Initialised with (e.g.)

  /*
    const int number_languages = 2;
    const char * const messages_[][number_languages] = {
    {"message 0 in German", // 0
    "message 0 in English"},
    {"message 1 in German", // 1
    "message 1 in English"},
    }

    Messages M<number_languages>(messages_, Deutsch);
  */

  // Now with M(0), M(1), ... the messages in German are obtained,
  // while with
  /*
    M.set_language(English);
  */
  // the current language is set to English.

  template <int number_languages>
  class Messages {
  public :
    Messages(const char * const message_catalogue[][number_languages], Sprachen Language) : catalogue(&message_catalogue[0][0]), L(Language) {}
    const char * operator() (int i) const { return catalogue[i * number_languages + L]; }
    void set_language(Sprachen Language) { L = Language; }
  private :
    const char * const * const catalogue;
    Sprachen L;
  };

}

namespace MessageHandling {

  template <typename catalogue_types>
  class Message_catalogues {
  public :

    Message_catalogues(void (*Initialiser) (Message_catalogues&)) :
    current_language(static_cast<Sprachen>(0)), current_catalogue(static_cast<catalogue_types>(0))
    {
      Initialiser(const_cast<Message_catalogues&>(*this));
    }

    void set_language(Sprachen c_l) const { current_language = c_l; }
    void set_catalogue(catalogue_types c_c) const { current_catalogue = c_c; }

    const char* & operator [] (const int name) {
      return M[std::make_pair(name, std::make_pair(current_catalogue, current_language))];
    }

    const char* const & operator [] (const int name) const {
      return M.find(std::make_pair(name, std::make_pair(current_catalogue, current_language))) -> second;
    }

  private :
    typedef std::pair< int, std::pair<catalogue_types, Sprachen> > Indices;
    std::map< Indices, const char* > M;
    mutable Sprachen current_language;
    mutable catalogue_types current_catalogue;
  };

}


#endif
