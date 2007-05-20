// Oliver Kullmann, 15.6.2005 (Swansea)

/*!
  \file Parallelism/ExperimentalThreads.hpp
  \brief Experiments using the Boost::Threads library

  \bug Parallelism::CounterWithMutex fails the test: Apparently an inconsistent copy-object is
  being used (so that CounterWithMutex::check asserts).
*/

#ifndef EXPERIMENTALTHREADS_8u76gBn
#define EXPERIMENTALTHREADS_8u76gBn

#include <cassert>
#include <memory>

#include <boost/thread.hpp>

namespace OKlib {
  namespace Parallelism {

    /*!
      \class Counter
      \brief Functor class, counting a counter down, to simulate some operation

      Contains a static unsigned integer "counter", which is initialised by Counter(n), while
      Counter() does not change the current value. For an object c of type Counter the call c()
      runs through counter*multiplier many simple assignments. The purpose of the template parameter
      is to create independent classes.
    */

    template <int N>
    class Counter {
    public :
      typedef unsigned int uint_type;
      
    public:
      Counter() {}
      explicit Counter(const uint_type n) { counter_ = n; }

      static uint_type counter() { return counter_; }
      void operator() () const {
        for (; counter_ != 0; --counter_)
          for (uint_type i = 0; i != multiplier; ++i)
            i = i;
        assert(counter_ == 0);
      }
    private :
      static uint_type counter_;
      static const uint_type multiplier = 100;
    };
    template <int N>
    typename Counter<N>::uint_type Counter<N>::counter_;

    /*!
      \class CounterWithMutex
      \brief Publically derived from Counter, provides mutex-functionality

      Only for objects created by the copy-constructor the pointer lock is valid.
      Invariant: If locked is true, then lock.get() is valid.

      \bug The problem likely is that after a copy-operation the copied object becomes invalid!

      \todo Document the class (what's the point of the strange design?).
      \tod Can assertions be made about member mutex?
    */

    template <int N>
    class CounterWithMutex : public Counter<N> {

      mutable boost::mutex& mutex;
      const bool locked;
      typedef std::auto_ptr<boost::mutex::scoped_lock> ptr_lock;
      mutable ptr_lock lock;

      typedef typename Counter<N>::uint_type uint_type;

    public :

      CounterWithMutex(const uint_type n, boost::mutex& m) : Counter<N>(n), mutex(m) , locked(false) {
        assert(not locked);
        assert(not lock.get());
      }
      CounterWithMutex(const CounterWithMutex& c) :  mutex(c.mutex), locked(true), lock((check(c), c.locked) ? c.lock : ptr_lock(new boost::mutex::scoped_lock(mutex))) {
        assert(not c.lock.get());
        assert(locked);
        assert(lock.get());
        assert(lock -> locked());
      }
      
      void operator() () const {
        assert(locked);
        assert(lock.get());
        assert(lock -> locked());

        Counter<N>::operator()();

        assert(locked);
        assert(lock.get());
        assert(lock -> locked());

        lock -> unlock();

        assert(locked);
        assert(lock.get());
        assert(not (lock -> locked()));
      }

    private :

      void check(const CounterWithMutex& c) const {
        if (c.locked)
          assert(c.lock.get());
      }

    };

  }

}

#endif
