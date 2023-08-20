// Oliver Kullmann, 20.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Copying from file-type from a QBF2BCC-like directory to another such
  directory

*/

#include <iostream>
#include <fstream>
#include <filesystem>

#include <ProgramOptions/Environment.hpp>

#include "DirStatistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.4",
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
         << " Xdir YDir name\n\n"
    " copies the name-files from Xdir to YDir.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 4) {
    std::cerr << error <<
      "Exactly three arguments (Xdir, Ydir, name)"
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


  const auto [X, Xignored] = all_adir(Xdir);
  std::cout << Xdir << ": " << X.size() << " " << Xignored << std::endl;
  const auto [Y, Yignored] = all_adir(Ydir);
  std::cout << Ydir << ": " << Y.size() << " " << Yignored << std::endl;

}
