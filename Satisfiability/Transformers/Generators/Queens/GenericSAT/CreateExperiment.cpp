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
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "CreateExperiment.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "10.8.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/CreateExperiment.cpp",
        "GPL v3"};

  using namespace CreateExperiment;

  const std::string executable_default = "./ExpQueensRC";
  constexpr par_t N_upper_default = 17;
  constexpr par_t N_lower_default = 4;

  const std::string experiment_stem = "Experiment_";
  const std::string logfile_name = "logfile";
  const std::string makefile_name = "Makefile";
  const std::string resultfile_name = "results.R";

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
  const par_t N_upper = argc<=index ? N_upper_default : std::stoi(argv[index++]);
  const par_t N_lower = argc<=index ? N_lower_default : std::stoi(argv[index++]);
  const std::string executable = argc<=index ? executable_default : argv[index++];
  index.deactivate();

  using namespace std::filesystem;
  const path executable_path(executable);
  if (not is_regular_file(executable_path)) return 1;
  const std::string executable_filename = executable_path.filename();

  using Environment::CurrentTime;
  const CurrentTime Now;
  using Environment::remove;
  const std::string directory_name =
    experiment_stem +
    std::to_string(N_lower) + "_" + std::to_string(N_upper) + "_" +
    std::to_string(CurrentTime::ticks_as_uints(Now.ticks)) +
    "_" + remove(Now.date, '.')  + "_" +
    remove(remove(Now.time, ':'), '_');

  std::cout << "Creating directory \"" << directory_name << "\".\n";
  const path directory_path("./" + directory_name);
  if (exists(directory_path)) return 2;
  if (not create_directory(directory_path)) return 3;

  copy(executable_path, directory_path);

  const path logfile_path(path(directory_path).append(logfile_name));
  if (exists(logfile_path)) return 4;
  std::fstream logfile(logfile_path, std::ios_base::out);
  if (not logfile) return 5;

  logfile << proginfo << "\n";
  if (not logfile) return 6;
  logfile.close();
  if (std::system((directory_name + "/" + executable_filename + " -v >> " + std::string(logfile_path)).c_str()) != 0) return 7;

  const path makefile_path(path(directory_path).append(makefile_name));
  if (exists(makefile_path)) return 8;
  std::fstream makefile(makefile_path, std::ios_base::out);
  if (not makefile) return 9;

  const path resultfile_path(path(directory_path).append(resultfile_name));
  if (exists(resultfile_path)) return 10;
  std::fstream resultfile(resultfile_path, std::ios_base::out);
  if (not resultfile) return 11;
  resultfile.close();
  if (std::system((directory_name + "/" + executable_filename + " -rh >> " + std::string(resultfile_path)).c_str()) != 0) return 12;

  write_makefile(makefile, make_job_description({{N_lower,N_upper}, {0,7},{0,3}}), "./" + executable_filename, resultfile_path.filename());

}
