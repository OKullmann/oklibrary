// Oliver Kullmann, 7.10.2002 (Swansea)
/* Copyright 2002 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Filter.cpp
  \brief Auxiliary application
  \deprecated First one needs to find out what it's doing.
*/

#include <iostream>
#include <string>
#include <map>

#include <OKlib/General/StringHandling.hpp>

using namespace std;
using namespace StringHandling;

int main() {
  map<string,unsigned int> Count;
  string var;
  while (cin >> var) {
    string assign;
    cin >> assign;
    if (assign == "0" or var.find("ident") != 0)
      continue;
    deleteHead_var(var, 5);
    string::size_type ch = var.find("choice");
    var.erase(ch, 6);
    string choice = var.substr(ch, string::npos);
    var.erase(ch);
    cout << var << " " << choice << "\n";
    ++Count[choice];
  }
  cout << "\n";
  for (map<string,unsigned int>::iterator i = Count.begin(); i != Count.end(); ++i)
    cout << i -> first << ": " << i -> second << "\n";
}
