// Oliver Kullmann, 27.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/DirTiming.cpp
  \brief Applying Timing.cpp to all files in a directory

  USAGE:

  > DirTiming Program Directory

  Applies Timing to the contents of the directory, for the format of files
  therein as follows:

  The primary files is F.cmd ("F" an arbitrary name), which contains:
   1. line : the arguments for Program
   2. line (optional): the range of allowed codes
   3. line (optional): N.

   The result of Timing is appended to F.R, with leading:
    1. Program
    2. version number
    3. name of machine
    4. bogomips

   The first two entries are obtained  cabylling "Program --version".

*/

#include <iostream>
#include <filesystem>

#include <cstdlib>
#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <SystemSpecifics/SystemCalls.hpp>

#include "DirMatching.hpp"
#include "Timing.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "7.3.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/InputOutput/DirTiming.cpp",
        "GPL v3"};

  namespace DM = DirMatching;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " Program Directory\n"
 ;
    return true;
  }

  enum class Error {
    empty_file = 31,
    too_many_lines = 32,
    abnormal_exit = 33,
    non_zero_code = 34,
    error_output = 35,
    append = 36,
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3) {
    std::cerr << error << "Exactly two input parameters are required:\n"
      " - the Program (to be timed),\n"
      " - the Directory with the timing-files.\n";
    return int(DM::Error::pnumber);
  }
  const std::string Program = argv[1];
  if (Program.empty()) {
    std::cerr << error << "Program is the empty string.\n";
    return int(DM::Error::empty_program);
  }
  const std::string Directory = argv[2];
  if (Directory.empty()) {
    std::cerr << error << "Directory is the empty string.\n";
    return int(DM::Error::empty_directory);
  }

  namespace SC = SystemCalls;
  const SC::ProgInfo progvers = SC::vsystem(Program);

  namespace fs = std::filesystem;
  const std::string aProgram = DM::make_absolute(Program, error);
  const fs::path pDirectory = DM::convert_dir(Directory, error);
  const fs::path home = fs::current_path();
  try { fs::current_path(pDirectory); }
  catch (const fs::filesystem_error& e) {
    std::cerr << error << "Can't change to directory " << pDirectory <<
      ":\n" << e.what();
    std::exit(int(DM::Error::invalid_directory));
  }
  const DM::files_t files = DM::find_cmds(pDirectory, error);
  if (files.empty()) return 0;

  for (const auto& testcase : files) {

    const fs::path cmd_path = testcase.path();
    const std::string cmd_file = cmd_path.string();
    assert(cmd_file.ends_with(".cmd"));
    const auto contents = DM::split(cmd_path, error);
    if (contents.empty()) {
      std::cerr << error << "File\n " << cmd_path << "\nis empty.\n";
      std::exit(int(Error::empty_file));
    }
    if (contents.size() > 3) {
      std::cerr << error << "File\n " << cmd_path << "\nhas " <<
        contents.size() << " lines.\n";
      std::exit(int(Error::too_many_lines));
    }
    const std::string params = contents[0],
      codes = contents.size() > 1 ? contents[1] : "\"\"",
      N = contents.size() > 2 ? contents[2] : "\"\"";
    const std::string timing_call = "Timing \"" + Program + " " + params +
      "\" " + codes + " " + N + " quiet";
    const SC::EReturnValue rv = SC::esystem(timing_call, "");
    if (rv.rv.s != SC::ExitStatus::normal) {
      std::cerr << error << "The Timing-program called via\n  " <<
        timing_call << "\n had an abnormal return:\n   " << std::string(rv.rv)
                << " with error-output:\n  " << rv.err;
      std::exit(int(Error::abnormal_exit));
    }
    if (rv.rv.val != 0) {
      std::cerr << error << "The Timing-program called via\n  " <<
        timing_call << "\n returned non-zero code " << rv.rv.val <<
        " with error-output:\n  " << rv.err;
      std::exit(int(Error::non_zero_code));
    }
    if (not rv.err.empty()) {
      std::cerr << error << "The Timing-program called via\n  " <<
        timing_call << "\n had non-empty error-output:\n  " << rv.err;
      std::exit(int(Error::error_output));
    }

    const std::string out = cmd_file.substr(0, cmd_file.size() - 4) + ".R";
    const fs::path pout(out);
    const bool exists = fs::exists(pout);
    std::ofstream output(out, std::ios_base::app);
    if (not output) {
      std::cerr << error << "Could not open file\n " << pout << "\n";
      std::exit(int(Error::append));
    }
    if (not exists) {
      using namespace Timing;
      output << "prog      vers   machine  bogomips ";
      print_header(header_or, header_or2, width_or0, width_or, width_or2,
                   output);
    }
    output << progvers.prg << "  "  << progvers.vrs << "  " << proginfo.machine
           << " " << proginfo.bogomips << " ";
    output << rv.out;
    if (not output) {
      std::cerr << error << "Writing-error with file\n " << pout << "\n";
      std::exit(int(Error::append));
    }
  }

}

