// Oliver Kullmann, 20.4.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/Tools.hpp
  \brief Various tools
*/

#ifndef TOOLSWAECHTER

#include <string>
#include <sstream>
#include <cstdlib> // for system
#include <cstdio> // for tmpnam and remove
#include <fstream>
#include <sstream>
#include <utility>
#include <map>

namespace Tools {

  using std::string;

  namespace Error {

    class General {
    public :
      General(const string& s) : w(s) {}
      virtual string what() const throw() { return w; }
    private :
      string w;
    };

    struct timestamp : General {
      timestamp(const string& s) : General(s) {}
    };
    struct missing : timestamp {
      missing(const string& s) : timestamp(s) {}
    };
    struct bad_value : timestamp {
      bad_value(const string& s) : timestamp(s) {}
    };
    struct unknown : timestamp {
      unknown(const string& s) : timestamp(s) {}
    };

    struct bad_bounds : General {
      bad_bounds(const string& s) : General(s) {}
    };

    struct system : General {
      system(const string& s) : General(s) {}
    };
    struct create : system {
      create(const string& s) : system(s) {}
    };
    struct open : system {
      open(const string& s) : system(s) {}
    };
    struct execute : system {
      execute(const string& s) : system(s) {}
    };

    struct conversion_to_string : General {
      conversion_to_string() : General("") {}
    };
    struct conversion_from_string : General {
      conversion_from_string(const string& s) : General(s) {}
    };
  }


  // Function objects -----------------------------------------------


  struct DeleteObject {
    template <typename T> void operator() (const T* ptr) const {
      delete ptr;
    }
  };
  struct DeleteObjectSecond {
    template <typename U, typename T> void operator() (const std::pair<U, T*>& p) const {
      delete p.second;
    }
  };
  

  // Iterator construction --------------------------------------------


  template<typename T, typename U> struct map_constIteratorFirst : std::map<T, U>::const_iterator {
    typedef typename std::map<T, U>::iterator map_iterator;
    typedef typename std::map<T, U>::const_iterator const_map_iterator;
    map_constIteratorFirst(map_iterator i) : const_map_iterator(i) {}
    T operator * () const { return static_cast<const_map_iterator>(*this) -> first; }
  };

  template<typename T, typename U> struct map_constIteratorSecond : std::map<T, U>::const_iterator {
    typedef typename std::map<T, U>::iterator map_iterator;
    typedef typename std::map<T, U>::const_iterator const_map_iterator;
    map_constIteratorSecond(map_iterator i) : const_map_iterator(i) {}
    U operator * () const { return static_cast<const_map_iterator>(*this) -> second; }
  };


  // Conversions -----------------------------------------------------

  using std::ostringstream;
  using std::istringstream;

  template<typename T> inline string toString(const T& x) {
    ostringstream s;
    s << x;
    if (! s)
      throw Error::conversion_to_string();
    return s.str();
  }

  template<typename T> inline void fromString(const string& in, T& out) {
    istringstream s(in);
    s >> out;
    if (! s)
      throw Error::conversion_from_string(in);
  }
    

  // Numbers ---------------------------------------------------------

  const int pow_2_31m1 = 2147483647;
  const int mpow_2_31 = -2147483648U;
  const unsigned int pow_2_32m1 = 4294967295U;

  inline int uint4_to_int4(unsigned int x) {
    // only apply for x <= 2^32 - 1;
    // if x < 2^31 then return x
    // else return x - 2^32;
    if (x <= pow_2_31m1)
      return x;
    else
      return mpow_2_31 + int(x - pow_2_31m1) - 1;
  }
  inline unsigned int int4_to_uint4(int x) {
    // only apply for - 2^31 <= x <= 2^31 - 1
    // if x >= 0 then return x
    // else return x + 2^32;
    if (x >= 0)
      return x;
    else
      return (unsigned int)(x + pow_2_32m1) + 1;
  }

  inline unsigned int divmod10(unsigned int& r, const unsigned int b) {
    // r = (10 * r) % b;
    // result = (10 * r) / b;
    double r10 = 10 * double(r);
    unsigned int q = (unsigned int)(r10 / double(b));
    r = (unsigned int)(r10 - q * b);
    return q;
  }

  inline string standardised_quotient(const unsigned int nom, const unsigned int denom) {
    // only apply for nom, denom < 2^32;
    ostringstream s;
    s << nom / denom;
    unsigned int r = nom % denom;
    if (r == 0)
      return s.str();
    s << ".";
    double e = 1;
    // loop invariant : r/e + s * denom =  nom
    // where s is interpreted as decimal number;
    for (;;) {
      const unsigned int digit = divmod10(r, denom);
      e *= 10;
      // loop invariant reestablished
      if (2 * double(r) <= e) { // r/e <= 1/2 (rounding up)
	s << digit;
	break;
      }
      else if (2 * (double(denom) - double(r)) < e) { // denom/e - r/e < 1/2 (rounding down)
	s << digit + 1;
	break;
      }
      else
	s << digit;
    }
    return s.str();
  }

  // Date and time --------------------------------------------------------

  class Timestamp {

  public :

    Timestamp() throw() {}

    enum date_type {sec, min, h, d, m, y};

    void set_field(const date_type field, const unsigned int value) throw(Error::bad_value) {
      ostringstream s; s << value;
      switch (field) {
      case sec :
	if (value >= 60)
	  throw Error::bad_value("second = " + s.str());
	else
	  minute = s.str(); break;
      case min :
	if (value >= 60)
	  throw Error::bad_value("minute = " + s.str());
	else
	  minute = s.str(); break;
      case h :
	if (value > 24)
	  throw Error::bad_value("hour = " + s.str());
	else
	  hour = s.str(); break;
      case d :
	if (value == 0 || value > 31)
	  throw Error::bad_value("day = " + s.str());
	else
	  day = s.str(); break;
      case m :
	if (value == 0 || value > 12)
	  throw Error::bad_value("month = " + s.str());
	else
	  month = s.str(); break;
      case y :
	if (value == 0)
	  throw Error::bad_value("year = 0");
	else
	  year = s.str(); break;
       }
    }

    date_type interprete(const string& s) const throw(Error::unknown) {
      if (s == "second")
	return sec;
      if (s == "minute")
	return min;
      if (s == "hour")
	return h;
      if (s == "day")
	return d;
      if (s == "month")
	return m;
      if (s == "year")
	return y;
      throw Error::unknown(s);
    }
    
    void set_field(const string& field, const unsigned int value) throw(Error::bad_value, Error::unknown) {
      set_field(interprete(field), value);
    }

    void set_time_zone(const string& tz) throw() {
      time_zone = tz;
    }
    
    operator string() const throw(Error::missing) {
      ostringstream result;
      if (day.empty())
	throw Error::missing("day");
      result << day << ".";
      if (month.empty())
	throw Error::missing("month");
      result << month << ".";
      if (year.empty())
	throw Error::missing("year");
      result << year << " ";
      if (hour.empty())
	throw Error::missing("hour");
      result << hour << ":";
      if (minute.empty())
	throw Error::missing("minute");
      result << minute;
      if (! second.empty())
	result << ":" << second;
      if (! time_zone.empty())
	result << " " << time_zone;
      return result.str();
    }

    void clear() throw() {
      second.clear(); minute.clear(); hour.clear(); day.clear(); month.clear(); year.clear(); time_zone.clear();
    }
      
  private :

    string second, minute, hour, day, month, year;
    string time_zone;

  };

  // Bounds ----------------------------------------------------------

  template<typename T> class bounds {

    // invariante : lb <= ex <= up, if set

  public :

    enum specifier {lower, upper, exact};

    bounds<T>() throw() : sub(false), slb(false), sex(false) {}

    void set_ub(const T x) throw(Error::bad_bounds) {
      ostringstream s;
      if (sex and ex > x) {
	s << x << " < " << ex;
	throw Error::bad_bounds(s.str());
      }
      else if (slb and lb > x) {
	s << x << " < " << lb;
	throw Error::bad_bounds(s.str());
      }
      else if (! sub or x < ub) {
	sub = true;
	ub = x;
      }
    }

    void set_lb(const T x) throw(Error::bad_bounds) {
      ostringstream s;
      if (sex and ex < x) {
	s << x << " > " << ex;
	throw Error::bad_bounds(s.str());
      }
      else if (sub and ub < x) {
	s << x << " > " << ub;
	throw Error::bad_bounds(s.str());
      }
      else if (! slb or x > lb) {
	slb = true;
	lb = x;
      }
    }

    void set_ex(const T x) throw(Error::bad_bounds) {
      ostringstream s;
      if (sub and ub < x) {
	s << x << " > " << ub;
	throw Error::bad_bounds(s.str());
      }
      else if (slb and lb > x) {
	s << x << " < " << lb;
	throw Error::bad_bounds(s.str());
      }
      else if (!sex) {
	sex = true;
	ex = x;
	}
      else if (ex != x) {
	s << x << " <> " << ex;
	throw Error::bad_bounds(s.str());
      }
    }

    void set(const specifier s, const T x) throw(Error::bad_bounds) {
      switch (s) {
      case upper : set_ub(x); break;
      case lower : set_lb(x); break;
      case exact : set_ex(x); break;
      }
    }

    void clear() throw() {
      sub = slb = sex = false;
    }
	
  private :

    T ub, lb, ex; // upper bound, lower bound, exact value
    bool sub, slb, sex; // whether they have been set
  };


  // Filenames --------------------------------------------------------

  string core_name(string s) {
    // core_name("/dir1/dir2/file1.ext") = "file1"
    // only for Unix/Linux !!
    string::size_type i;
    if ((i = s.rfind('/')) != string::npos)
      s.erase(0, i + 1);
    if ((i = s.find('.')) != string::npos)
      s.erase(i);
    return s;
  }


  // System calls ------------------------------------------------------

  class System {
    // ToDo: Combine with the system call in library General.

  public :

    System() // using a temporary file named by the system
      : tempName_(std::tmpnam(0)) {
      if (not tempName_)
        throw Error::create("");
      tempName.assign(tempName_);
      std::ofstream file(tempName.c_str());
      if (! file)
        throw Error::create(tempName);
    }

    System(const string& name) // using a temporary file named name
      : tempName(name) {
      std::ofstream file(tempName.c_str());
      if (! file)
        throw Error::create(tempName);
    }

    string operator () (const string& command) {
      // returns the whole output
      const string& full_command = command + " > " + tempName;
      if (std::system(full_command.c_str()) != 0)
        throw Error::execute(full_command);
      std::ifstream file(tempName.c_str());
      if (! file)
        throw Error::open(tempName);
      std::ostringstream s;
      s << file.rdbuf();
      return s.str();
    }

    ~System() {
      std::remove(tempName.c_str());
    }

  private :

    const char* const tempName_;
    string tempName;

    System(System&) {} // not allowed
    System& operator = (System&) {} // not allowed

  };



}

#endif

#define TOOLSWAECHTER

