// Oliver Kullmann, 10.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Parallelism/demo_Example_12_1.cpp
  \brief Example program according to [C++ Cookbook], problem 12.1
  
  The computation of functor OKlib::Parallelism::Counting_and_writing is
  to count and to write the current count into a file. Running
  \code
demo_Example_12_1 1000000 3000000
  \endcode
  runs OKlib::Parallelism::Counting_and_writing in the main thread with
  loop-bound 1000000 and output-file "OUTPUT_main" (see below), and in the
  child thread with loop-bound 3000000 and output-file "OUTPUT_child"
  (see below).

  \todo The output-file should go somewhere in system_directories, like
  a sub-directory "data"; build-system support is needed here.

  \todo It seems that only because function OKlib::Parallelism::write_number
  is an inline function the program works? (If write_number would be a
  function, then we would have conflicting calls?? Or not --- there is
  no static data?) (If inline were needed, then it should be a functor-class.)

  \todo What does yield really accomplish? Just giving time to the child
  (which was already start), or starting the child(ren)?

  \todo Later this should go the a sub-directory demos. Or to a sub-directory
  "examples" ? (It is not a demo for our library, but for another library?!)

*/

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

namespace OKlib {
  namespace Parallelism {

    //! Write an integer to the beginning of a filestream

    inline void write_number(const int n, std::ofstream& f) {
      f.seekp(0, std::ios::beg);
      f << n << std::endl;
      return;
    }

    /*!
      \class Counting_and_writing
      \brief Constructed with a total count and an output-filestream.

      Member-function operator() then counts up to the total count,
      writing each iteration value to the filestream via write_number
    */

    class Counting_and_writing {
      const unsigned int count;
      std::ofstream& file;
    public :
      Counting_and_writing(const unsigned int count, std::ofstream& file) : count(count), file(file) {}
      void operator()() const {
        for (unsigned int i = 0; i != count; ++i) {
          write_number(i, file);
        }
      }
    };

  }
}


int main(const int argc, const char* const argv[]) {

  if (argc != 3) {
    std::cerr << "2 arguments required (number of iterations in the main and child thread)\n";
    return 1;
  }

  const unsigned int iterations_main(boost::lexical_cast<unsigned int>(argv[1]));
  const unsigned int iterations_child(boost::lexical_cast<unsigned int>(argv[2]));

  const char* const main_name = "OUTPUT_main";
  std::ofstream output_file_main(main_name);
  const boost::filesystem::path main_path(main_name);
  assert(boost::filesystem::exists(main_path));

  const char* const child_name = "OUTPUT_child";
  std::ofstream output_file_child(child_name);
  const boost::filesystem::path child_path(child_name);
  assert(boost::filesystem::exists(child_path));

  OKlib::Parallelism::Counting_and_writing action_main(iterations_main, output_file_main);
  OKlib::Parallelism::Counting_and_writing action_child(iterations_child, output_file_child);

  boost::thread child(action_child);

  boost::thread::yield(); // this starts the child (by giving time to the child)

  action_main();

  child.join(); // waiting until child finishes (otherwise as soon as action_main is accomplished the program is exit)
}

