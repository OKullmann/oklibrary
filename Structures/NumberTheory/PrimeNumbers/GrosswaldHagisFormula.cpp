// Oliver Kullmann, 8.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Structures/NumberTheory/PrimeNumbers/GrosswaldHagisFormula.cpp
  \brief Computing the coefficients C_k according to [Grosswald, Hagis, 1979]

  <ul>
   <li> Reads inputs k, max_p, decimal_digits from standard input (last is
   optional). </li>
   <li> The first input can also be of the form "k1,k2" (no spaces), meaning
   all k1 <= k <= k2. </li>
   <li> From the number of decimal digits the number of binary digits is
   computed by multiplication with 4, and the default value for the number of
   decimal digits is 100. </li>
   <li> Computes C_gh_hp(k,max_p,decimal_digits) according to
   ComputerAlgebra/RamseyTheory/Lisp/GreenTao/Asymptotics.mac. </li>
  </ul>

*/

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <gmpxx.h>

namespace {

  enum Error_code { errcode_noerror = 0,
    errcode_parameter = 1,
    errcode_kl2 = 2,
    errcode_kuint = 3,
    errcode_kread = 4,
    errcode_kcomp = 5,
    errcode_pread = 6,
    errcode_ddl1 = 7,
    errcode_ddulong = 8,
    errcode_ddread = 9
  };

  const std::string program = "GrosswaldHagisFormula";
  const std::string version = "0.1.1";
  const std::string err = "ERROR[" + program + "]: ";

  const unsigned long default_binary_digits = 400;
  const unsigned long output_precision = 20;

  typedef std::pair<unsigned int, Error_code> Reading_result;

  Reading_result reading_length(const std::string& in) {
    try {
      mpz_class length(in);
      if (length < 2) return Reading_result(0,errcode_kl2);
      if (not length.fits_uint_p()) return Reading_result(0,errcode_kuint);
      return Reading_result(length.get_ui(),errcode_noerror);
    }
    catch (...) { return Reading_result(0,errcode_kread); };
  }

  // compute p^(k-2)/(p-1)^(k-1):
  inline mpq_class basic_factor(const unsigned int k, const mpz_class& p) {
    mpz_class temp(p);
    mpz_pow_ui(temp.get_mpz_t(), temp.get_mpz_t(), k-2);
    mpq_class factor(temp);
    temp = p-1;
    mpz_pow_ui(temp.get_mpz_t(), temp.get_mpz_t(), k-1);
    factor /= temp;
    return factor;
  }

}

int main(const int argc, const char* const argv[]) {

  // READING

  if (argc < 3) {
    std::cerr << err << "At least two arguments are needed,\n"
    "the length of the arithmetic progression and the maximal prime number.\n";
    return errcode_parameter;
  }

  unsigned int k_first, k_last;
  {
    const std::string length_argument = argv[1];
    const std::string::size_type pos_comma = length_argument.find(',');
    if (pos_comma == std::string::npos) {
      const Reading_result res = reading_length(length_argument);
      if (res.second != 0) {
        switch (res.second) {
        case errcode_kl2 :
          std::cerr << err << "The length of the arithmetic progression must be at least 2.\n"; break;
        case errcode_kuint :
          std::cerr << err << "The length of the arithmetic progression must fit into unsigned int.\n"; break;
        default :
          std::cerr << err << "Error when reading the length of the arithmetic progression.\n";
        }
        return res.second;
      }
      k_first = k_last = res.first;
    }
    else {
      Reading_result res = reading_length(length_argument.substr(0,pos_comma));
      if (res.second != 0) {
        switch (res.second) {
        case errcode_kl2 :
          std::cerr << err << "The start-length of the arithmetic progressions must be at least 2.\n"; break;
        case errcode_kuint :
          std::cerr << err << "The lengths of arithmetic progressions must fit into unsigned int.\n"; break;
        default :
          std::cerr << err << "Error when reading the start-length of the arithmetic progression.\n";
        }
        return res.second;
      }
      k_first = res.first;
      res = reading_length(length_argument.substr(pos_comma+1));
      if (res.second != 0) {
        switch (res.second) {
        case errcode_kl2 :
          std::cerr << err << "The end-length of the arithmetic progressions must be at least 2.\n"; break;
        case errcode_kuint :
          std::cerr << err << "The lengths of arithmetic progressions must fit into unsigned int.\n"; break;
        default :
          std::cerr << err << "Error when reading the end-length of the arithmetic progression.\n";
        }
        return res.second;
      }
      if (res.first < k_first) {
        std::cerr << err << "The end-length of arithmetic progressions must not be smaller than the start-length.\n";
        return errcode_kcomp;
      }
      k_last = res.first;
    }
  }
  const unsigned int number_lengths = k_last - k_first + 1;

  mpz_class temp;

  try {
    temp = mpz_class(argv[2]);
  }
  catch (std::invalid_argument) {
    std::cerr << err << "Error when reading the maximal prime number.\n";
    return errcode_pread;
  }
  const mpz_class max_p(temp);

  if (argc > 3) {
    try {
      temp = mpz_class(argv[3]);
      if (temp < 1) {
        std::cerr << err << "The number of decimal digits must be at least 1.\n";
        return errcode_ddl1;
      }
      temp *= 4;
      if (not temp.fits_ulong_p()) {
        std::cerr << err << "The number of binary digits must fit into unsigned long.\n";
        return errcode_ddulong;
      }
    }
    catch (std::invalid_argument) {
      std::cerr << err << "Error when reading the number of decimal digits.\n";
      return errcode_ddread;
    }
  }
  else temp = default_binary_digits;
  const unsigned long binary_digits(temp.get_ui());

  // COMPUTING

  std::vector<mpq_class> C_gh_fin(number_lengths,1);
  mpf_set_default_prec(binary_digits);
  std::vector<mpf_class> C_gh_inf(number_lengths,1);
  mpz_class p(2);
  for (; p <= max_p; mpz_nextprime(p.get_mpz_t(), p.get_mpz_t())) {
    mpq_class factor(basic_factor(k_first,p));
    const mpq_class ifac(mpq_class(p)/(p-1));
    for (unsigned int k = k_first; k <= k_last; ++k, factor *= ifac)
      if (p <= k) C_gh_fin[k-k_first] *= factor;
      else C_gh_inf[k-k_first] *= factor * (p-k+1);
  }
  
  // OUTPUT

  std::cout.precision(output_precision);
  std::cout << "Precision in bits: " << mpf_get_prec(C_gh_inf[0].get_mpf_t()) << "\n";
  std::cout << "The first prime number not taken into account: " << p << "\n\n";
  for (unsigned int k = k_first; k <= k_last; ++k) {
    const mpf_class C_gh_fin_f(C_gh_fin[k-k_first]);
    std::cout << "C_" << k << " = " << C_gh_fin_f * C_gh_inf[k-k_first] << "\n";
    std::cout << "Finite and infinite part: " << C_gh_fin_f << ", " << C_gh_inf[k-k_first] << "\n";
    std::cout << "1 - first remaining factor: " << mpf_class(1-basic_factor(k,p)*(p-k+1)) << "\n";
  }
}

