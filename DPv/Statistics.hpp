#ifndef STATISTICS_Jhh9YT324J

#define STATISTICS_Jhh9YT324J

#include "BasicDataStructure.hpp"

namespace OKlib {

  namespace DPv {

    unsigned int clause_count(const Clause_set& cls){
      return cls.cs.size();
    }

    unsigned int ldg(const Literal l, const Clause_set& cls){
      unsigned int counter = 0;
       for (Clause_set::const_iterator i = cls.cs.begin(); i != cls.cs.end(); ++i) {
	 if ( (*i).c.find(l) != (*i).c.end()) ++counter;

    }
       return counter;
    }

    
    unsigned int vdg(const Variable v, const Clause_set& cls){
       return ldg(Literal(v.v), cls) + ldg(Literal(-v.v), cls);
    }
    
    unsigned int clause_left(const Variable& v, const Clause_set& cls) {
      return clause_count(DP_reduction(v,cls));
    }
    
    /* 
    template <class charT, class traits>
    std::basic_ostream<charT,traits>& operator << (std::basic_ostream<charT,traits>& os, const Clause_set& obj){
      VariableSet vs(obj);
      os << "c(F) = " << clause_count(obj) << '\n';
      for (VariableSet::const_iterator i = vs.vars.begin(); i != vs.vars.end(); ++i) {
	os << "vdg_" << (*i).v << "(F) = " << vdg(*i, obj) << '\t' << "ldg_" << (*i).v << "(F) = " << ldg(Literal((*i).v)) << '\t' << "ldg_" << ((*i).v * -1)  << "(F) = " << ldg(literal_compliment(Literal((*i).v))) << '\n';
	os << "c(DP_" << (*i).v << "(F)) = " << clause_left(DP_reduction(*i, obj)) << '\n';
      }
    }
    */
    
    
    void stat(const Clause_set& obj){
      VariableSet vs(obj);
      std::cout << "c(F) = " << clause_count(obj) << '\n';
      for (VariableSet::const_iterator i = vs.vars.begin(); i != vs.vars.end(); ++i) {
	std::cout << "vdg_" << (*i).v << "(F) = " << vdg(*i, obj) << '\t' << "ldg_" << (*i).v << "(F) = " << ldg(Literal((*i).v),obj) << '\t' << "ldg_" << ((*i).v * -1)  << "(F) = " << ldg(literal_compliment(Literal((*i).v)), obj) << '\t' << "c(DP_" << (*i).v << "(F)) = " << clause_left(*i, obj) << '\n';
      }
    }
  }
}
#endif
