// Oliver Kullmann, 2.2.2006 (Swansea)

/*!
  \file Transitional/Utilities/plans/Utilities.hpp
  \brief Plans for the utilities module

  \todo First submodule Timestamp.hpp:
  #include <stdint.h> // later to be replaced by <cstdint>
  template <typename UInt = uint_fast64_t, class OverflowPolicy = NoCheck> // later std::uint_fast64_t
  class Timestamp {
    Uint counter;
  public :
    typedef UInt size_type;
    typedef OverflowPolicy<Uint> overflow_policy_type;  
  public :
    bool null() const { return counter == 0; }
    void reset() { counter = 0; }
    Timestamp& operator ++() {
      overflow_policy_type::check(++counter);
    }
    Timestamp& operator ++(int) {
      Timestamp tmp(*this);
      operator ++();
      return tmp;
    }
  ;
  operators <, ==, and the other four;
  struct NoCheck {
    template <typename UInt>
    static void check(const UInt) {}
  };
  struct ThrowPolicy {
  template <typename UInt>
    static void check(const UInt after_increment) {
      if (after_increment == 0)
        throw suitable exception;
    }
  }
*/

/*!
  \namespace OKlib::Utilities
  \brief Various utilities
*/

namespace OKlib {
  namespace Utilities {
  }
}  

