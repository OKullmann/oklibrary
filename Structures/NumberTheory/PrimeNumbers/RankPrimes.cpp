// Oliver Kullmann, 9.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Structures/NumberTheory/PrimeNumbers/RankPrimes.cpp
  \brief Finding the index of prime numbers

  Reads an integer from standard input, and either ranks or unranks it:
  <ul>
   <li> Ranking means to compute the index i of the smallest prime number p
   with p >= p_in, where p_in is the input. For example for
   input p_in = 8 the output is i = 5, and the same output is obtained
   for 8 <= p_in <= 11. </li>
   <li> Unranking means to compute the prime number p with index i, where
   i is the input. For example for input i = 3 the output is 5, while for
   i = 5 we get 11. </li>
  </ul>
  Two parameters are used:
  <ol>
   <li> Table data:
    <ul>
     <li> A datafile containing pairs "index prime", so that computation
     becomes faster by jumping directly to the nearest prime resp. index.
     <li> With a non-integer entry in index position one can indicate the the
     end of the data section. </li>
    </ul>
   </li>
   <li> Either the string "rank" or "unrank". </li>
  </ol>

*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>

#include <gmpxx.h>

namespace {

  const int errcode_parameter = 1,
    errcode_file = 2,
    errcode_mode = 3,
    errcode_index = 4,
    errcode_prime1 = 5,
    errcode_prime2 = 6,
    errcode_read = 7,
    errcode_stdin = 8;

  const std::string program = "RankPrimes";
  const std::string err = "ERROR[" + program + "]: ";
  const std::string err_pre = "ERROR[" + program + "] while preprocessing: ";

  enum Mode {rank, unrank};
  typedef std::map<mpz_class, mpz_class> table_type;
  typedef table_type::const_iterator iterator_type;

}

int main(const int argc, const char* const argv[]) {

  if (argc != 3) {
    std::cerr << err << "Exactly two arguments are needed,\n"
    "the path to the file with stored pairs \"index prime\",\n"
      "and one of the strings \"rank\" or \"unrank\".\n";
    return(errcode_parameter);
  }

  std::string datafile_path(argv[1]);
  std::ifstream datafile(datafile_path.c_str());
  if (not datafile) {
    std::cerr << err << "Error when opening file " <<
      datafile_path << ".\n";
    return(errcode_file);
  }

  const std::string mode_string(argv[2]);
  if (mode_string != "rank" and mode_string != "unrank") {
    std::cerr << err << "The second parameter must be either \"rank\" or \"unrank\".\n";
    return(errcode_mode);
  }
  const Mode mode = (mode_string == "rank") ? rank : unrank;

  table_type table;
  for (;;) {
    mpz_class index, prime;
    datafile >> index;
    if (not datafile) break;
    if (index < 1) {
      std::cerr << err_pre << "Indices are natural numbers >= 1, but found " << index << ".\n";
      return(errcode_index);
    }
    datafile >> prime;
    if (not datafile) {
      std::cerr << err_pre << "Error while attempting to read the prime number\n"
        " belonging to index " << index << " in file\n" << datafile_path << ".\n";
    return(errcode_read);
    }
    if (prime < 2) {
      std::cerr << err_pre << "Prime numbers are natural numbers >= 2, but found " << prime << ".\n";
      return(errcode_prime1);
    }
    if (mpz_probab_prime_p(prime.get_mpz_t(), 10) == 0) {
      std::cerr << err_pre << "Number " << prime << " is not a prime number.\n";
      return(errcode_prime2);
    }
    switch (mode) {
    case rank : table.insert(std::make_pair(prime,index)); break;
    case unrank : table.insert(std::make_pair(index,prime)); break;
    }
  }

  switch (mode) {

  case rank: {
    mpz_class p_in, i = 1, p = 2;
    std::cin >> p_in;
    if (not std::cin) {
      std::cerr << err << "Error when attempting to read an integer from standard input.\n";
      return(errcode_stdin);
    }
    if (not table.empty()) {
      iterator_type it = table.lower_bound(p_in);
      if (it != table.begin()) {
        if (it != table.end()) {
          p = it -> first;
          if (p == p_in) i = it -> second;
          else {
            --it; p = it -> first; i = it -> second;
          }
        }
        else {
          --it; p = it -> first; i = it -> second;
        }
      }
    }
    for (; p < p_in;
         ++i, mpz_nextprime(p.get_mpz_t(), p.get_mpz_t())) {}
    std::cout << i << "\n"; break;
  }

  case unrank: {
    mpz_class i, j = 1, p = 2;
    std::cin >> i;
    if (not std::cin) {
      std::cerr << err << "Error when attempting to read an integer from standard input.\n";
      return(errcode_stdin);
    }
    if (not table.empty()) {
      iterator_type it = table.lower_bound(i);
      if (it != table.begin()) {
        if (it != table.end()) {
          j = it -> first;
          if (j == i) p = it -> second;
          else {
            --it; j = it -> first; p = it -> second;
          }
        }
        else {
          --it; j = it -> first; p = it -> second;
        }
      }
    }
    for (; j < i;
         ++j, mpz_nextprime(p.get_mpz_t(), p.get_mpz_t())) {}
    std::cout << p << "\n"; break;
  }
  }
  
}
