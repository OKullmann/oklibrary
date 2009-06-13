// Oliver Kullmann, 15.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef VANDERWAERDENHYPERGRAPHTESTS_oppQyyr8838
#define VANDERWAERDENHYPERGRAPHTESTS_oppQyyr8838

#include <iterator>
#include <string>

#include <boost/lexical_cast.hpp>
 
#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Concepts/Iterators.hpp>
#include <OKlib/Concepts/LibraryBasics.hpp>

#include <OKlib/Applications/RamseyTheory/Van_der_Waerden_hypergraph.hpp>

namespace OKlib {
  namespace Van_der_Waerden_hypergraph {

    template <class Iterator_arithmetic_progression>
    class Test_Iterator_arithmetic_progression : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_Iterator_arithmetic_progression test_type;
    private :
      void perform_test_trivial() {
        // ToDo: Define (general) tests for InputIterator and MultiPass_InputIterator, and apply it here.
        // ToDo: Define concept "IteratorArithmeticProgression" and check it here.
        OKLIB_MODELS_CONCEPT_REQUIRES(Iterator_arithmetic_progression, ::OKlib::Concepts::MultiPassInputIterator);
        OKLIB_MODELS_CONCEPT_TAG(Iterator_arithmetic_progression, ::OKlib::Concepts::MultiPassInputIterator);
        typedef typename std::iterator_traits<Iterator_arithmetic_progression>::value_type value_type;
        {
          Iterator_arithmetic_progression end(11,0);
          value_type count = 0;
          for (Iterator_arithmetic_progression begin(2,3); begin != end; ++begin, ++count) ;
          const value_type steps =  (11 - 2) / 3;
          if (count != steps)
            OKLIB_THROW("Count is " + boost::lexical_cast<std::string>(count) + ", and not " + boost::lexical_cast<std::string>(steps));
        }
        {
          // ToDo: to be completed.
        }
      }
    };

    template <template <typename Integer> class Iterator_arithmetic_progression>
    class Test_Iterator_arithmetic_progression_Integer : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Iterator_arithmetic_progression_Integer test_type;
      Test_Iterator_arithmetic_progression_Integer() {
        insert(this);
      }
    public :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_Iterator_arithmetic_progression<Iterator_arithmetic_progression<unsigned int> >());
        OKLIB_TESTTRIVIAL_RETHROW(Test_Iterator_arithmetic_progression<Iterator_arithmetic_progression<int> >());
        OKLIB_TESTTRIVIAL_RETHROW(Test_Iterator_arithmetic_progression<Iterator_arithmetic_progression<long unsigned int> >());
        OKLIB_TESTTRIVIAL_RETHROW(Test_Iterator_arithmetic_progression<Iterator_arithmetic_progression<long int> >());
      }
    };

    // ------------------------------------------------------------------------------------------------------------------------

    template <class Arithmetic_progression>
    class Test_Arithmetic_progression : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_Arithmetic_progression test_type;
    private :
      typedef typename Arithmetic_progression::value_type value_type;
      typedef typename Arithmetic_progression::iterator iterator;
      void perform_test_trivial() {
        // ToDo: Conceptual check
        {
          const value_type start = 3;
          const value_type length = 3;
          const value_type slope = 2;
          const Arithmetic_progression ap(start,length,slope);
          if (ap.start() != start)
            throw ::OKlib::TestSystem::TestException("Start is " + boost::lexical_cast<std::string>(ap.start()) + ", and not " + boost::lexical_cast<std::string>(start)).add(OKLIB_TESTDESCRIPTION);
          if (ap.size() != length)
            throw ::OKlib::TestSystem::TestException("Size is " + boost::lexical_cast<std::string>(ap.size()) + ", and not " + boost::lexical_cast<std::string>(length)).add(OKLIB_TESTDESCRIPTION);
          if (ap.slope() != slope)
            throw ::OKlib::TestSystem::TestException("Slope is " + boost::lexical_cast<std::string>(ap.slope()) + ", and not " + boost::lexical_cast<std::string>(slope)).add(OKLIB_TESTDESCRIPTION);
          value_type steps = 0;
          for (iterator i = ap.begin(); i != ap.end(); ++i, ++steps) {
            value_type element = start + steps * slope;
            if (*i != element)
              throw ::OKlib::TestSystem::TestException("Element is " + boost::lexical_cast<std::string>(*i) + ", and not " + boost::lexical_cast<std::string>(element)).add(OKLIB_TESTDESCRIPTION);
          }
          if (steps != length)
            throw ::OKlib::TestSystem::TestException("Step count is " + boost::lexical_cast<std::string>(steps) + ", and not " + boost::lexical_cast<std::string>(length)).add(OKLIB_TESTDESCRIPTION);
        }
        {
          // ToDo: To be completed.
        }
      }
    };

    template <template <typename Integer> class Arithmetic_progression>
    class Test_Arithmetic_progression_Integer : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Arithmetic_progression_Integer test_type;
      Test_Arithmetic_progression_Integer() {
        insert(this);
      }
    public :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_Arithmetic_progression<Arithmetic_progression<unsigned int> >());
        OKLIB_TESTTRIVIAL_RETHROW(Test_Arithmetic_progression<Arithmetic_progression<int> >());
        OKLIB_TESTTRIVIAL_RETHROW(Test_Arithmetic_progression<Arithmetic_progression<long unsigned int> >());
        OKLIB_TESTTRIVIAL_RETHROW(Test_Arithmetic_progression<Arithmetic_progression<long int> >());
      }
    };

    // ------------------------------------------------------------------------------------------------------------------------

    }
}

#endif
