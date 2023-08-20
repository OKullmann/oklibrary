// Oliver Kullmann, 20.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Copying from file-type from a QBF2BCC-like directory to another such
  directory

EXAMPLES:

If DIR is a directory, where the files X in the A-dirs have been updated
(possibly newly created), then via

QBFLIB> DirCopy DIR QBF2BCC X

one can inspect statistics on what would happen, and then via

QBFLIB> DirCopy DIR QBF2BCC X C

the copy is performed (performing it a second time is a no-op, since
the files X in QBF2BCC are only updated if not existing or having a
different content).

*/

#include <iostream>
#include <fstream>
#include <filesystem>

#include <ProgramOptions/Environment.hpp>

#include "DirStatistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "20.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirExtract.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " Xdir Ydir name [mode]\n\n"
    " Xdir, Ydir     : directory-names\n"
    " name           : (generic) filename, to be handled in Xdir, Ydir\n"
    " mode           : \"C\"=perform-copy, else test-only\n\n"

    " copies the name-files from Xdir to Ydir.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 4 and argc != 5) {
    std::cerr << error <<
      "Either three or four arguments (Xdir, Ydir, name, {level])"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const std::filesystem::path Xdir(argv[1]), Ydir(argv[2]);
  if (not std::filesystem::is_directory(Xdir)) {
    std::cerr << error <<
      "Input-X-directory " << Xdir << " is not an (existing) directory.\n";
    return int(Error::input_directory);
  }
  if (not std::filesystem::is_directory(Ydir)) {
    std::cerr << error <<
      "Input-Y-directory " << Ydir << " is not an (existing) directory.\n";
    return int(Error::input_directory);
  }
  if (std::filesystem::equivalent(Xdir, Ydir)) {
    std::cerr << error <<
      "X-directory " << Xdir << " =  Y-directory " << Ydir << ".\n";
    return int(Error::input_directory);
  }
  const std::string name(argv[3]);
  if (name.empty()) {
    std::cerr << error <<
      "File-name \"" << name << "\" is empty.\n";
    return int(Error::filename_empty);
  }
  const bool test_only = argc == 4 or std::string(argv[4]) != "C";

  if (test_only) std::cout << "test-only-mode";
  else std::cout << "copy-mode";
  std::cout << " with filename \"" << name << "\"" << std::endl;
  const auto [X, Xignored] = all_adir(Xdir);
  std::cout << Xdir << "\n  " << X.size() << " " << Xignored << std::endl;
  const auto [Y, Yignored] = all_adir(Ydir);
  std::cout << Ydir << "\n  " << Y.size() << " " << Yignored << std::endl;
  if (Xignored != 0 or Yignored != 0) {
    std::cerr << error <<
      "Repeated A-directories.\n";
    return int(Error::repeated_adirs);
  }

  size_t skipped = 0, copied = 0, targets_exist = 0, targets_equal = 0;
  for (const auto& [i, xd] : X) {
    const auto xpath = xd.dir / name;
    if (not std::filesystem::is_regular_file( xpath)) {
      ++skipped; continue;
    }
    const auto xfind = Y.find(i);
    if (xfind == Y.end()) {
      std::cerr << error <<
        "The A-dir\n" << xd.dir << "\nfrom Xdir is not in Ydir.\n";
      return int(Error::missing_adir);
    }
    const auto yd = xfind->second;
    if (xd != yd) {
      std::cerr << error <<
        "The X-dir\n" << xd << "\nis different from the Ydir:\n"
                << yd << "\n";
      return int(Error::different_adir);
    }
    const auto ypath = yd.dir / name;
    const bool copy_exists = std::filesystem::is_regular_file(ypath);
    targets_exist += copy_exists;
    const std::string xcontent = Environment::get_content(xpath);
    const bool copy_equal =
      copy_exists and xcontent == Environment::get_content(ypath);
    targets_equal += copy_equal;
    if (not copy_equal) {
      ++copied;
      if (not test_only) Environment::put_content(ypath, xcontent);
    }
  }

  std::cout << "skipped: " << skipped << "\n";
  std::cout << "targets-exist: " << targets_exist << "\n";
  std::cout << "targets-equal: " << targets_equal << "\n";
  if (test_only) std::cout << "would-be-copied: ";
  else std::cout << "copied: ";
  std::cout << copied << "\n";
}
