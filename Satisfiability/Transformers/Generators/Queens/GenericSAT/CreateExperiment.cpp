// Oliver Kullmann, 9.9.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "9.8.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/CreateExperiment.cpp",
        "GPL v3"};

  const std::string executable_default = "./ExpQueensRC";
  const std::string experiment_stem = "Experiment_";
  const std::string logfile_name = "logfile";

  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = proginfo.prg;
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;
  const std::string executable = argc<=index ? executable_default : argv[index++];
  using Environment::CurrentTime;
  const CurrentTime Now;
  using Environment::remove;
  const std::string directory_name =
    experiment_stem +
    std::to_string(CurrentTime::ticks_as_uints(Now.ticks)) +
    "_" + remove(Now.date, '.')  + "_" +
    remove(remove(Now.time, ':'), '_');
  index.deactivate();

  using std::filesystem::path;
  std::cout << "Creating directory \"" << directory_name << "\".\n";
  const path directory_path("./" + directory_name);
  if (std::filesystem::exists(directory_path)) return 1;
  if (not std::filesystem::create_directory(directory_path)) return 2;

  const path logfile_path(path(directory_path).append(logfile_name));
  if (std::filesystem::exists(logfile_path)) return 3;
  std::fstream logfile(logfile_path, std::ios_base::out);
  if (not logfile) return 4;

}
