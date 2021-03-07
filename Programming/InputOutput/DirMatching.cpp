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
   - F.out(_lm | _fm) : output to standard output (regular expression)
   - F.err(_lm | _fm) : output to standard error (regular expression)
   - F.code : output code (regular expression)
   - F* : possibly referred to in the command-line (could be any name).

  To F.out[], F.err[] the program Matching is applied.
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
   - There are also more powerful classes, for setting resources.

  Without F.code, return-code 0 is assumed (everything else is an error).

*/

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>

#include <cstdlib>
#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <SystemSpecifics/SystemCalls.hpp>

#include "Matching.hpp"
#include "DirMatching.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.7",
        "7.3.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/InputOutput/DirMatching.cpp",
        "GPL v3"};

  using namespace DirMatching;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " Program Directory\n"
 ;
    return true;
  }

  bool check_file(const std::filesystem::path& p) {
    namespace fs = std::filesystem;
    if (not fs::exists(p)) return false;
    const auto status = fs::status(p);
    if (status.type() != fs::file_type::regular) {
      std::cerr << error << "Input-file\n" << p << "\nnot a regular file.\n";
      std::exit(int(Error::invalid_infile));
    }
    if ((status.permissions() & fs::perms::owner_read) == fs::perms::none) {
      std::cerr << error << "Input-file\n" << p << "\nis not readable.\n";
      std::exit(int(Error::invalid_infile));
    }
    return true;
  }

  void report_outerr(const std::filesystem::path& t, const std::string& p,
                     const std::string& out, const std::string& err) {
    std::cerr << "TESTCASE-ERROR:\n  " << t << "\n  " << p << "\n";
    if (not out.empty())
      std::cerr << "Standard-Output:\n  " << "\"" << out << "\"\n";
    if (not err.empty())
      std::cerr << "Standard-Error:\n  " << "\"" << err << "\"\n";
  }

  void matching(const std::filesystem::path& home, const std::filesystem::path& t, const std::string& p,
                const std::filesystem::path& pattern, const std::filesystem::path& comp, const bool lm) {
    namespace fs = std::filesystem; namespace SS = SystemCalls;
    const std::string err = SS::system_filename("matching_stderr");
    const fs::path perr = home / err;
    const std::string mo = Environment::RegistrationPolicies<Matching::MatO>::string[int(lm ? Matching::MatO::lines : Matching::MatO::full)];
    const std::string command = "Matching " + pattern.string() + " " + comp.string() + " " + mo;
    const auto rv = SS::esystem(command, "", "", perr.string());
    const std::string cerr = get_content(perr, error);
    if (rv.continued) {
      std::cerr << error << "Matching with " << pattern << ":\n"
        "Return-code says \"continued\".\n";
      report_outerr(t,p, "", cerr);
      std::exit(int(Error::matching_continued));
    }
    if (rv.s == SS::ExitStatus::stopped) {
      std::cerr << error << "Matching with " << pattern << ":\n"
        "Return-code says \"caught signal\" " << rv.val << ".\n";
      report_outerr(t,p, "", cerr);
      std::exit(int(Error::matching_stopped));
    }
    if (rv.s == SS::ExitStatus::aborted) {
      std::cerr << error << "Matching with " << pattern << ":\n"
        "Return-code says \"aborted by signal\" " << rv.val << ".\n";
      report_outerr(t,p, "", cerr);
      std::exit(int(Error::matching_aborted));
    }
    assert(rv.s == SS::ExitStatus::normal);
    if (rv.val != 0 and rv.val != int(Matching::Error::mismatch)) {
      std::cerr << error << "Matching with " << pattern << ":\n"
        "Execution-error with return-code " << rv.val << ".\n";
      report_outerr(t,p, "", cerr);
      std::exit(int(Error::matching_aborted));
    }
    if (rv.val == int(Matching::Error::mismatch)) {
      report_outerr(t,p, "", cerr);
      std::cerr << "PROBLEM: Mismatch with " << pattern << ".\n";
      std::exit(int(Error::mismatch));
    }
    try {
      if (not fs::remove(perr)) {
        std::cerr << error << "File for removal does not exist:\n" << perr
                  << "\n";
        std::exit(int(Error::remove_stderr_match));
      }
    }
    catch (const fs::filesystem_error& e) {
      std::cerr << error << "OS-error when removing auxiliary file\n"
                << perr << ":\n" << e.what();
      std::exit(int(Error::os_error));
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

  namespace fs = std::filesystem;
  const std::string aProgram = make_absolute(Program, error);
  const fs::path pDirectory = convert_dir(Directory, error);

  const fs::path home = fs::current_path();
  const std::string
    stdout = SystemCalls::system_filename("DirMatching_stdout"),
    stderr = SystemCalls::system_filename("DirMatching_stderr");
  try { fs::current_path(pDirectory); }
  catch (const fs::filesystem_error& e) {
    std::cerr << error << "Can't change to directory " << pDirectory <<
      ":\n" << e.what();
    std::exit(int(Error::invalid_directory));
  }
  const fs::path
    pstdout = home / stdout,
    pstderr = home / stderr;

  const files_t files = find_cmds(pDirectory, error);
  if (files.empty()) return 0;

  for (const auto& testcase : files) {

    const fs::path cmd_path = testcase.path();
    const std::string cmd_file = cmd_path.string();
    assert(cmd_file.ends_with(".cmd"));
    const std::string params =
      Environment::remove_trailing_spaces(get_content(cmd_path, error));
    const std::string command = aProgram + " " + params;
    const std::string stem = cmd_file.substr(0, cmd_file.size() - 4);

    const bool with_stdin = check_file(pDirectory / (stem + ".in")),
      with_code = check_file(pDirectory / (stem + ".code")),
      with_outlm  = check_file(pDirectory / (stem + ".out_lm")),
      with_outfm  = check_file(pDirectory / (stem + ".out_fm")),
      with_errlm  = check_file(pDirectory / (stem + ".err_lm")),
      with_errfm  = check_file(pDirectory / (stem + ".err_fm"));
    assert(not with_outlm or not with_outfm);
    assert(not with_errlm or not with_errfm);
    const bool with_out = with_outlm or with_outfm;
    const bool with_err = with_errlm or with_errfm;

    namespace SC = SystemCalls;
    const auto rv =
      SC::esystem(command,
        with_stdin ? stem+".in" : "", pstdout.string(), pstderr.string());
    const std::string out = get_content(pstdout, error),
      err = get_content(pstderr, error);

    if (rv.continued) {
      std::cerr << error << "Return-code says \"continued\".\n";
      report_outerr(cmd_path, aProgram, out, err);
      std::exit(int(Error::continued));
    }
    if (rv.s == SC::ExitStatus::stopped) {
      std::cerr << error << "Return-code says \"caught signal\" " <<
        rv.val << ".\n";
      report_outerr(cmd_path, aProgram, out, err);
      std::exit(int(Error::stopped));
    }
    if (rv.s == SC::ExitStatus::aborted) {
      std::cerr << error << "Return-code says \"aborted by signal\" " <<
        rv.val << ".\n";
      report_outerr(cmd_path, aProgram, out, err);
      std::exit(int(Error::aborted));
    }
    assert(rv.s == SC::ExitStatus::normal);
    if (with_code or rv.val != 0) {
      if (not with_code) {
        std::cerr << error << "Return-code not zero: " <<
          rv.val << ", but no code-file.\n";
        report_outerr(cmd_path, aProgram, out, err);
        std::exit(int(Error::no_code));
      }
      const std::string code = Environment::remove_trailing_spaces(
        get_content(pDirectory / (stem + ".code"), error));
      std::regex reg;
      try { reg.assign(code); }
      catch (const std::regex_error& e) {
        std::cerr << error << "Regular expression error in code-file:\n"
          "file: " << (pDirectory / (stem + ".code")) << "\n"
          "expression: \"" << code << "\"\n"
          "what: " << e.what() << "\n"
          "error-code: " << e.code() << "\n";
        std::exit(int(Error::regular_expression));
      }
      if (not std::regex_match(std::to_string(rv.val), reg)) {
        report_outerr(cmd_path, aProgram, out, err);
        std::cerr << "PROBLEM: Return-code mismatch:\n"
          "Pattern  : \"" << code << "\"\n"
          "Returned : \"" << rv.val << "\"\n";
        return int(Error::code_mismatch);
      }
    }

    if (not out.empty() and not with_out) {
      report_outerr(cmd_path, aProgram, out, err);
      std::cerr << "PROBLEM: There is standard-output, but no output-pattern.\n";
      return int(Error::missing_pout);
    }
    if (with_out)
      matching(home, cmd_path, aProgram,
        with_outlm ? pDirectory/(stem+".out_lm") : pDirectory/(stem+".out_fm"),
        pstdout, with_outlm);
    if (not err.empty() and not with_err) {
      report_outerr(cmd_path, aProgram, out, err);
      std::cerr << "PROBLEM: There is error-output, but no error-pattern.\n";
      return int(Error::missing_pout);
    }
    if (with_err)
      matching(home, cmd_path, aProgram,
        with_errlm ? pDirectory/(stem+".err_lm") : pDirectory/(stem+".err_fm"),
        pstderr, with_errlm);
  }

  try {
    if (not fs::remove(pstdout)) {
      std::cerr << error << "File for removal does not exist:\n" << pstdout
                << "\n";
      std::exit(int(Error::remove_stdout));
    }
    if (not fs::remove(pstderr)) {
      std::cerr << error << "File for removal does not exist:\n" << pstderr
                << "\n";
      std::exit(int(Error::remove_stderr));
    }
  }
  catch (const fs::filesystem_error& e) {
    std::cerr << error << "OS-error when removing auxiliary files\n"
              << pstdout << "\n" << pstderr << "\n" << e.what();
    std::exit(int(Error::os_error));
  }
}
