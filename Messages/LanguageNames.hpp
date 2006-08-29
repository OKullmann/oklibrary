// Oliver Kullmann, 21.2.2006 (Swansea)

/*!
  \file LanguageNames.hpp
  \brief Component for translating language names into language constants

  \todo Write tests!
  \todo Once available, the std::map in LanguageName should be replaced
  by a hash map; and finally a specialised string-algorithm shall be used.
*/

#ifndef LANGUAGENAMES_janVVcd4
#define LANGUAGENAMES_janVVcd4

#include <functional>
#include <map>
#include <string>
#include <utility>
#include <stdexcept>

#include <Transitional/Messages/Languages.hpp>

namespace OKlib {

  namespace Messages {

    /*!
      \class LanguageName
      \brief Functor to translate language names into language constants.

      ::OKlib::Messages::LanguageName(name) yields a value of type
      ::OKlib::Messages::Languages, throwing an exception
      LanguageName::UninterpretableLanguageName
      in case name doesn't refer to an existing language.
    */

    class LanguageName : std::unary_function<const char*, Languages> {

      typedef std::map<std::string, Languages> map_type;
      static const map_type map;

      typedef std::pair<std::string, Languages> pair_type;
      struct PairFunction : std::unary_function<const char* const&, pair_type> {
        pair_type operator()(const char* const& name) const {
          return pair_type(std::string(name), static_cast<Languages>(&name - ::OKlib::Messages::Locales));
        }
      };
      typedef boost::transform_iterator<PairFunction, const char* const *> pair_iterator_type;

      struct UninterpretableLanguageName : std::runtime_error {
        UninterpretableLanguageName(const std::string message) : std::runtime_error(message) {}
      };

    public :

      Languages operator()(const char* const name) const {
        typedef map_type::const_iterator iterator;
        static const iterator end(map.end());
        const iterator& found(map.find(std::string(name)));
        if (found == end)
          throw UninterpretableLanguageName(name);
        else
          return found -> second;
      }
    };
    const LanguageName::map_type LanguageName::map(LanguageName::pair_iterator_type(::OKlib::Messages::Locales), LanguageName::pair_iterator_type(::OKlib::Messages::Locales + ::OKlib::Messages::number_of_languages));

  }

}

#endif
