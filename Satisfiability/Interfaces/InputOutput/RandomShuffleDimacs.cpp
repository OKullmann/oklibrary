// Matthew Gwynne, 20.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/RandomShuffleDimacs.cpp
  \brief Application to apply a random permutation to the given clause-list.

  <ul>
   <li> Reads a Dimacs clause-list from standard input, and writes to standard 
   output (and standard error). </li>
   <li> A single argument is taken, a positive integer seed (1 if not given). 
   </li>
   <li> The result, given on standard output, is a Dimacs file containing 
   containing the input clause-list after applying a random permutation
   based on the seed. </li>
   <li> The random permutation used is determined by using the given seed
   value to initialise a random number generator (Mersenne twister - mt19937)
   and this is then used as the random number generator input for 
   boost::variate_generator and then boost::random_number_generator, which
   is then passed as the random number generator for C++ std::random_shuffle 
   on the clause-list (in the order it is given in the input). </li>
   <li> See "Random r_1-bases" in 
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp for discussion on the
   exact behaviour of std::random_shuffle and the random number generator. 
   </li>
  </ul>


  \todo Move CLSAdaptorRandomShuffle
  <ul>
   <li> This should likely go to into ClauseSetAdaptors.hpp. </li>
  </ul>


  \todo DONE Add application tests

*/

#include <iostream>
#include <algorithm>
#include <functional>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/random_number_generator.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>


namespace OKlib {
  namespace InputOutput {

    /*!
       \class CLSAdaptorRandomShuffle
       \brief Adaptor which sorts the clause-list given to it.

       Given a random number generator, applies a random
       permutation to the given clause-list using the random
       number generator as input to std::random_shuffle. 
       The result clause-list is output to the given clause-adaptor.

       The random permutation used is determined by using the given
       random number generator and this is then used as the random number 
       generator input for the C++ std::random_shuffle on the clause-list (in 
       the order it is given in the input).

       Note that as the input must all be read before it can be shuffled, 
       there may be a large delay between the input of a clause into this
       adaptor and it's output to the output adaptor.

    */
    template <typename Int = int, class String = std::string, 
              class RandomGenerator =  boost::mt19937,
              class OutputCLSAdaptor = OKlib::InputOutput::CLSAdaptorDIMACSOutput<> >
    class CLSAdaptorRandomShuffle {

    public :
      
      typedef Int int_type;
      typedef String string_type;
      typedef OutputCLSAdaptor output_cls_adaptor_type;
      typedef RawDimacsCLSAdaptor<> stored_cls_adaptor_type;
      typedef stored_cls_adaptor_type::clause_type stored_clause_type;
      typedef RandomGenerator base_generator_type;

    public :

      CLSAdaptorRandomShuffle(base_generator_type& rand_gen,
                                   output_cls_adaptor_type& cls_adaptor_arg) : 
        output_cls_adaptor(cls_adaptor_arg), stored_cls_adaptor(),
        base_rand_gen(rand_gen) {}
 
      void comment(const string_type& s) { output_cls_adaptor.comment(s); }
      void n(const int_type pn) { stored_cls_adaptor.n(pn); }
      void c(const int_type pc) { stored_cls_adaptor.c(pc); }

      void finish() {
        typedef boost::uniform_int<> uniform_distribution_type;
        uniform_distribution_type uniform_distribution(0,std::numeric_limits<int>::max()); // is this correct???
        typedef boost::variate_generator<base_generator_type&, uniform_distribution_type> generator_type;
        generator_type rand_gen(base_rand_gen, uniform_distribution);
        typedef boost::random_number_generator<generator_type> RandomNumberGenerator;
        RandomNumberGenerator rg(rand_gen);
        std::random_shuffle(stored_cls_adaptor.clause_set.begin(), 
                            stored_cls_adaptor.clause_set.end(), rg);
        ListTransfer<output_cls_adaptor_type>(stored_cls_adaptor.clause_set,
                                              output_cls_adaptor, 
                                              "Clauses randomised.");
      }

      void tautological_clause(const int_type t) {
        stored_cls_adaptor.tautological_clause(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        stored_cls_adaptor.clause(r,t);
      }

    private:

      output_cls_adaptor_type output_cls_adaptor;
      stored_cls_adaptor_type stored_cls_adaptor;
      base_generator_type& base_rand_gen;

    };
  }
}


namespace {

  enum { errcode_parameter = 1, errcode_parameter_value = 2 };

  const std::string program = "RandomShuffleDimacs";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.3";

  inline int convert_seed(const char* const arg) {
    int seed;
    try { seed = boost::lexical_cast<int>(arg); }
    catch (boost::bad_lexical_cast&) { return 0; }
    if (seed < 0) return 0;
    else return seed;
  }

  typedef boost::mt19937 base_generator_type;
  base_generator_type base_rand_gen;
  inline void set_random(const int seed) {
    assert(seed >= 1);
    base_rand_gen.seed(seed);
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc > 2) {
    std::cerr << err << "At most one arguments is allowed "
      "(the seed for the random-number generator).\n";
    return errcode_parameter;
  }

  const int seed = (argc == 1) ? 1 : convert_seed(argv[1]);
  if (seed == 0) {
    std::cerr << err << "The seed \"" << argv[1] << "\" must be an integer >= 1 fitting into type int.\n";
    return(errcode_parameter_value);
  }
  set_random(seed);

  typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<> CLSAdaptor;
  CLSAdaptor output(std::cout);
  typedef OKlib::InputOutput::CLSAdaptorRandomShuffle<> CLSRandomShuffleAdaptor;
  CLSRandomShuffleAdaptor randomiser(base_rand_gen, output);
  randomiser.comment("Randomised with seed=" + 
                     boost::lexical_cast<std::string>(seed) + ".");
  OKlib::InputOutput::StandardDIMACSInput<CLSRandomShuffleAdaptor>(std::cin, 
                                                                   randomiser);
}
