//TO DO:  LICENCE

#ifndef IOTOOLSMODELSDEFINITIONS_jdjbbTy7728Jj

#define IOTOOLSMODELSDEFINITIONS_jdjbbTy7728Jj

#include <functional>
#include <ios>
#include <new>

namespace IOTools {

  // TO DO: Move to a different module (isn't there a standard implementation?!)
  struct string_compare : std::binary_function<const char*, const char*, bool> {
    bool operator() (const char* const a, const char* const b) {
      return std::strcmp(a,b) < 0;
    }
  };

  // From "Standard C++ IOStreams and Locales", Section 3.1.5
  // TO DO: Checking the logic (and putting it into the documentation)
  template <typename charT, class Traits, class Argument>
  std::basic_ostream<charT, Traits>& generic_inserter (
       std::basic_ostream<charT, Traits>& os,
       const Argument& arg) {
    typedef typename std::basic_ostream<charT, Traits> ostream;
    std::ios_base::iostate error(std::ios_base::goodbit);
    try {
      typename ostream::sentry sentry_object(os);
      if (sentry_object) {
	error = arg.print(os);
	os.width(0);
      }
    }
    // now error != 0 iff (width threw and print_on != 0)
    catch(const std::bad_alloc&) { // setting badbit
      error |= std::ios_base::badbit;
      const std::ios_base::iostate exception_mask = os.exceptions(); // possibly throws ?!
      const bool throw_if_failbit = exception_mask & std::ios_base::failbit;
      const bool throw_if_badbit = exception_mask & std::ios_base::badbit;
      if (throw_if_failbit and not throw_if_badbit)
	os.setstate(error); // possibly throws std::ios_base::failure
      else if (throw_if_badbit) {
	try { os.setstate(error); } // throws std::ios_base::failure
	catch(const std::ios_base::failure&) {}
	throw;
      }
    }
    catch(...) { // setting failbit
      error |= std::ios_base::failbit;
      const bool badbit_in_error = error & std::ios_base::badbit;
      const std::ios_base::iostate exception_mask = os.exceptions(); // possibly throws ?!
      const bool throw_if_failbit = exception_mask & std::ios_base::failbit;
      const bool throw_if_badbit = exception_mask & std::ios_base::badbit;
      if (throw_if_badbit and badbit_in_error)
	os.setstate(error); // throws std::ios_base::failure; why not rethrow?!
      else if (throw_if_failbit) {
	try { os.setstate(error); } // throws std::ios_base::failure
	catch(const std::ios_base::failure&) {}
	throw;
      }
    }
    if (error) os.setstate(error); // possibly throws std::ios_base::failure
    return os;
  }


}

#endif
