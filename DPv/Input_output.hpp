#ifndef INPUTOUTPUT_ah9s8ty332

#define INPUTOUTPUT_ah9s8ty332

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>

namespace OKlib {
  
  namespace DPv {
    
    namespace Input {
      
      struct Dimacs {
	Dimacs(const std::string& fname) : fname(fname) {
	  fopen();
	}
      private :
	std::ifstream input;
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

    };
    
    }
}
}
#endif
