// Oliver Kullmann, 6.10.2002 (Swansea)
/* Copyright 2002 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/LinInequal.hpp
  \brief Tools for encoding linear inequalities as CNF
*/

#ifndef LINinEQUALWAECHTER
#define LINinEQUALWAECHTER

#include <cmath>
#include <string>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <istream>
#include <map>
#include <utility>
#include <set>

#include <OKlib/General/StringHandling.hpp>

namespace LinInequal {

  inline unsigned int bin_length(unsigned int x) {
    if (x == 0)
      return 1;
    else
      return (unsigned int) (floor(log(x) / log(2)) + 1);
  }
  
  inline std::string P(const std::string& v, unsigned int i) {
    return v + StringHandling::toString(i) + " ";
  }
  inline std::string N(const std::string& v, unsigned int i) {
    return "-" + v + StringHandling::toString(i) + " ";
  }
  inline std::string P(const std::string& v){
    return v + " ";
  }
  inline std::string N(const std::string& v) {
    return "-" + v + " ";
  }
  inline std::string E() {
    return "0\n";
  }
  
  
  inline void AddBin(
                     const std::string& A,
                     const unsigned int length_A,
                     const std::string& B,
                     const unsigned int length_B,
                     const std::string out,
                     const std::string aux,
                     std::ostream& os)
  {
    // A, B correspond to natural numbers in binary representation;
    // length_A, length_B >= 1
    os << P(out,1) << N(A,1) << P(B,1) << E();
    os << P(out,1) << P(A,1) << N(B,1) << E();
    os << N(out,1) << P(A,1) << P(B,1) << E();
    os << N(out,1) << N(A,1) << N(B,1) << E();
    // out1 is true iff exactly one of A1,B1 is true
    
    os << P(aux,1) << N(A,1) << N(B,1) << E();
    os << N(aux,1) << P(A,1) << E();
    os << N(aux,1) << P(B,1) << E();
    // aux1 is true iff A1,B1 are true
    
    const std::string& C = (length_A <= length_B) ? B : A;
    const unsigned int min = std::min(length_A, length_B);
    const unsigned int max = std::max(length_A, length_B);
    
    for (unsigned int i = 2; i <= min; ++i) {
      os << P(aux,i) << N(A,i) << N(B,i) << E();
      os << P(aux,i) << N(A,i) << N(aux,i-1) << E();
      os << P(aux,i) << N(B,i) << N(aux,i-1) << E();
      os << N(aux,i) << P(A,i) << P(B,i) << E();
      os << N(aux,i) << P(A,i) << P(aux,i-1) << E();
      os << N(aux,i) << P(B,i) << P(aux,i-1) << E();
      // auxi is true iff at least two of Ai,Bi,aux(i-1) are true
    }
    
    for (unsigned int i = min + 1; i <= max; ++i) {
      os << P(aux,i) << N(C,i) << N(aux,i-1) << E();
      os << N(aux,i) << P(C,i) << E();
      os << N(aux,i) << P(aux,i-1) << E();
      // auxi is true iff Ci,aux(i-1) are true
    }
    
    for (unsigned int i = 2; i <= min; ++i) {
      os << P(out,i) << N(A,i) << P(B,i) << P(aux,i-1) << E();
      os << P(out,i) << N(B,i) << P(A,i) << P(aux,i-1) << E();
      os << P(out,i) << N(aux,i-1) << P(A,i) << P(B,i) << E();
      os << P(out,i) << N(A,i) << N(B,i) << N(aux,i-1) << E();
      os << N(out,i) << P(A,i) << N(B,i) << N(aux,i-1) << E();
      os << N(out,i) << P(B,i) << N(A,i) << N(aux,i-1) << E();
      os << N(out,i) << P(aux,i-1) << N(A,i) << N(B,i) << E();
      os << N(out,i) << P(A,i) << P(B,i) << P(aux,i-1) << E();
      // outi is true iff exactly one or three of Ai,Bi,aux(i-1) are true
    }
    
    for (unsigned int i = min + 1; i <= max; ++i) {
      os <<  P(out,i) << P(C,i) << N(aux,i-1) << E();
      os <<  P(out,i) << N(C,i) << P(aux,i-1) << E();
      os <<  N(out,i) << P(C,i) << P(aux,i-1) << E();
      os <<  P(out,i) << N(C,i) << N(aux,i-1) << E();
      // outi is true iff exactly on of Ci,aux(i-1) is true
    }
    
    os << P(out,max+1) << N(aux,max) << E();
    os << N(out,max+1) << P(aux,max) << E();
    // out(max+1) is true iff aux(max) is true
  }
  
  
  template <typename It> // random access iterator to strings
  inline void AddVar(
                     const It V_b,
                     const It V_e,
                     const std::string& out,
                     const std::string aux,
                     std::ostream& os)
  {
    unsigned int m = V_e - V_b;
    
    switch (m) {
    case 0 : return;
    case 1 :
      os << P(out,1) << N(*V_b) << E();
      os << N(out,1) << P(*V_b) << E();
      return;
    case 2 :
      os << P(out,1) << N(*V_b) << P(*(V_b+1)) << E();
      os << P(out,1) << P(*V_b) << N(*(V_b+1)) << E();
      os << N(out,1) << P(*V_b) << P(*(V_b+1)) << E();
      os << N(out,1) << N(*V_b) << N(*(V_b+1)) << E();
      os << P(out,2) << N(*V_b) << N(*(V_b+1)) << E();
      os << N(out,2) << P(*V_b) << E();
      os << N(out,2) << P(*(V_b+1)) << E();
      return;
    case 3 :
      os << P(out,1) << N(*V_b) << P(*(V_b+1)) << P(*(V_b+2)) << E();
      os << P(out,1) << P(*V_b) << N(*(V_b+1)) << P(*(V_b+2)) << E();
      os << P(out,1) << P(*V_b) << P(*(V_b+1)) << N(*(V_b+2)) << E();
      os << P(out,1) << N(*V_b) << N(*(V_b+1)) << N(*(V_b+2)) << E();
      os << N(out,1) << P(*V_b) << N(*(V_b+1)) << N(*(V_b+2)) << E();
      os << N(out,1) << N(*V_b) << P(*(V_b+1)) << N(*(V_b+2)) << E();
      os << N(out,1) << N(*V_b) << N(*(V_b+1)) << P(*(V_b+2)) << E();
      os << N(out,1) << P(*V_b) << P(*(V_b+1)) << P(*(V_b+2)) << E();
      os << P(out,2) << N(*V_b) << N(*(V_b+1)) << P(*(V_b+2)) << E();
      os << P(out,2) << N(*V_b) << P(*(V_b+1)) << N(*(V_b+2)) << E();
      os << P(out,2) << P(*V_b) << N(*(V_b+1)) << N(*(V_b+2)) << E();
      os << P(out,2) << N(*V_b) << N(*(V_b+1)) << N(*(V_b+2)) << E();
      os << N(out,2) << P(*V_b) << P(*(V_b+1)) << N(*(V_b+2)) << E();
      os << N(out,2) << P(*V_b) << N(*(V_b+1)) << P(*(V_b+2)) << E();
      os << N(out,2) << N(*V_b) << P(*(V_b+1)) << P(*(V_b+2)) << E();
      os << N(out,2) << P(*V_b) << P(*(V_b+1)) << P(*(V_b+2)) << E();
      return;
    }
    
    const unsigned int mA = m / 2;
    const unsigned int mB = m - mA;
    
    const std::string PartA = aux + "A";
    const std::string PartB = aux + "B";
    AddVar(V_b, V_b + mA, PartA, aux + "HA", os);
    AddVar(V_b + mA, V_e, PartB, aux + "HB", os);
    AddBin(PartA, bin_length(mA), PartB, bin_length(mB), out, aux + "U", os);
  }
  
  
  template <typename It>
  inline void CompVar(
                      const It V_b,
                      const It V_e,
                      unsigned int bound,
                      std::ostream& os)
  {
    // It random access iterator, *It must be a string;
    std::vector<bool> bits;
    const unsigned int bl = bin_length(bound);
    bits.reserve(bl);
    do {
      bits.push_back(bound % 2);
      bound /= 2;
    } while (bound != 0);
    
    const typename std::iterator_traits<It>::difference_type m = V_e - V_b;
    if (m < bl)
      return;
    else if (m > bl)
      for (It i = V_b + bl; i != V_e; ++i)
	os << N(*i) << E();
    // now m = bl
    It v = V_b;
    for (unsigned int i = 0; i < bl; ++i, ++v) {
      if (bits[i])
	continue;
      os << N(*v);
      It v2 = v+1;
      for (unsigned int j = i+1; j < bl; ++j, ++v2)
	if (! bits[j])
	  continue;
	else
	  os << N(*v2);
      os << E();
    }
  }


  template <typename It>
  void count_bound(
                   const It V_b,
                   const It V_e,
                   unsigned int bound,
                   std::ostream& os,
                   const std::string& prefix)
  {
    AddVar(V_b, V_e, prefix + "S", prefix + "H", os);
    const unsigned int bl = bin_length(V_e - V_b);
    std::vector<std::string> S;
    S.reserve(bl);
    for (unsigned int i = 1; i <= bl; ++i)
      S.push_back(P(prefix + "S", i));
    CompVar(S.begin(), S.end(), bound, os);
  }
  
  
  typedef std::vector<unsigned int> ui_vec;

  inline std::string AV(const std::string& ident, const unsigned int choice) {
    return "ident" + ident + "choice" + StringHandling::toString(choice);
  }

  void Assignment(
                  const ui_vec& C,
                  std::istream& in,
                  const unsigned int level,
                  std::ostream& out)
  {
    using namespace std;

    const unsigned int number_choices = C.size();
    vector< vector<string> > Chosen(number_choices);
    
    set<string> Ident;
    map<pair<string, unsigned int>, unsigned int> Pref;
    {
      string ident;
      while (in >> ident) {
	Ident.insert(ident);
	unsigned int choice; unsigned int preference;
	in >> choice >> preference;
	Pref.insert(make_pair(make_pair(ident, choice), preference));
      }
    }

    for (set<string>::const_iterator i = Ident.begin(); i != Ident.end(); ++i) {
      const string& ident = *i;
      for (unsigned int choice = 1; choice <= number_choices; ++choice) {
	const unsigned int pref = Pref.find(make_pair(ident, choice)) -> second;
	if (pref > level)
	  continue;
	const string V = AV(ident, choice);
	out << P(V);
	Chosen[choice - 1].push_back(V);
      }
      out << E();
    }

    for (unsigned int i = 0; i < number_choices; ++i)
      count_bound(Chosen[i].begin(), Chosen[i].end(), C[i], out,
                  "R" + StringHandling::toString(i+1));
  }
}

#endif
