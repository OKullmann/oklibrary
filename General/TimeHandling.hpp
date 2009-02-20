// Oliver Kullmann, 19.6.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TimeHandling.hpp
  \brief Tools for handling times and dates
  \todo Functions currentDateTime should go to module Messages.
  \todo Which components should be replaced by components from Boost ?!
*/

#ifndef TIMEHANDLINGWAECHTER

#define TIMEHANDLINGWAECHTER

#include <ctime>
#include <ostream>
#include <sstream>
#include <locale>

#include <OKlib/General/StringHandling.hpp>

namespace TimeHandling {

  /*!
    \class SystemTime
    \brief DEPRECATED (replaced by boost::timer): For elapsed system
    time in seconds since initialisation.

    Only for ca. 30 minutes, but with high precision.
    \todo It is actually not clear whether boost::timer is specified to be
    the *system* time?! Send an e-mail to the Boost mailing list.
  */

  class SystemTime {
  public :

    SystemTime();
    SystemTime(bool set);
    operator double() const;
    void reset();

  private :

    std::clock_t time_point;
  };

  /*!
    \class WallTime
    \brief For elapsed wall time in (full) seconds since initialisation.
  */

  class WallTime {
  public :

    WallTime();
    WallTime(bool set);
    operator double() const;
    void reset();

  private :

    std::time_t time_point;
  };

}

namespace TimeHandling {

  /*!
    \brief Returns a string with the current date and time (in the format given by the std::asctime
    from <ctime> (see C Standard 7.23.3.1)).
  */
  std::string currentDateTime();

  /*!
    \brief As currentDateTime(), but using the format string as for the std::strftime
    function from <ctime> (see C Standard 7.23.3.5), and either admitting a std::ostream
    as argument, or returning a string and admitting a locale object as optional argument
    (with the system-default locale as default).
  */
  std::ostream& currentDateTime(const std::string& format, std::ostream& out);
  std::string currentDateTime(const std::string& format, const std::locale& loc = std::locale(""));

  /*!
    \brief Returns the number s of seconds represented as seconds (s), minutes (m), hours (h), days (d) or years (y)
  */
  std::string output_seconds(double s);
 
}

namespace TimeHandling {

  class Progress {

  public :

    Progress(std::ostream& output_stream, unsigned int total_number, unsigned int skip, const std::string& item_text, const std::string& remaining_time_text);

    void operator() ();

  private :

    std::ostream& output_stream;
    const unsigned int total_number, skip;
    const std::string item_text, remaining_time_text;
    
    unsigned int counter, new_goal;
    const WallTime time_used;
    std::string::size_type char_printed;
  };

}


namespace TimeHandling { // Implementations ------------------------------

  // System time

  inline SystemTime::SystemTime()
    : time_point(std::clock()) {}
  inline SystemTime::SystemTime(const bool set) {
    if (set)
      time_point = std::clock();
  }

  inline SystemTime::operator double() const {
    return double(std::clock() - time_point) / double(CLOCKS_PER_SEC);
  }

  inline void SystemTime::reset() {
    time_point = std::clock();
  }

  // Wall time

  inline WallTime::WallTime()
    : time_point(std::time(0)) {}
  inline WallTime::WallTime(const bool set) {
    if (set)
      time_point = std::time(0);
  }

  inline WallTime::operator double() const {
    return std::difftime(std::time(0), time_point);
  }

  inline void WallTime::reset() {
    time_point = std::time(0);
  }

  // Current date and time

  inline std::string currentDateTime() {
    const std::time_t t0 = std::time(0);
    return std::asctime(std::localtime(&t0));
  }

  inline std::ostream& currentDateTime(const std::string& format, std::ostream& out) {
    const std::time_t t0 = std::time(0);
    const std::tm* const tmp = std::localtime(&t0);
    const std::time_put<char>& tfac = std::use_facet<std::time_put<char> >(out.getloc());
    std::time_put<char>::iter_type ret = tfac.put(out, out, ' ', tmp, format.c_str(), format.c_str() + format.length());
    return out;
  }

  inline std::string currentDateTime(const std::string& format, const std::locale& loc) {
    std::ostringstream oss;
    oss.imbue(loc);
    currentDateTime(format, oss);
    return oss.str();
  }

  // Readable output of seconds

  inline std::string output_seconds(double s) {
    // assuming s >= 0
    if (s <= 2 * 60)
      return StringHandling::toString(s) + " s";
    else
      s /= 60;
    if (s <= 2 * 60)
       return StringHandling::toString(s) + " m";
    else
      s /= 60;
    if (s <= 2 * 24)
      return StringHandling::toString(s) + " h";
    else
      s /= 24;
    if (s <= 2 * 365)
      return StringHandling::toString(s) + " d";
    else
      s /= 365;
    return StringHandling::toString(s) + " y";
  }

  // Progress

  inline Progress::Progress(std::ostream& os, const unsigned int tn, const unsigned int s, const std::string& it, const std::string& rtt)
    : output_stream(os), total_number(tn), skip(s), item_text(it), remaining_time_text(rtt), counter(0), new_goal(skip), time_used(), char_printed(0) {}

  inline void Progress::operator() () {
    if (++counter < new_goal)
      return;
    std::string output(char_printed, '\b');
    const double quotient = double(counter) / total_number;
    output += item_text + StringHandling::toString(counter) + " (" + StringHandling::toString(quotient * 100) + "%); " + remaining_time_text + output_seconds(time_used / quotient - time_used);
    const std::string::size_type size = output.size() - char_printed;
    if (size < char_printed)
      output += std::string(char_printed - size, ' ');
    else
      char_printed = size;
    new_goal += skip;
    output_stream << output << std::flush;
  }
  
}

#endif

