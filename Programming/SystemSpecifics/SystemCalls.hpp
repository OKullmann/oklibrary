// Oliver Kullmann, 17.2.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for making system calls on POSIX systems

  OKlib/General/SystemHandling.hpp contains older code on the topic.

   - Pid_t (typedef)
   - pid()

   - ExitStatus (scoped enum)
   - ReturnValue (class for holding return-values of system-calls)
   - EReturnValue (class extended ReturnValue by strings for out and err)

   - name_prefix (string-constant)
   - system_filename(string stem) (extending stem with fixed prefix and
     suffix "_pid")

   - call_extension(string command, string cin, string cout, string cerr)
     helper-function for producing commands


   - esystem(string command, string cin, string cout, string cerr,
     bool cinexec) -> ReturnValue
     makes a system-call, handling the three streams

   - esystem(string command, string cin,  bool cinexec) > EReturnValue
     handles cout and cerr internally


   - timing_output, timing_command, timing_options_header, timing_options
     (string-constants)
   - Timing (class for holding timing-results)

   - tsystem(string command, string cout, string cerr) -> Timing
     making a timed system-call

   - Ubool (scoped enum for handling "bool + unknown"
   - b2U(bool) (conversion)
   - ProgInfo (class for holding programming-information)
   - PItype (scoped enum for the type of programm-info)
   - version_call (string constant)

   - vsystem(string command, PItype) -> ProgInfo
     extracting program-info for command.

TODOS:

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
#include <regex>

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
      const std::string error = "ERROR[SystemCalls::ReturnValue::status]: ";
      const bool exited = WIFEXITED(ret);
      const bool signaled = WIFSIGNALED(ret);
      const bool stopped = WIFSTOPPED(ret);
      if (exited + signaled + stopped != 1) {
        using std::to_string;
        throw std::runtime_error(error +
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

  std::string call_extension(const std::string& command, const std::string& cin, const std::string& cout, const std::string& cerr, const bool cinexec=false) {
    assert(not cinexec or not cin.empty());
    std::string res;
    if (not cin.empty())
      res = cinexec ? cin + " | " : "cat " + cin + " | ";
    res += command;
    if (not cout.empty()) res += " > " + cout;
    if (not cerr.empty()) res += " 2> " + cerr;
    return res;
  }

  // If cin != "", then either the file cin or the result of the command cin
  // (if cinexec=true) is piped to the command; standard output is redirected
  // to file cout, standard error to cerr:
  ReturnValue esystem(const std::string command, const std::string& cin, const std::string& cout, const std::string& cerr, const bool cinexec=false) {
    std::cout.flush();
    return ReturnValue(std::system(
      call_extension(command, cin, cout, cerr, cinexec).c_str()));
  }
  struct EReturnValue {
    ReturnValue rv;
    std::string out, err;
  };
  EReturnValue esystem(const std::string command, const std::string& cin, const bool cinexec=false) {
    const std::string error = "ERROR[SystemCalls::esystem]: ";
    const std::string out_stem = "esystem_out_";
    const std::string err_stem = "esystem_err_";
    const std::string timestamp =
      std::to_string(Environment::CurrentTime::timestamp());
    const std::string out = system_filename(out_stem + timestamp);
    const std::string err = system_filename(err_stem + timestamp);

    const ReturnValue rv = esystem(command, cin, out, err, cinexec);

    const std::filesystem::path pout(out), perr(err);
    const std::string cout = Environment::get_content(pout);
    const std::string cerr = Environment::get_content(perr);
    try {
      if (not std::filesystem::remove(pout))
        throw std::runtime_error(error + "Can't remove file\n  " + out);
      if (not std::filesystem::remove(perr))
        throw std::runtime_error(error + "Can't remove file\n  " + err);
    }
    catch (const std::filesystem::filesystem_error& e) {
      throw std::runtime_error(error +
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
    const std::string error = "ERROR[SystemCalls::tsystem]: ";
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


  enum class Ubool {f=0, t=1, unknown=2};
  constexpr Ubool b2U(const bool b) noexcept {
    if (b) return Ubool::t; else return Ubool::f;
  }
  struct ProgInfo {
    using string = std::string;
    const std::string prg;
    const std::string vrs;
    const std::string comp;
    const std::string comp_vrs;
    const Ubool ndebug;
    const Ubool optimised;
    ProgInfo(const string& p, const string& v, const string& c, const string& cv, const Ubool n, const Ubool o) noexcept : prg(p), vrs(v), comp(c), comp_vrs(cv), ndebug(n), optimised(o) {}
  };

  enum class PItype {oklib=0,};
  const std::string version_call = " --version";
  ProgInfo vsystem(const std::string& command, const PItype pit = PItype::oklib) {
    const std::string error = "ERROR[SystemCalls::vsystem]: ";
    const EReturnValue rv = esystem(command + version_call, "");
    if (rv.rv.s != ExitStatus::normal)
      throw std::runtime_error(error + "Command \"" + command + "\" results "
        "in abnormal return: " + std::string(rv.rv) + ".\n");
    if (rv.rv.val != 0)
      throw std::runtime_error(error + "Command \"" + command + "\" yields "
        "non-zero return: " + std::to_string(rv.rv.val) + ".\n");
    if (not rv.err.empty()) {
      throw std::runtime_error(error + "Command \"" + command + "\" results "
        "in error-output:\n" +rv.err  + "\n");
    }
    std::smatch p, v, c, cv, n, o;
    switch (pit) {

    case PItype::oklib : {
      const char* const ep =
        R"RAW([pP]rogram name(:| =) +(\w+))RAW";
      if (not std::regex_search(rv.out, p, std::regex(ep)))
         throw std::runtime_error(error + "Command \"" + command + "\" yields "
           "output without parsable program-name:\n" + rv.out + "\n");
      const char* const ev =
        R"RAW([^r] [v|V]ersion: +([\d\.]+))RAW";
      if (not std::regex_search(rv.out, v, std::regex(ev)))
        throw std::runtime_error(error + "Command \"" + command + "\" yields "
           "output without parsable version-number:\n" + rv.out + "\n");
      const char* const ec =
        R"RAW([cC]ompiler version: +(\S+))RAW";
      if (not std::regex_search(rv.out, c, std::regex(ec)))
         throw std::runtime_error(error + "Command \"" + command + "\" yields "
           "output without parsable compiler-info:\n" + rv.out + "\n");
      const char* const ecv =
        R"RAW([cC]ompiler version: +\S+ +([\d\.]+))RAW";
      if (not std::regex_search(rv.out, cv, std::regex(ecv)))
        throw std::runtime_error(error + "Command \"" + command + "\" yields "
           "output without parsable compiler-version:\n" + rv.out + "\n");
      const char* const en =
        R"RAW(NDEBUG:? +(defined|undefined))RAW";
      if (not std::regex_search(rv.out, n, std::regex(en)))
        throw std::runtime_error(error + "Command \"" + command + "\" yields "
           "output without parsable NDEBUG-status:\n" + rv.out + "\n");
      const char* const eo =
        R"RAW((OPTIMIZE:|optimisation) +(on|off))RAW";
      if (not std::regex_search(rv.out, o, std::regex(eo)))
         throw std::runtime_error(error + "Command \"" + command + "\" yields "
           "output without parsable optimisation-status:\n" + rv.out + "\n");
      break;
    }
    default :
      throw std::logic_error(error + "Unhandled value of variable pit: " +
        std::to_string(int(pit)) + ".\n");
    }
    return {p[2], v[1], c[1], cv[1], b2U(n[1]=="defined"), b2U(o[2]=="on")};
  }

}

#endif
