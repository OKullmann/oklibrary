// Oliver Kullmann, 3.4.2017 (Swansea)
/* Copyright 2017, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Tools for timing (possibly system-specific) */

#ifndef TIMING_AxZQb446eB
#define TIMING_AxZQb446eB

namespace Timing {

  // Points in time (in sec):
  typedef double Time_point;

  /* Measuring process time (including all child-processes)

     Via a UserTime object, one can access current total
     usage of *user*-time via "timing()" (in sec, as a "Time_point"); i.e.,
     first a reference point is established via
       UserTime timing;
       const Time_point t0 = timing();
     then the computation is performed, and finally the process-time is
     determined via
       const Time_point t1 = timing();
     Now t1-t0 is the used time in sec.
  */
# ifndef _MSC_VER
  extern "C" {
#  include <sys/resource.h>
  }
  class UserTime {
    mutable rusage timing;
    rusage* const ptiming;
    UserTime(const UserTime&) = delete;
    UserTime& operator =(const UserTime& rhs) = delete;
  public :
    typedef Time_point tpoint_t;
    UserTime() noexcept : ptiming(&timing) {}
    Time_point operator()() const noexcept {
      getrusage(RUSAGE_SELF, ptiming);
      return timing.ru_utime.tv_sec + timing.ru_utime.tv_usec / 1000000.0;
    }
  };
# else
  extern "C" {
#  define NOMINMAX
#  include <Windows.h>
  }
  class UserTime {
    mutable FILETIME a,b,c,d;
    UserTime(const UserTime&) = delete;
    UserTime& operator =(const UserTime& rhs) = delete;
  public :
    UserTime() = default;
    typedef Time_point tpoint_t;
    Time_point operator()() const noexcept {
      if (GetProcessTimes(GetCurrentProcess(), &a,&b,&c,&d) != 0) {
        return
          0.0000001 *
            (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32));
      }
      else return 0;
    }
  };
# endif

}

#endif
