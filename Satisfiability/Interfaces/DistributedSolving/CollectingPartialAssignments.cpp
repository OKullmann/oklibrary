// Oliver Kullmann, 19.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/DistributedSolving/CollectingPartialAssignments.cpp
  \brief Application for collecting partial assignments into one file

  <ul>
   <li> Takes exactly two arguments, "Data" and "Dir". </li>
   <li> This is the data-file and the instance-directory (as produced by
   SplittingViaOKsolver). </li>
   <li> The data-files contains one header-line, and then lines whose second
   entry (space-separated from the first entry) is an index i. </li>
   <li> The content of file Dir/i is output to standard output (in the order
   of the lines from file Data). </li>
  </ul>

*/

#include <iostream>
#include <fstream>
#include <limits>
#include <string>

namespace {

  enum {
    errcode_parameter = 1,
    errcode_data = 2,
    errcode_reading_data = 3,
    errcode_reading_pa = 4 };

  const std::string program = "CollectingPartialAssignments";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.6";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 3) {
    std::cerr << err << "Exactly two arguments are needed, the data-file\n"
     " and the instance-directory.\n";
    return errcode_parameter;
  }
  std::ifstream data_file(argv[1]);
  if (not data_file) {
    std::cerr << err << "Could not open data-file \"" << argv[1] << "\".\n";
    return errcode_data;
  }
  data_file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
  while (data_file.peek() != EOF) {
    data_file.ignore(std::numeric_limits<std::streamsize>::max(),' ');
    std::string file_number;
    data_file >> file_number;
    data_file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    if (not data_file) {
      std::cerr << err << "Reading error in \"" << argv[1] << "\".\n";
      return errcode_reading_data;
    }
    const std::string pa_name = std::string(argv[2]) + "/" + file_number;
    std::ifstream pa(pa_name.c_str());
    std::cout << pa.rdbuf();
    if (not pa) {
      std::cerr << err << "Reading error for \"" << pa_name << "\".\n";
      return errcode_reading_pa;
    }
  }
}
