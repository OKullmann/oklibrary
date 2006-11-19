// Oliver Kullmann, 15.6.2005 (Swansea)

/*!
  \file Parallelism/ExperimentalThreads.hpp
  \brief Experiments using the Boost::Threads library
*/

#ifndef EXPERIMENTALTHREADS_8u76gBn
#define EXPERIMENTALTHREADS_8u76gBn

#include <cassert>
#include <memory>

#include <boost/thread.hpp>

namespace OKlib {

  namespace Parallelism {

    template <int N>
    struct Counter {
      typedef unsigned int integer_type;
      static integer_type counter;
      static const integer_type multiplier = 100;
      Counter() {}
      Counter(const integer_type n) {
        counter = n;
      }
      Counter(const Counter&) {}
      ~Counter() {}
      void operator() () const {
        for (; counter != 0; --counter)
          for (integer_type i = 0; i != multiplier; ++i)
            i = i;
      }
    };
    template <int N>
    typename Counter<N>::integer_type Counter<N>::counter;

    template <int N>
    struct CounterWithMutex : Counter<N> {

      mutable boost::mutex& mutex;
      const bool locked;

      typedef std::auto_ptr<boost::mutex::scoped_lock> auto_ptr;
      mutable auto_ptr lock;

      typedef typename Counter<N>::integer_type integer_type;

      CounterWithMutex(const integer_type n, boost::mutex& m) : Counter<N>(n), mutex(m) , locked(false) {}
      CounterWithMutex(const CounterWithMutex& c) :  mutex(c.mutex), locked(true), lock((c.locked) ? c.lock : auto_ptr(new boost::mutex::scoped_lock(mutex))) {
        assert(lock -> locked());
      }
      ~CounterWithMutex() {}
      
      void operator() () const {
        assert(lock -> locked());
        Counter<N>::operator()();
        lock -> unlock();
      }
    };

  }

}

#endif
