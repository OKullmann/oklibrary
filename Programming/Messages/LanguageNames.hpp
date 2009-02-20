// Oliver Kullmann, 21.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/LanguageNames.hpp
  \brief Component for translating language names (as C-strings) into language constants (as C++ enumerated constants)
*/

#ifndef LANGUAGENAMES_janVVcd4
#define LANGUAGENAMES_janVVcd4

#include <functional>
#include <map>
#include <string>
#include <utility>
#include <stdexcept>

#include <OKlib/Programming/Messages/Languages.hpp>

namespace OKlib {
  namespace Messages {

    /*!
      \class LanguageName
      \brief Functor to translate language names into language constants.

      ::OKlib::Messages::LanguageName(name), where name is a C-string,
      yields a value of type ::OKlib::Messages::Languages
      (an enumeration type), while throwing an exception of type
      LanguageName::UninterpretableLanguageName
      in case name doesn't refer to an existing language.

      The available names are exactly the names of the enumerated constants in the enumeration type
      OKlib::Messages::Languages (so LanguageName(name) is just the value of name as C++ constant),
      which in turn are exactly the strings given by the macro OKLIB_LANGUAGES
      (in OKlib/Programming/Messages/Languages.hpp).
      Since the language names are made available by the array ::OKlib::Messages::Locales,
      class LanguageName is nothing than a find-wrapper for this array (enabling faster
      access than by just linearly searching through this array).

      For example LanguageName("en_GB") == ::OKlib::Messages::en_GB == ::OKlib::Messages::Languages(0).
    */

    class LanguageName : std::unary_function<const char*, ::OKlib::Messages::Languages> {

      typedef ::OKlib::Messages::Languages language_type;
      typedef std::map<std::string, language_type> map_type;
      static const map_type map;

      typedef std::pair<std::string, language_type> pair_type;
      struct PairFunction : std::unary_function<const char* const&, pair_type> {
        pair_type operator()(const char* const& name) const {
          return pair_type(std::string(name), static_cast<language_type>(&name - ::OKlib::Messages::Locales));
        }
      };
      //! The iterator used for the initialisation of object "map".
      typedef boost::transform_iterator<PairFunction, const char* const *> pair_iterator_type;

    public :

      struct UninterpretableLanguageName : std::runtime_error {
        UninterpretableLanguageName(const std::string message) : std::runtime_error(message) {}
      };

      language_type operator()(const char* const name) const {
        typedef map_type::const_iterator iterator;
        static const iterator& end(map.end());
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
