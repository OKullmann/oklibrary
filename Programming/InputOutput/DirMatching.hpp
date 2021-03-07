// Oliver Kullmann, 7.3.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#ifndef DIRMATCHING_r5Vw4EPEBY
#define DIRMATCHING_r5Vw4EPEBY

#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>

namespace DirMatching {

  enum class Error {
    pnumber = 1,
    empty_program = 2,
    empty_directory = 3,
    invalid_program = 4,
    os_error = 5,
    invalid_directory = 6,
    invalid_cmd = 7,
    invalid_file = 8,
    remove_stdout = 9,
    remove_stderr = 11,
    invalid_infile = 12,
    continued = 13,
    stopped = 14,
    aborted = 15,
    no_code = 16,
    regular_expression = 17,
    code_mismatch = 18,
    missing_pout = 19,
    missing_perr = 21,
    matching_continued = 22,
    matching_stopped = 23,
    matching_aborted = 24,
    mismatch = 25,
    remove_stderr_match = 26,
  };

  // Making filenames absolute except they do not contain a path-element
  // (then just return the string):
  std::string make_absolute(const std::string& prog, const std::string& error) {
    const std::filesystem::path path(prog);
    if (path.filename() == prog) return prog;
    try { return std::filesystem::canonical(path).string(); }
    catch (const std::filesystem::filesystem_error& e) {
      std::cerr << error << "The path \"" << prog <<
        "\" for the program is invalid:\n" << e.what();
      std::exit(int(Error::invalid_program));
    }
  }

  // Check whether directory exists, and return canonical path:
  std::filesystem::path convert_dir(const std::string& dir, const std::string& error) {
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

  typedef std::vector<std::filesystem::directory_entry> files_t;
  files_t find_cmds(const std::filesystem::path& dir, const std::string error) {
    files_t res;
    for (const auto& p : std::filesystem::directory_iterator(dir)) {
      if (not p.path().filename().string().ends_with(".cmd")) continue;
      if (not p.is_regular_file()) {
        std::cerr << error << "The cmd-file\n  " << p.path() <<
          "\nis not a regular file.\n";
        std::exit(int(Error::invalid_cmd));
      }
      res.push_back(p);
    }
    std::sort(res.begin(), res.end());
    return res;
  }



}

#endif
