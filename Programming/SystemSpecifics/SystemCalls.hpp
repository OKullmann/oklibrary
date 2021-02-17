// Oliver Kullmann, 17.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for making system calls on POSIX systems

  OKlib/General/SystemHandling.hpp contains older code on the topic.

*/

#ifndef SYSTEMCALLS_HLADUC6aKT
#define SYSTEMCALLS_HLADUC6aKT

#include <cstdlib> // for system, getenv

#include <sys/types.h> // for pid_t
#include <unistd.h> // for getpid
#include <sys/wait.h> // for return value of std::system

namespace SystemCalls {

  typedef pid_t Pid_t; // signed integral type, fitting into long

  Pid_t pid() noexcept { return getpid(); }

}

#endif
