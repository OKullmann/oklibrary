// Oliver Kullmann, 5.5.2005 (Swansea)

/*!
  \file Experimentation/Competition/Analyse.cpp
  \brief Reading in a data file, computing the lexicographical order of solvers.
  Superseded by ComputeLexicographicalEvaluation (which handles directly
  the original data file).
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <stdexcept>
#include <string>
#include <iterator>
#include <cstdlib>
#include <fstream>
#include <cassert>

#include <boost/lexical_cast.hpp>

namespace Analyse {

  typedef unsigned int CountSolved;
  typedef unsigned int NumberSolver;
  typedef double Time;

  struct FormatError : std::runtime_error {
    FormatError(const std::string& m) : std::runtime_error(m) {}
  };

  struct mean_time {
    void set_NaN() { is_NaN = true; }
    void set_time(const Time t) { is_NaN = false; time = t; }
    bool is_NaN;
    Time time;
  };

  std::istream& operator >>(std::istream& in, mean_time& t) {
    if (not in)
      throw FormatError("operator >>(std::istream& in, mean_time& t): stream unreadable from the beginning");
    std::string time;
    in >> time;
    if (not in)
      throw FormatError("operator >>(std::istream& in, mean_time& t): can't read time");
    if (time == "NaN")
      t.set_NaN();
    else {
      try {
        t.set_time(boost::lexical_cast<Time>(time));
      }
      catch(const boost::bad_lexical_cast& e) {
        throw FormatError(std::string("operator >>(std::istream& in, mean_time& t): can't convert time; message from boost::bad_lexical_cast is\n") + e.what());
      }
    }
    return in;
  }

  std::ostream& operator <<(std::ostream& out, const mean_time& t) {
    if (t.is_NaN)
      return out << "NaN";
    else
      return out << t.time;
  }

  bool operator <(const mean_time& a, const mean_time& b) {
    if (b.is_NaN)
      return false;
    else
      if (a.is_NaN)
        return true;
      else
        return b.time < a.time;
  }

  struct SolverResult {
    NumberSolver num_solver;
    typedef std::vector<CountSolved> vector_numbers;
    typedef std::vector<mean_time> vector_times;
    unsigned int ncolumns;
    vector_numbers solved;
    vector_times times;
  };

  bool operator <(const SolverResult& a, const SolverResult& b) {
    const bool a_l_b = std::lexicographical_compare(a.solved.rbegin(), a.solved.rend(), b.solved.rbegin(), b.solved.rend());
    if (a_l_b) return true;
    const bool b_l_a = std::lexicographical_compare(b.solved.rbegin(), b.solved.rend(), a.solved.rbegin(), a.solved.rend());
    if (b_l_a) return false;
    return std::lexicographical_compare(a.times.rbegin(), a.times.rend(), b.times.rbegin(), b.times.rend());
  }

  std::ostream& operator <<(std::ostream& out, const SolverResult& r) {
    if (not out)
      throw FormatError("operator >>(std::ostream& in, SolverResult& R): stream unwritable from the beginning");
    out << r.num_solver << "\n";
    std::copy(r.solved.begin(), r.solved.end(), std::ostream_iterator<CountSolved>(out, " "));
    out << "\n";
    std::copy(r.times.begin(), r.times.end(), std::ostream_iterator<mean_time>(out, " "));
    out << "\n";
    return out;
  }

  typedef std::set<SolverResult> SolverResults;

  std::istream& operator >> (std::istream& in, SolverResults& R) {
    if (not in)
      throw FormatError("operator >>(std::istream& in, SolverResults& R): stream unreadable from the beginning");
    unsigned int number_results;
    unsigned int number_columns;
    in >> number_results >> number_columns;
    if  (not in)
      throw FormatError("operator >>(std::istream& in, SolverResults& R): can't read number_results, number_columns");

    for (unsigned int i = 0; i < number_results; ++i) {
      SolverResult r;
      in >> r.num_solver;
      if  (not in)
          throw FormatError(std::string("operator >>(std::istream& in, SolverResults& R): can't read solver number for result number ") + boost::lexical_cast<std::string>(i + 1));
      r.solved.reserve(number_columns);
      for (unsigned int j = 0; j < number_columns; ++j) {
        CountSolved c;
        in >> c;
        if  (not in)
          throw FormatError(std::string("operator >>(std::istream& in, SolverResults& R): can't read number of instances solved for result number ") + boost::lexical_cast<std::string>(i + 1) + " and column number " + boost::lexical_cast<std::string>(j + 1));
        r.solved.push_back(c);
      }
      r.times.reserve(number_columns);
      for (unsigned int j = 0; j < number_columns; ++j) {
        mean_time t;
        in >> t;
        if  (not in)
          throw FormatError(std::string("operator >>(std::istream& in, SolverResults& R): can't read mean time for result number ") + boost::lexical_cast<std::string>(i + 1) + " and column number " + boost::lexical_cast<std::string>(j + 1));
        r.times.push_back(t);
      }
      R.insert(r);
      assert(R.size() == i+1);
    }
    return in;
  }

  std::ostream& operator <<(std::ostream& out, const SolverResults& R) {
    std::copy(R.rbegin(), R.rend(), std::ostream_iterator<SolverResult>(out, "\n"));
    return out;
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    std::cerr << "ERROR[Analyse]: Exactly one argument expected, the name of the file with the data.\n";
    return EXIT_FAILURE;
  }
  std::ifstream file(argv[1]);
  Analyse::SolverResults R;
  file >> R;
  std::cout << R;
}
