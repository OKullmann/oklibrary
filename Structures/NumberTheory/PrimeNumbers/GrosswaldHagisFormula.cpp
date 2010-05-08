// Oliver Kullmann, 8.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Structures/NumberTheory/PrimeNumbers/GrosswaldHagisFormula.cpp
  \brief Computing the C_k according to [Grosswald, Hagis, 1979]

  Reads inputs k, max_p, decimal_digits from standard input (last is optional).
  Computes C_gh_hp(k,max_p,decimal_digits) according to
  ComputerAlgebra/RamseyTheory/Lisp/GreenTao/Asymptotics.mac.

*/

#include <iostream>
#include <string>
#include <cassert>

#include <gmpxx.h>

namespace {

  enum { errcode_parameter = 1,
    errcode_kl2 = 2,
    errcode_kuint = 3,
    errcode_kread = 4,
    errcode_pread = 5,
    errcode_ddl1 = 6,
    errcode_ddulong = 7,
    errcode_ddread = 8
  };

  const std::string program = "GrosswaldHagisFormula";
  const std::string err = "ERROR[" + program + "]: ";

  const unsigned long default_binary_digits = 400;
  const unsigned long output_precision = 20;

}

int main(const int argc, const char* const argv[]) {

  if (argc < 3) {
    std::cerr << err << "At least two arguments are needed,\n"
    "the length of the arithmetic progression and the maximal prime number.\n";
    return(errcode_parameter);
  }

  mpz_class temp;
  try {
    temp = mpz_class(argv[1]);
    if (temp < 2) {
      std::cerr << err << "The length of the arithmetic progression must be at least 2.\n";
      return(errcode_kl2);
    }
    if (not temp.fits_uint_p()) {
      std::cerr << err << "The length of the arithmetic progression must fit into unsigned int.\n";
      return(errcode_kuint);
    }
  }
  catch (std::invalid_argument) {
    std::cerr << err << "Error when reading the length of the arithmetic progression.\n";
    return(errcode_kread);
  }
  const unsigned int k(temp.get_ui());
  try {
    temp = mpz_class(argv[2]);
  }
  catch (std::invalid_argument) {
    std::cerr << err << "Error when reading the maximal prime number.\n";
    return(errcode_pread);
  }
  const mpz_class max_p(temp);
  if (argc > 3) {
    try {
      temp = mpz_class(argv[3]);
      if (temp < 1) {
        std::cerr << err << "The number of decimal digits must be at least 1.\n";
        return(errcode_ddl1);
      }
      temp *= 4;
      if (not temp.fits_ulong_p()) {
        std::cerr << err << "The number of binary digits must fit into unsigned long.\n";
        return(errcode_ddulong);
      }
    }
    catch (std::invalid_argument) {
      std::cerr << err << "Error when reading the number of decimal digits.\n";
      return(errcode_ddread);
    }
  }
  else temp = default_binary_digits;
  const unsigned long binary_digits(temp.get_ui());

  mpq_class C_gh_fin(1);
  mpz_class p(2);
  for (; p <= k; mpz_nextprime(p.get_mpz_t(), p.get_mpz_t())) {
    mpz_class temp;
    mpz_pow_ui(temp.get_mpz_t(), p.get_mpz_t(), k-2);
    C_gh_fin *= temp;
    temp = p-1;
    mpz_pow_ui(temp.get_mpz_t(), temp.get_mpz_t(), k-1);
    C_gh_fin /= temp;
  }
  assert(p > k);

  mpf_set_default_prec(binary_digits);

  mpf_class C_gh_inf(1);
  // std::cout << "Precision: " << mpf_get_prec(C_gh_inf.get_mpf_t()) << "\n";
  for (; p <= max_p; mpz_nextprime(p.get_mpz_t(), p.get_mpz_t())) {
    mpz_class temp;
    mpz_pow_ui(temp.get_mpz_t(), p.get_mpz_t(), k-2);
    mpq_class factor;
    factor = temp;
    factor *= p-k+1;
    temp = p-1;
    mpz_pow_ui(temp.get_mpz_t(), temp.get_mpz_t(), k-1);
    factor /= temp;
    C_gh_inf *= factor;
  }

  std::cout.precision(output_precision);
  const mpf_class C_gh_fin_f(C_gh_fin);
  // the finite and the infinite part:
  // std::cout << C_gh_fin_f << ", " << C_gh_inf << "\n";
  std::cout << C_gh_fin_f * C_gh_inf << "\n";
}

