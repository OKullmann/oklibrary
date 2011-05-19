// Oliver Kullmann, 18.5.2011 (Swansea)

/*!
  \file Interfaces/InputOutput/ManipParam.cpp
  \brief Replacing the parameter-line in a DIMACS file by new values

    ManipParam file new-n new-c

  An error occurs when the new line is longer than the old line.
  Otherwise the old line is overwritten with the new line, padded with spaces
  at the end (if needed).

  \todo Better parameter-handling
  <ul>
   <li> We need also the ability to take over either n or c (or both). </li>
  </ul>

*/

#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include <cassert>

namespace {

  enum {
    errcode_parameter = 1,
    errcode_open = 2,
    errcode_nopline = 3,
    errcode_unfinishedpline = 4,
    errcode_nospace = 5,
    errcode_write = 6
  };
  const std::string program = "ManipParam";
  const std::string err = "ERROR[" + program + "]: ";
  const std::string version = "0.0.5";
}

int main(const int argc, const char* const argv[]) {

  if (argc != 4) {
    std::cerr << err << "Exactly three parameters are needed:\n"
      " The filename, and the strings for n and c.\n";
    return errcode_parameter;
  }

  const std::string filename = argv[1];
  const std::string n = argv[2];
  const std::string c = argv[3];

  std::fstream f(filename.c_str(), std::ios::in | std::ios::out);
  if (not f) {
    std::cerr << err << "Can not open file \"" << filename <<
      "\" (for reading and writing).\n";
    return errcode_open;
  }
  std::string new_parameter_line = "p cnf " + n + " " + c;
  typedef std::string::size_type size_type;
  const size_type new_length = new_parameter_line.size();

  const std::streamsize max = std::numeric_limits<std::streamsize>::max();
    while (f and f.peek() != 'p') f.ignore(max,'\n');
  if (not f) {
    std::cerr << err << "No parameter line in file \"" << filename << "\".\n";
    return errcode_nopline;
  }
  const std::ios::pos_type pos_parameter_line = f.tellg();
  f.ignore(max,'\n');
  if (f.eof()) {
    std::cerr << err << "Unfinished parameter line in file \"" << filename << "\".\n";
    return errcode_unfinishedpline;
  }
  const std::ios::pos_type pos_after_parameter_line = f.tellg();
  assert(pos_after_parameter_line > pos_parameter_line);
  const size_type length = (pos_after_parameter_line - pos_parameter_line) - 1;
  assert(length >= 1);
  if (length < new_length) {
    std::cerr << err << "The length of the parameter line in file \"" <<
      filename << "\" is " << length << ",\n which is less then the length "
      << new_length << " of the new parameter line.\n";
    return errcode_nospace;
  }
  new_parameter_line.append(length - new_length, ' ');
  f.seekp(pos_parameter_line);
  f << new_parameter_line;
    
  if (not f) {
    std::cerr << err << "Write error with file \"" << filename << "\".\n";
    return errcode_write;
  }
}
