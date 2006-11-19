// Oliver Kullmann, 15.6.2005 (Swansea)

/*! \file Parallelism/ExperimentalThreads_Tests.hpp
  Test metafunctions
*/

#ifndef EXPERIMENTALTHREADSTESTS_908NbCxz
#define EXPERIMENTALTHREADSTESTS_908NbCxz

#include <boost/thread.hpp>

#include <Transitional/TestSystem/TestBaseClass.hpp>
#include <Transitional/TestSystem/TestExceptions.hpp>

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

        {
          typedef Counter<0> counter0;
          typedef typename counter0::integer_type integer_type_0;
          const integer_type_0 count0 = 10000000U;
          {
            const counter0 c0(count0);
            OKLIB_TEST_EQUAL(counter0::counter, count0);
            c0();
            OKLIB_TEST_EQUAL(counter0::counter, 0);
          }
          const counter0 c0(count0);
          OKLIB_TEST_EQUAL(counter0::counter, count0);
          
          typedef Counter<1> counter1;
          typedef typename counter1::integer_type integer_type_1;
          const integer_type_1 count1 = 10000001U;
          const counter1 c1(count1);
          OKLIB_TEST_EQUAL(counter1::counter, count1);
          
          boost::thread thread0(c0);
          boost::thread thread1(c1);
          OKLIB_TEST_NOTEQUAL(thread0, thread1);
          {
            boost::thread thread;
            OKLIB_TEST_NOTEQUAL(thread0, thread);
            OKLIB_TEST_NOTEQUAL(thread1, thread);
          }
          
          thread0.join();
          thread1.join();
          
          OKLIB_TEST_EQUAL(counter0::counter, 0);
          OKLIB_TEST_EQUAL(counter1::counter, 0);

        }

        {
          boost::mutex mutex;

          typedef CounterWithMutex<0> counter0;
          typedef typename counter0::integer_type integer_type_0;
          const integer_type_0 count0 = 10000000U;
          const counter0 c0(count0, mutex);
          OKLIB_TEST_EQUAL(counter0::counter, count0);
          
          typedef CounterWithMutex<1> counter1;
          typedef typename counter1::integer_type integer_type_1;
          const integer_type_1 count1 = 10000001U;
          const counter1 c1(count1, mutex);
          OKLIB_TEST_EQUAL(counter1::counter, count1);

          boost::thread thread0(c0);
          boost::thread thread1(c1);
          OKLIB_TEST_NOTEQUAL(thread0, thread1);

          {
            boost::mutex::scoped_lock lock(mutex);
          }
          OKLIB_TEST_EQUAL(counter0::counter, 0);
          OKLIB_TEST_EQUAL(counter1::counter, 0);
        }

        { // NOW parallelism
          boost::mutex mutex0;
          typedef CounterWithMutex<0> counter0;
          typedef typename counter0::integer_type integer_type_0;
          const integer_type_0 count0 = 10000000U;
          const counter0 c0(count0, mutex0);
          
          boost::mutex mutex1;
          typedef CounterWithMutex<1> counter1;
          typedef typename counter1::integer_type integer_type_1;
          const integer_type_1 count1 = 10000001U;
          const counter1 c1(count1, mutex1);
          
          {
            boost::thread thread0(c0);
            boost::thread thread1(c1);
            boost::mutex::scoped_lock lock0(mutex0);
            boost::mutex::scoped_lock lock1(mutex1);
          }
          OKLIB_TEST_EQUAL(counter0::counter, 0);
          OKLIB_TEST_EQUAL(counter1::counter, 0);
        }
      }
      
    };
    
  }

}

#endif
