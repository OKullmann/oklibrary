// Oliver Kullmann, 26.9.2003 (Swansea)

#ifndef CONCEPTGENERALWAECHTER_hjahfd5252hajcbvs

#define CONCEPTGENERALWAECHTER_hjahfd5252hajcbvs

namespace Concepts_General {

  // -----------------------------------------------------------
  // Basic Concepts
  // -----------------------------------------------------------

  template <typename T>
  struct FullyConstructibleConcept {
    FullyConstructibleConcept();
    void constraints() {
      boost::function_requires<boost::DefaultConstructibleConcept<T> >();
      boost::function_requires<boost::CopyConstructibleConcept<T> >();
      boost::function_requires<boost::AssignableConcept<T> >();
    }
  };
    
  template <typename T>
  struct OutputStreamableConcept {
    OutputStreamableConcept();
    void constraints() {
      o << v;
    }
    T v;
    std::ostream o;
  };

  // -----------------------------------------------------------
  // Order Concepts
  // -----------------------------------------------------------

  template <class T>
  struct TotalOrderComparableConcept {
    TotalOrderComparableConcept();
    void constraints() {
      boost::function_requires<boost::LessThanComparableConcept<T> >();
      boost::function_requires<boost::EqualityComparableConcept<T> >();
      // For a, b of type T we must have:
      // If not (a < b) and not (b < a) then a == b.
      // And if a == b then not (a < b).
      // Thus a <= b (i.e., not (b < a)) is equivalent to (a < b or a == b).
    }
  };

  // -----------------------------------------------------------
  // Concept Tags for library-specific tags
  // (used for the test class template)
  // -----------------------------------------------------------

  struct Root_tag {};



}

#endif



