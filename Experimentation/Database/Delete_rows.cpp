// Oliver Kullmann, 1.7.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// Call:

// Delete_rows deletion_table subtraction_table test_field

/* deletes all rows from deletion_table where the field test_field
is in subtraction_table;
*/

#include <string>
#include <algorithm>
#include <memory>
#include <iostream>

#include <OKlib/General/DatabaseHandler01.hpp>

using namespace std;

const string database = "OKRandGen";

using namespace DatabaseHandler01;

class eliminate_row {
public :
  eliminate_row(const Connection& C, const string& t, const string& f)
    : Comm(new Command(C)), table(t), field(f) {}
  eliminate_row(const eliminate_row& er)
    : Comm(er.Comm), table(er.table), field(er.field) {}
  void operator() (const string& value) {
    Comm -> issue("delete from " + table + " where " + field + " = " + value + ";");
  }
private :
  mutable auto_ptr<Command> Comm; // ich glaube, es ist ein Fehler von g++
  // (3.0.4), der keine nicht-konstanten Kopier-Konstruktoren zulaesst;
  const string& table;
  const string& field;
};

int main(const int argc, const char* const argv[]) {

  if (argc < 4) {
    cerr << "ERROR[Delete_rows] Three arguments are needed (the table from which rows are to be deleted, the table containing the rows to be eliminated, and the test field).\n";
    return 1;
  }
  const string deletion_table(argv[1]);
  const string subtraction_table(argv[2]);
  const string test_field(argv[3]);

  Connection Conn(database);

  Select S(Conn);
  S.issue("select " + test_field + " from " + subtraction_table + ";");
  const int test_column = S.get_Attribute(test_field).column;
  
  for_each(RowIterator(S, test_column), RowIterator(S, test_column) + S.number_rows(), eliminate_row(Conn, deletion_table, test_field));
}

