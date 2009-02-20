// Oliver Kullmann, 29.5.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file StringHandling.hpp
  \brief Various tools for handling strings.
  \todo How much should be replaced by Boost components?!
*/

#ifndef STRINGHANDLINGWAECHTER

#define STRINGHANDLINGWAECHTER

#include <string>
#include <sstream>
#include <functional>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <limits>
#include <cassert>

#include <OKlib/General/ErrorHandling.hpp>

namespace StringHandling {

  struct Error : ErrorHandling::Error {
    Error(const std::string& what) : ErrorHandling::Error(what) {}
  };

}

// -----------------------------
// C-string
// -----------------------------

namespace StringHandling {

  inline char *copy_new(const std::string s) {
    // returned string must be deleted with "delete []"
    const std::string::size_type l = s.size();
    char* p = new char[l+1];
    s.copy(p, l);
    p[l] = 0;
    return p;
  }

  class Vector_c_str {
    // members must have been generated with new []
  public :
    typedef std::vector<char *> vec_char;
    vec_char v;
    Vector_c_str() {}
    ~ Vector_c_str() {
      for (vec_char::iterator i = v.begin(); i != v.end(); ++i)
	delete[] *i;
    }
    void push_back(const std::string& s) {
      v.push_back(copy_new(s));
    }
    void clear() {
      for (vec_char::iterator i = v.begin(); i != v.end(); ++i)
	delete[] *i;
      v.clear();
    }
      
  private :
    Vector_c_str(const Vector_c_str&);
    Vector_c_str& operator = (const Vector_c_str&);
  };

}
// -----------------------------
// Conversions
// -----------------------------

namespace StringHandling {

  namespace Error_StringHandling {
    struct conversion_to_string : Error {
      conversion_to_string(const std::string& what) : Error(what) {}
    };
    struct conversion_from_string : Error {
      conversion_from_string(const std::string& what) : Error(what) {}
    };
  }

  template<typename T> inline std::string toString(const T& x) {
    // throw(Error_StringHandling::conversion_to_string(), std::ios_base::failure)
    std::ostringstream s;
    s << x;
    if (! s)
      throw Error_StringHandling::conversion_to_string(std::string("StringHandling::toString : Can not convert value of type ") + typeid(T).name() + " to string");
    return s.str();
  }
  inline std::string toString(unsigned char x) {
    // throw(Error_StringHandling::conversion_to_string(), std::ios_base::failure)
    std::ostringstream s;
    s << int(x);
    if (! s)
      throw Error_StringHandling::conversion_to_string(std::string("StringHandling::toString : Can not convert value of type ") + typeid(char).name() + " to string");
    return s.str();
  }
  inline std::string toString(signed char x) {
    // throw(Error_StringHandling::conversion_to_string(), std::ios_base::failure)
    std::ostringstream s;
    s << int(x);
    if (! s)
      throw Error_StringHandling::conversion_to_string(std::string("StringHandling::toString : Can not convert value of type ") + typeid(char).name() + " to string");
    return s.str();
  }
  template<typename T> inline std::string toString_nc(const T& x) {
    // "nc" : not checked
    // throw(std::ios_base::failure)
    std::ostringstream s;
    s << x;
    return s.str();
  }
  inline std::string toString_nc(unsigned char x) {
    // "nc" : not checked
    // throw(std::ios_base::failure)
    std::ostringstream s;
    s << int(x);
    return s.str();
  }
  inline std::string toString_nc(signed char x) {
    // "nc" : not checked
    // throw(std::ios_base::failure)
    std::ostringstream s;
    s << int(x);
    return s.str();
  }

  
  

  template<typename T> inline T fromString(const std::string& st) {
    // throw(Error_StringHandling::conversion_from_string(), std::ios_base::failure)
    std::istringstream s(st);
    T x;
    s >> x;
    if (! s)
      throw Error_StringHandling::conversion_from_string(std::string("StringHandling::fromString : Can not convert string \"") + st + "\" into value of type " + typeid(T).name());
    return x;
  }
  template<typename T> inline T fromString_nc(const std::string& st) {
    // throw(std::ios_base::failure)
    std::istringstream s(st);
    T x;
    s >> x;
    return x;
  }


}

// -----------------------------
// Classifying Characters
// -----------------------------

struct isSpace : std::unary_function<char, bool> {
  bool operator() (const char c) const {
    return std::isspace(c);
    // uses the current global C locale
  }
};


// -----------------------------
// Erasing Characters
// -----------------------------

namespace StringHandling {

  inline std::string deleteCharacter(std::string s, const char c) {
    // throw()
    s.erase(remove(s.begin(), s.end(), c), s.end());
    return s;
  }

  inline void deleteCharacter_var(std::string& s, const char c) {
    // throw()
    s.erase(remove(s.begin(), s.end(), c), s.end());
  }

  inline std::string deleteSpaces(std::string s) {
    // throw()
    s.erase(remove_if(s.begin(), s.end(), isSpace()), s.end());
    return s;
  }

  inline void deleteSpaces_var(std::string& s) {
    // throw()
    s.erase(remove_if(s.begin(), s.end(), isSpace()), s.end());
  }

  inline std::string deleteHead(const std::string& s, const int n) {
    // throw()
    assert(n != std::numeric_limits<int>::min()); // TODO Write a better abs-function, which for input int returns unsigned int, so that this asserts becomes superfluous.
    if (n == 0)
      return s;
    else if ((unsigned int) std::abs(n) >= s.size())
      return "";
    else if (n > 0)
      return s.substr(n);
    else
      return s.substr(0, s.size() + n);
  }

  inline void deleteHead_var(std::string& s, const int n) {
    // throw()
    assert(n != std::numeric_limits<int>::min()); // TODO see above for abs-function
    if ((unsigned int) std::abs(n) >= s.size()) {
      s.clear();
    }
    else if (n > 0)
      s.erase(0, n);
    else
      s.erase(s.size() + n);
  }

// -----------------------------
// Replacing substrings
// -----------------------------

  // The following three functions replace the first occurrence of the sub-string to_be_replaced of the string original with the string substitute.
  // The differ how they react in case the sub-string is in fact not a sub-string.
  // In any case, the string original is not changed if the sub-string is in fact not a sub-string.
  // All throws std::length_error if the resulting string is too big.

  inline void replace_substring_throw(std::string& original, const std::string& to_be_replaced, const std::string& substitute) {
    // throws std::out_of_range in case to_be_replaced is not a substring of original
    // throws std::length_error if resulting string is too big
    // to catch these exceptions, include <stdexcept>;
    // both exception classes are derived from std::logic_error,
    // which again is derived from std::exception (include <exception> for the latter)
    original.replace(original.find(to_be_replaced), to_be_replaced.size(), substitute);
  }

  inline bool replace_substring_check(std::string& original, const std::string& to_be_replaced, const std::string& substitute) {
    // throws std::length_error if resulting string is too big
    const std::string::size_type pos = original.find(to_be_replaced);
    if (pos == std::string::npos)
      return false;
    else {
      original.replace(pos, to_be_replaced.size(), substitute);
      return true;
    }
  }

  inline void replace_substring_if_possible(std::string& original, const std::string& to_be_replaced, const std::string& substitute) {
    // throws std::length_error if resulting string is too big
    const std::string::size_type pos = original.find(to_be_replaced);
    if (pos == std::string::npos)
      return;
    else {
      original.replace(pos, to_be_replaced.size(), substitute);
      return;
    }
  }

  // The following function replaces all occurrences of the string
  // "to_be_replaced" by "substitute" (simultaneously).

  inline void replace_all_substrings(std::string& original, const std::string& to_be_replaced, const std::string& substitute) {
    // throws std::length_error if resulting string is too big
    const std::string::size_type size_substitute = substitute.size();
    const std::string::size_type size_replaced = to_be_replaced.size();
    for (std::string::size_type pos = 0; (pos = original.find(to_be_replaced, pos)) != std::string::npos; pos += size_substitute)
      original.replace(pos, size_replaced, substitute);
  }
  
}


#endif
