#ifndef INPUTOUTPUT_ah9s8ty332

#define INPUTOUTPUT_ah9s8ty332

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <cassert>

#include "DPv.hpp"

namespace OKlib {
  
  namespace DPv {
    
    namespace Input {

      struct FStream {
      private :
	std::fstream input;
	std::string fname;
	void fopen() {
	  if (not input.is_open()) input.open(fname.c_str(), std::ios_base::in);
	  assert(input.is_open() == true);
	}

	void fclose() {
	  if (input.is_open()) input.close();
	  assert(input.is_open() == false);
	}
      };
	
      struct Dimacs {
	//Dimacs(const std::istream& input) : input(input) {
	//Clause_set cs;
	//}

	//std::istream input;
      };
      
    }
  }
}
#endif
