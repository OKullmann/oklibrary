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

#include <string>
#include <exception>

#include <cassert>
#include <cstdlib> // for system

#include <sys/types.h> // for pid_t
#include <unistd.h> // for getpid
#include <sys/wait.h> // for return value of std::system

namespace SystemCalls {

  typedef pid_t Pid_t; // signed integral type, fitting into long
  Pid_t pid() noexcept { return getpid(); }

  enum class ExitStatus { normal=0, aborted=1, stopped=2 };
  struct ReturnValue {
    const ExitStatus s;
    const int val;
    const bool continued;
    ReturnValue(const int ret) noexcept : s(status(ret)), val(value(ret)), continued(WIFCONTINUED(ret)) {}

    static ExitStatus status(const int ret) {
      const bool exited = WIFEXITED(ret);
      const bool signaled = WIFSIGNALED(ret);
      const bool stopped = WIFSTOPPED(ret);
      if (exited + signaled + stopped != 1) {
        using std::to_string;
        throw std::runtime_error("ERROR[SystemCalls::ReturnValue::status: " +
          to_string(exited) + to_string(signaled) +  to_string(stopped));
      }
      else if (exited) return ExitStatus::normal;
      else if (signaled) return ExitStatus::aborted;
      else return ExitStatus::stopped;
    }
  private :
    int value(const int ret) noexcept {
      switch (s) {
      case ExitStatus::normal : return WEXITSTATUS(ret);
      case ExitStatus::aborted : return WTERMSIG(ret);
      default : return WSTOPSIG(ret); }
    }
  };

  const std::string name_prefix = "SystemCalls_";
  std::string system_filename(const std::string& stem) {
    static const std::string p = std::to_string(pid());
    assert(not p.empty());
    return name_prefix + stem + "_" + p;
  }

  std::string call_extension(const std::string& command, const std::string& cin, const std::string& cout, const std::string& cerr) {
    std::string res;
    if (not cin.empty()) res = "cat " + cin + " | ";
    res += command;
    if (not cout.empty()) res += " > " + cout;
    if (not cerr.empty()) res += " 2> " + cerr;
    return res;
  }

}

#endif
