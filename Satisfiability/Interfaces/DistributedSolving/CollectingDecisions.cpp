// Oliver Kullmann, 25.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/DistributedSolving/CollectingDecisions.cpp
  \brief Application for collecting decision assignments into one file

  <ul>
   <li> For processing the decision-assignments as stored in a directory
   produced by SplittingViaOKsolver. </li>
   <li> Takes exactly three arguments, "Data", "Dir" and N. </li>
   <li> This is the data-file and the instance-directory (as produced by
   SplittingViaOKsolver), and the number of instances. </li>
   <li> The data-files contains one header-line, and then lines whose second
   entry (space-separated from the first entry) is an index i. </li>
   <li> The directory contains the file "decisions", whose i'th line consists
   of natural numbers j_1,...,j_k, k >= 0, j_l >= 0. </li>
   <li> File Dir/i contains space-separated integers, which fills an array A,
   with zero-based indices, and A[j_1], ..., A[j_k] are output to standard
   output (in the order of the lines from file Data). </li>
  </ul>

*/

#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

namespace {

  enum {
    errcode_parameter = 1,
    errcode_data = 2,
    errcode_decisions_file = 3,
    errcode_N = 4,
    errcode_number_decisions = 5,
    errcode_reading_decisions = 6,
    errcode_reading_data = 7,
    errcode_running_number = 8,
    errcode_file_number_0 = 9,
    errcode_file_number_N = 10,
    errcode_reading_pa = 11,
    errcode_index = 12
  };

  const std::string program = "CollectingDecisions";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.6";

  typedef int literal_type;
  typedef std::vector<literal_type> literalvector_type;
  typedef literalvector_type::size_type index_type;
  typedef std::vector<index_type> indexvector_type;
  typedef long unsigned int count_type;

}

int main(const int argc, const char* const argv[]) {
  if (argc != 4) {
    std::cerr << err << "Exactly three arguments are needed, the data-file,\n"
     " the instance-directory, and the number of files.\n";
    return errcode_parameter;
  }

  const std::string data_filename = argv[1];
  std::ifstream data_file(data_filename.c_str());
  if (not data_file) {
    std::cerr << err << "Could not open data-file \"" << data_filename << "\".\n";
    return errcode_data;
  }
  const std::string dirname = argv[2];
  const std::string decisions_filename = dirname + "/decisions";
  std::ifstream decisions_file(decisions_filename.c_str());
  if (not decisions_file) {
    std::cerr << err << "Could not open decisions-file \"" << decisions_filename << "\".\n";
    return errcode_decisions_file;
  }
  count_type N;
  {std::stringstream read_number;
   read_number << argv[3];
   read_number >> N;
   if (not read_number) {
     std::cerr << err << "The third argument must be a natural number of appropriate size.\n";
    return errcode_N;
   }
  }

  typedef std::vector<indexvector_type> table_t;
  table_t T; T.reserve(N);
  for (count_type i = 0; i < N; ++i) {
    index_type number_decisions = 0;
    decisions_file >> number_decisions;
    if (not decisions_file) {
      std::cerr << err << "Reading error for number of decisions in \"" << decisions_filename << "\".\n";
      return errcode_number_decisions;
    }
    indexvector_type I(number_decisions);
    for (index_type j = 0; j < number_decisions; ++j) {
      decisions_file >> I[j];
      if (not decisions_file) {
        std::cerr << err << "Reading error in \"" << decisions_filename << "\".\n";
        return errcode_reading_decisions;
      }
    }
    T.push_back(I);
  }
  assert(T.size() == N);

  data_file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
  for (count_type counter = 1; counter <= N; ++counter) {

    count_type running_number;
    data_file >> running_number;
    count_type file_number;
    data_file >> file_number;
    index_type number_assignments;
    data_file >> number_assignments;
    data_file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    if (not data_file) {
      std::cerr << err << "Reading error in \"" << data_filename << "\".\n";
      return errcode_reading_data;
    }
    if (running_number != counter) {
      std::cerr << err << "A non-consecutive running-number " << running_number << " in \"" << data_filename << "\".\n";
      return errcode_running_number;
    }
    if (file_number == 0) {
      std::cerr << err << "A file-index of 0 in \"" << data_filename << "\".\n";
      return errcode_file_number_0;
    }
    if (file_number > N) {
      std::cerr << err << "A file-index " << file_number << " in \""
        << data_filename << "\", which is greater than the parameter " << N
        << ".\n";
      return errcode_file_number_N;
    }

    std::stringstream pa_name;
    pa_name << dirname << "/" << file_number;
    std::ifstream pa(pa_name.str().c_str());
    pa.ignore(std::numeric_limits<std::streamsize>::max(),' ');
    literalvector_type P(number_assignments);
    for (index_type i = 0; i < number_assignments; ++i) pa >> P[i];
    if (not pa) {
      std::cerr << err << "Reading error for \"" << pa_name << "\".\n";
      return errcode_reading_pa;
    }
    const indexvector_type& I = T[file_number-1];
    const index_type number_decisions = I.size();
    for (index_type i = 0; i < number_decisions; ++i) {
      const index_type j = I[i];
      if (j >= number_assignments) {
        std::cerr << err << "For running number " << running_number <<
          " in file " << data_filename << "\n the index " << j <<
          " is more than the number " << number_assignments <<
          " of assignments.\n";
        return errcode_index;
      }
      std::cout << P[j];
      if (i < number_decisions-1) std::cout << " ";
    }
    std::cout << "\n";
  }
}
