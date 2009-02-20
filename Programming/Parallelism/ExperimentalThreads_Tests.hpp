// Oliver Kullmann, 15.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Parallelism/ExperimentalThreads_Tests.hpp
  \brief Test metafunctions
  \deprecated Move to new test system.

  \todo By moving to the new test system it must become possible
  to find out exactly when the assertion in the failing test happens.

  \todo Describe what the tests test.
*/

#ifndef EXPERIMENTALTHREADSTESTS_908NbCxz
#define EXPERIMENTALTHREADSTESTS_908NbCxz

#include <boost/thread.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace Parallelism {

#ifndef BOOST_HAS_THREADS
# error "No threads!"
#endif

    template <template <int N> class Counter>
    class Test_Counter : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Counter test_type;
      Test_Counter() {
        insert(this);
      }
    private :
      
      void perform_test_trivial() {

        { // running independent counters sequentially (???)
          typedef Counter<0> counter0;
          typedef typename counter0::uint_type uint_type_0;
          const uint_type_0 count0 = 10000000U;
          {
            const counter0 c0(count0);
            OKLIB_TEST_EQUAL(counter0::counter(), count0);
            c0();
            OKLIB_TEST_EQUAL(counter0::counter(), 0);
          }
          const counter0 c0(count0);
          OKLIB_TEST_EQUAL(counter0::counter(), count0);
          
          typedef Counter<1> counter1;
          typedef typename counter1::uint_type uint_type_1;
          const uint_type_1 count1 = 10000001U;
          const counter1 c1(count1);
          OKLIB_TEST_EQUAL(counter1::counter(), count1);

          //independent counter c0, c1 initialised
          boost::thread thread0(c0);
          boost::thread thread1(c1);
          // now both counters are counting down in parallel threads
          OKLIB_TEST_NOTEQUAL(thread0, thread1);
          {
            boost::thread thread;
            OKLIB_TEST_NOTEQUAL(thread0, thread);
            OKLIB_TEST_NOTEQUAL(thread1, thread);
          }
          
          thread0.join();
          thread1.join();
          
          OKLIB_TEST_EQUAL(counter0::counter(), 0);
          OKLIB_TEST_EQUAL(counter1::counter(), 0);

        }

        {
          boost::mutex mutex;

          typedef CounterWithMutex<0> counter0;
          typedef typename counter0::uint_type uint_type_0;
          const uint_type_0 count0 = 10000000U;
          const counter0 c0(count0, mutex);
          OKLIB_TEST_EQUAL(counter0::counter(), count0);
          
          typedef CounterWithMutex<1> counter1;
          typedef typename counter1::uint_type uint_type_1;
          const uint_type_1 count1 = 10000001U;
          const counter1 c1(count1, mutex);
          OKLIB_TEST_EQUAL(counter1::counter(), count1);

          boost::thread thread0(c0);
          boost::thread thread1(c1);
          OKLIB_TEST_NOTEQUAL(thread0, thread1);

          {
            boost::mutex::scoped_lock lock(mutex);
          }
          OKLIB_TEST_EQUAL(counter0::counter(), 0);
          OKLIB_TEST_EQUAL(counter1::counter(), 0);
        }

        { // NOW parallelism
          boost::mutex mutex0;
          typedef CounterWithMutex<0> counter0;
          typedef typename counter0::uint_type uint_type_0;
          const uint_type_0 count0 = 10000000U;
          const counter0 c0(count0, mutex0);
          
          boost::mutex mutex1;
          typedef CounterWithMutex<1> counter1;
          typedef typename counter1::uint_type uint_type_1;
          const uint_type_1 count1 = 10000001U;
          const counter1 c1(count1, mutex1);
          
          {
            boost::thread thread0(c0);
            boost::thread thread1(c1);
            boost::mutex::scoped_lock lock0(mutex0);
            boost::mutex::scoped_lock lock1(mutex1);
          }
          OKLIB_TEST_EQUAL(counter0::counter(), 0);
          OKLIB_TEST_EQUAL(counter1::counter(), 0);
        }
      }
      
    };
    
  }

}

#endif
