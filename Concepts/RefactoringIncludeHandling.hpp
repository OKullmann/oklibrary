// Oliver Kullmann, 23.12.2005 (Swansea)

/*!
  \file RefactoringIncludeHandling.hpp
  \brief Concepts for refactoring tools regarding replacement of include directives.
*/

#ifndef REFACTORINGINCLUDEHANDLING_jjHHbgt6

#define REFACTORINGINCLUDEHANDLING_jjHHbgt6

#include "LibraryBasics.hpp"
#include "Traits.hpp"

#include "InputOutput.hpp"

#include "BoostPathCorrected.hpp"

#include "IncludeHandling.hpp"

namespace OKlib {

  namespace Concepts {

    struct IncludeDirective_tag : virtual BasicRequirements_tag, virtual FullyConstructibleEq_tag,  virtual EqualitySubstitutable_tag, virtual OutputStreamable_tag {};

    template <class IncDir>
    struct IncludeDirective {
      typedef typename string_type<IncDir>::type string_type;
      typedef typename size_type<IncDir>::type size_type;
      typedef OKlib::Refactoring::Include_forms Include_forms;
      BOOST_STATIC_ASSERT(::OKlib::MetaProgramming::is_unqualified_unsigned_integral<size_type>::value);
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

        static_cast<string_type>(dir_c.header_file());
        static_cast<string_type&>(dir.header_file());
        static_cast<size_type>(dir_c.number_spaces_after_hash());
        static_cast<size_type&>(dir.number_spaces_after_hash());
        static_cast<size_type>(dir_c.number_spaces_after_include());
        static_cast<size_type&>(dir.number_spaces_after_include());
        static_cast<Include_forms>(dir_c.include_form());
        static_cast<Include_forms&>(dir.include_form());
        static_cast<string_type>(dir_c.opening());
        static_cast<string_type>(dir.opening());
        static_cast<string_type>(dir_c.closing());
        static_cast<string_type>(dir.closing());
      }
      IncDir dir;
      const IncDir dir_c;
      const size_type s_c;
      const OKlib::Refactoring::Include_forms if_c;
      const string_type str_c;
    };

    class IncludeDirective_Archetype {
      struct convertible_to_bool {
        operator bool() {}
      };
    public :
      typedef IncludeDirective_tag concept_tag;
      struct string_type {};
      typedef unsigned int size_type; // ToDo: Can this be made more generic?
      convertible_to_bool operator ==(const IncludeDirective_Archetype&) const {}
      convertible_to_bool operator !=(const IncludeDirective_Archetype&) const {}
      friend std::ostream& operator <<(std::ostream&, const IncludeDirective_Archetype&) {}
      IncludeDirective_Archetype() {}
      IncludeDirective_Archetype(string_type,size_type,size_type,OKlib::Refactoring::Include_forms) {}
      operator boost::filesystem::path() const {}
      string_type header_file() const {}
      string_type& header_file() {}
      size_type number_spaces_after_hash() const {}
      size_type& number_spaces_after_hash() {}
      size_type number_spaces_after_include() const {}
      size_type& number_spaces_after_include() {}
      OKlib::Refactoring::Include_forms include_form() const {}
      OKlib::Refactoring::Include_forms& include_form() {}
      string_type opening() const {}
      string_type& opening() {}
      string_type closing() const {}
      string_type& closing() {}
    };

  }

}

#endif
