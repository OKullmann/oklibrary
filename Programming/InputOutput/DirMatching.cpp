// Oliver Kullmann, 16.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/DirMatching.cpp
  \brief Applying Matching.cpp to all files in a directory

  USAGE:

  > DirMatching Program Directory

  Applies Matching (which is just called by its name) to the outputs of
  Program, as given by the contents of Directory; the format of the files
  therein is as follows:

  The primary file is "F.cmd" ("F" an arbitrary name), containing
  the command-line input; five further optional accompanying files:
   - F.in : input from standard input
   - F.out[_lm | _fm] : output to standard output (regular expression)
   - F.err[_lm | _fm] : output to standard error (regular expression)
   - F.code : output code (regular expression)
   - F.data : possibly referred to in the command-line (could by any name).

  For F.out, F.err the program Matching is applied.
  If one or two of these files do not exist, then it is an error, if there is
  output on the corresponding stream.

  Creates temporary files DirMatching_F.out|err, in the working-directory (the
  directory where DirMatching is called); these files are not deleted in case
  of error.

  If Program is given via a pathname (relative to working-dir), then
  "eprogram" is the absolute path to program, otherwise eprogram is Program.
  Directory can be given as a relative path (relative to the
  working-directory).

  If there is no F.in, then the system-call for creating the output is, after
  changing the current working-directory to Directory:

    eprogram [command-line arguments] > DirMatching_F.out 2> DirMatching_F.err

  Otherwise

    cat F.in | eprogram [arguments] > DirMatching_F.out 2> DirMatching_F.err

  DONE The return-code is as returned by std::system:
   - In OKlib/General/SystemHandling.hpp there is (older) code for the
     evaluation of the the value returned (for POSIX-systems).
   - The class SystemHandling::PidType seems closest to our needs, regarding
     interpreting the return-code.
   - The class System_sh there performs a system-call, and makes all results
     available.
   - There are also more powerful classes, for setting resources

  Without F.code, return-code 0 is assumed (everything else is an error).

*/

#include <iostream>
#include <filesystem>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <SystemSpecifics/SystemCalls.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "17.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/InputOutput/DirMatching.cpp",
        "GPL v3"};

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
    pnumber = 1,
    empty_program = 2,
    empty_directory = 3,
    invalid_program = 4,
    os_error = 5,
    invalid_directory = 6,
  };

  std::string make_absolute(const std::string& prog) {
    const std::filesystem::path path(prog);
    const bool is_free = path.filename() == prog;
    try {
      return
        is_free ? prog : std::filesystem::canonical(path).string();
    }
    catch (const std::filesystem::filesystem_error& e) {
      std::cerr << error << "The path \"" << prog <<
        "\" for the program is invalid:\n" << e.what();
      std::exit(int(Error::invalid_program));
    }
  }

  std::filesystem::path convert_dir(const std::string& dir) {
    const std::filesystem::path path(dir);
    std::filesystem::file_status status;
    try { status = std::filesystem::status(path); }
    catch (const std::filesystem::filesystem_error& e) {
      std::cerr << error << "The directory \"" << dir <<
        "\" leads to an OS-error:\n" << e.what();
      std::exit(int(Error::os_error));
    }
    if (status.type() != std::filesystem::file_type::directory) {
      std::cerr << error << "The directory \"" << dir <<
        "\" does not exist.\n";
      std::exit(int(Error::invalid_directory));
    }
    try { return std::filesystem::canonical(path); }
    catch (const std::filesystem::filesystem_error& e) {
      std::cerr << error << "The path \"" << dir <<
        "\" for the directory can't be made canonical:\n" << e.what();
      std::exit(int(Error::invalid_directory));
    }
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3) {
    std::cerr << error << "Exactly two input parameters are required:\n"
      " - the Program (to be tested),\n"
      " - the Directory with the test-cases.\n";
    return int(Error::pnumber);
  }
  const std::string Program = argv[1];
  if (Program.empty()) {
    std::cerr << error << "Program is the empty string.\n";
    return int(Error::empty_program);
  }
  const std::string Directory = argv[2];
  if (Directory.empty()) {
    std::cerr << error << "Directory is the empty string.\n";
    return int(Error::empty_directory);
  }

  const std::string aProgram = make_absolute(Program);
  const std::filesystem::path pDirectory = convert_dir(Directory);
  std::cerr << pDirectory << "\n";
}
