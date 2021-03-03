// Oliver Kullmann, 17.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for making system calls on POSIX systems

  OKlib/General/SystemHandling.hpp contains older code on the topic.

   - Pid_t
   - pid()

   - ExitStatus
   - ReturnValue

   - name_prefix
   - system_filename(string)

   - call_extension(string command, string cin, string cout, string cerr)
   - esystem(string command, string cin, string cout, string cerr)
   - EReturnValue, esystem(string command, string cin)

   - timing_output, timing_command, timing_options_header, timing_options
   - Timing
   - tsystem(string command, string cout, string cerr)

TODOS:

1. Write vsystem(string command), which performs "command --version",
   and extracts relevant information from the output:
    - program name (string)
    - version (string, or triple of natural numbers)
    - compiler name and version (strings)
    - ndebug and optimised (bools).
   Via an option-variable one specifies the "model" for parsing:
    - oklib for our form of version-output; with awk-regular expressions:
     - name: awk '/^program name:/{print $3}'
     - version: awk '/^ version:/{print $2}'
     - compiler: awk '/^compiler version:/{print $3, $4}'
     - optimised: awk '/^ NDEBUG:/{print $2}' == defined
                  awk '/^ OPTIMIZE:/{print $2}' == on
   If there is output on standard-error, or the return is abnormal or
   the return-code is not zero, then an exception is thrown.
   For "oklib" all parsing must be unique and succesful.

*/

#ifndef SYSTEMCALLS_HLADUC6aKT
#define SYSTEMCALLS_HLADUC6aKT

#include <string>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <cassert>
#include <cstdlib> // for system

#include <sys/types.h> // for pid_t
#include <unistd.h> // for getpid
#include <sys/wait.h> // for return value of std::system

#include <ProgramOptions/Environment.hpp>

namespace SystemCalls {

  typedef pid_t Pid_t; // signed integral type, fitting into long
  Pid_t pid() noexcept { return getpid(); }

  enum class ExitStatus { normal=0, aborted=1, stopped=2 };
  struct ReturnValue {
    const ExitStatus s;
    const int val;
    const bool continued;
    ReturnValue(const int ret) noexcept : s(status(ret)), val(value(ret)), continued(WIFCONTINUED(ret)) {}
    operator std::string() const noexcept {
      return std::to_string(int(s)) + " " + std::to_string(val) + " " +
        std::to_string(continued);
    }

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

  ReturnValue esystem(const std::string command, const std::string& cin, const std::string& cout, const std::string& cerr) {
    std::cout.flush();
    return ReturnValue(std::system(
             call_extension(command,cin,cout,cerr).c_str()));
  }
  struct EReturnValue {
    ReturnValue rv;
    std::string out, err;
  };
  EReturnValue esystem(const std::string command, const std::string& cin) {
    const std::string out_stem = "esystem_out_";
    const std::string err_stem = "esystem_err_";
    const std::string timestamp =
      std::to_string(Environment::CurrentTime::timestamp());
    const std::string out = system_filename(out_stem + timestamp);
    const std::string err = system_filename(err_stem + timestamp);

    const ReturnValue rv = esystem(command, cin, out, err);

    const std::filesystem::path pout(out), perr(err);
    const std::string cout = Environment::get_content(pout);
    const std::string cerr = Environment::get_content(perr);
    try {
      if (not std::filesystem::remove(pout))
        throw std::runtime_error("ERROR[SystemCalls::esystem]: "
          "Can't remove file\n  " + out);
      if (not std::filesystem::remove(perr))
        throw std::runtime_error("ERROR[SystemCalls::esystem]: "
          "Can't remove file\n  " + err);
    }
    catch (const std::filesystem::filesystem_error& e) {
      throw std::runtime_error("ERROR[SystemCalls::esystem]: "
        "OS-error when removing auxiliary files " + out + ", " + err);
    }
    return {rv, cout, cerr};
  }


  const std::string timing_output = "TIMING";
  const std::string timing_command = "/usr/bin/time";
  const std::string timing_options_header = "UeSPMZxC";
  const std::string timing_options =
    "-f \"" + timing_options_header +
    "  %U %e %S %P %M %Z %x %C\" --quiet  --output=";
  struct Timing {
    const ReturnValue rv; // for the timing-command itself
    long double u,e,s; // all times in seconds: user,elapsed,system
    long double p; // process-usage in percent
    unsigned long long m; // maximal resident memory in Kbytes
    unsigned long long z; // page-size in bytes
    int x; // return-code of executed command
    std::string c; // command plus arguments as given to time
    Timing(const ReturnValue r) : rv(r) {}
  };

  Timing tsystem(const std::string command, const std::string& cout, const std::string& cerr) {
    const std::string error = "ERROR[SystemCalls]: ";
    const std::string tresult = system_filename(timing_output);
    const std::string tcommand = timing_command + " " + timing_options +
      tresult + " " + command;
    const ReturnValue rv =
      std::system(call_extension(tcommand,"",cout,cerr).c_str());
    if (rv.s != ExitStatus::normal)
      throw std::runtime_error(error + "Timing-call\n  " + tcommand +
        "\n results in abnormal return: " + std::string(rv) + "\n");
    else if (rv.val == 127)
      throw std::runtime_error(error + "Command to be timed does not exist:"
        "\n  " + tcommand + "\n");
    else if (rv.val == 126)
      throw std::runtime_error(error + "Timing-command\n  " +
        tcommand + "\n could not be invoked.\n");
    std::filesystem::path out(tresult);
    std::ifstream content(tresult);
    if (not content)
      throw std::runtime_error(error + "Can't open file\n  " + tresult + "\n");
    std::stringstream s; s << content.rdbuf(); content.close();
    if (s.bad() or content.bad())
      throw std::runtime_error(error + "Reading-error with file\n  " +
        tresult + "\n");
    std::string read;
    s >> read;
    if (s.bad() or read != timing_options_header)
      throw std::runtime_error(error + "Time-output has wrong header:\n  " +
        read + "\n");
    Timing res(rv);
    s >> res.u >> res.e >> res.s;
    s >> read;
    if (not read.ends_with('%'))
      throw std::runtime_error(error +
        "Processor-percentage not completed with \"%\":\n  " + read + "\n");
    read.pop_back();
    if (read == "?") res.p = 100;
    else res.p = std::stold(read);
    s >> res.m >> res.z >> res.x;
    res.c = Environment::transform_spaces(s.str());
    if (s.bad())
      throw std::runtime_error(error + "Syntax-error with file\n  " +
        tresult + "\n");
    if (not std::filesystem::remove(out))
      throw std::runtime_error(error + "Auxiliary file can't be removed:\n  " +
        tresult + "\n");
    return res;
  }

}

#endif
