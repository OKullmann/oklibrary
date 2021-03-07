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
    1. version number
    2. name of machine
    3. bogomips
    4. Program

   These three numbers are obtained from calling "Program --version".

*/

#include <iostream>
#include <filesystem>

#include <cstdlib>
#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <SystemSpecifics/SystemCalls.hpp>

#include "DirMatching.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.4",
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
    tobecompleted = 1,
    
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

  namespace fs = std::filesystem;
  const std::string aProgram = DM::make_absolute(Program, error);
  const fs::path pDirectory = DM::convert_dir(Directory, error);


}

