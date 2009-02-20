// Oliver Kullmann, 23.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/RefactoringIncludeHandling.hpp
  \brief Concepts for refactoring tools regarding replacement of include directives.
*/

#ifndef REFACTORINGINCLUDEHANDLING_jjHHbgt6
#define REFACTORINGINCLUDEHANDLING_jjHHbgt6

#include <OKlib/Concepts/LibraryBasics.hpp>

#include <OKlib/traits/TypeTraits.hpp>

#include <OKlib/Programming/Refactoring/traits/string_type.hpp>
#include <OKlib/Programming/Refactoring/traits/size_type.hpp>

#include <OKlib/Concepts/InputOutput.hpp>

#include <OKlib/Programming/Refactoring/BoostPathCorrected.hpp>

#include <OKlib/Programming/Refactoring/IncludeHandling.hpp>

namespace OKlib {
  namespace Concepts {

    struct IncludeDirective_tag : virtual BasicRequirements_tag, virtual FullyConstructibleEq_tag,  virtual EqualitySubstitutable_tag, virtual OutputStreamable_tag {};

    template <class IncDir>
    struct IncludeDirective {
      typedef typename ::OKlib::Refactoring::traits::string_type<IncDir>::type string_type;
      typedef typename ::OKlib::Refactoring::traits::size_type<IncDir>::type size_type;
      typedef ::OKlib::Refactoring::Include_forms Include_forms;
      BOOST_STATIC_ASSERT(::OKlib::traits::is_unqualified_unsigned_integral<size_type>::value);
      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(IncDir, IncludeDirective);

        OKLIB_MODELS_CONCEPT_REQUIRES(IncDir, BasicRequirements);
        OKLIB_MODELS_CONCEPT_TAG(IncDir, BasicRequirements);
        OKLIB_MODELS_CONCEPT_REQUIRES(IncDir, FullyConstructibleEq);
        OKLIB_MODELS_CONCEPT_TAG(IncDir, FullyConstructibleEq);
        OKLIB_MODELS_CONCEPT_REQUIRES(IncDir, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_TAG(IncDir, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_REQUIRES(IncDir, OutputStreamable);
        OKLIB_MODELS_CONCEPT_TAG(IncDir, OutputStreamable);

        OKLIB_MODELS_CONCEPT_REQUIRES(string_type, FullyConstructible);

        IncDir(str_c, s_c, s_c, if_c);
        static_cast<boost::filesystem::path>(dir_c);

        size_type siz; string_type str; Include_forms inc;
        str = static_cast<string_type>(dir_c.header_file());
        static_cast<string_type&>(dir.header_file()) = str;
        siz = static_cast<size_type>(dir_c.number_spaces_after_hash());
        static_cast<size_type&>(dir.number_spaces_after_hash()) = siz;
        siz = static_cast<size_type>(dir_c.number_spaces_after_include());
        static_cast<size_type&>(dir.number_spaces_after_include()) = siz;
        inc = static_cast<Include_forms>(dir_c.include_form());
        static_cast<Include_forms&>(dir.include_form()) = inc;
        str = static_cast<string_type>(dir_c.opening());
        str = static_cast<string_type>(dir.opening());
        str = static_cast<string_type>(dir_c.closing());
        str = static_cast<string_type>(dir.closing());
      }
      IncDir dir;
      const IncDir dir_c;
      const size_type s_c;
      const OKlib::Refactoring::Include_forms if_c;
      const string_type str_c;
    };

    class IncludeDirective_Archetype {
      struct convertible_to_bool {
        operator bool() { return bool(); }
      };
    public :
      typedef IncludeDirective_tag concept_tag;
      struct string_type {};
      typedef unsigned int size_type; // ToDo: Can this be made more generic?
      convertible_to_bool operator ==(const IncludeDirective_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator !=(const IncludeDirective_Archetype&) const { return convertible_to_bool(); }
      friend std::ostream& operator <<(std::ostream& out, const IncludeDirective_Archetype&) { return out; }
      IncludeDirective_Archetype() {}
      IncludeDirective_Archetype(string_type,size_type,size_type,OKlib::Refactoring::Include_forms) {}
      operator boost::filesystem::path() const { return boost::filesystem::path(); }
    private :
      string_type str;
      size_type siz;
      OKlib::Refactoring::Include_forms inc;
    public :
      string_type header_file() const { return string_type(); }
      string_type& header_file() { return str; }
      size_type number_spaces_after_hash() const { return size_type(); }
      size_type& number_spaces_after_hash() { return siz; }
      size_type number_spaces_after_include() const { return size_type(); }
      size_type& number_spaces_after_include() { return siz; }
      OKlib::Refactoring::Include_forms include_form() const { return OKlib::Refactoring::Include_forms(); }
      OKlib::Refactoring::Include_forms& include_form() { return inc; }
      string_type opening() const { return string_type(); }
      string_type closing() const { return string_type(); }
    };

  }

}

#endif
