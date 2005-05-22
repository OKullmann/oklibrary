#ifndef INPUTOUTPUT_ah9s8ty332

#define INPUTOUTPUT_ah9s8ty332

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <cassert>
#include <utility>
#include <limits>

#include <boost/lexical_cast.hpp>

#include "DPv.hpp"
#include "BasicDataStructure.hpp"

namespace OKlib {
  
  namespace DPv {
    
    namespace Input {

      struct FStream {

	FStream(const std::string& fname) : fname(fname) {
	  fopen();
	}  
	~FStream() {
	  fclose();
	}
	
	std::fstream filestream;
	
      private :
	
	std::string fname;
	void fopen() {
	  if (not filestream.is_open()) filestream.open(fname.c_str(), std::ios_base::in);
	  assert(filestream.is_open() == true);
	}

	void fclose() {
	  if (filestream.is_open()) filestream.close();
	  assert(filestream.is_open() == false);
	}
	

      }; 
    }
      struct Dimacs {
	
	typedef OKlib::DPv::Clause_set::const_iterator const_iterator;
	OKlib::DPv::Clause_set formula;

	template <typename charT, typename traits> friend
	Dimacs& operator >>(std::basic_istream<charT, traits>& is, Dimacs& obj);

	friend Dimacs& operator >>(std::fstream& is, Dimacs& obj);
      };
      
      
	
      template <typename charT, typename traits>
      Dimacs& operator >>(std::basic_istream<charT, traits>& is, Dimacs& obj){
	return obj;
      }
    
      Dimacs& operator >>(std::fstream& is, Dimacs& obj){

	char x = is.peek();
	while (x == 'c' or x == 'p') {
	  is.ignore(std::numeric_limits<int>::max(), '\n');
	  x = is.peek();
	}

	OKlib::DPv::int_type j; 
	while (not is.eof()){
	  is >> j;
	  Clause cl;
	  while (j != 0){
	    Literal l(j);
	    cl_insert(l,cl); 
	    is >> j;
	  }
	  //for (Clause::const_iterator i = cl.c.begin(); i!=cl.c.end();++i) {
	  //std::cout << (*i).l << '\t';
	  //}
	  //std::cout << '\n';
	  if (j == 0) is.ignore(1,'\n');
	  if (not cl.c.size() == 0) cls_insert(cl, obj.formula);
	}
	return obj;
      }
  }
}
#endif
