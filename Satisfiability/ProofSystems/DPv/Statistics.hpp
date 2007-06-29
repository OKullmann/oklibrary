/*!
  \file DPv/Statistics.hpp
  \brief Deprecated. Some functions for basic statistics about clause-sets.
  \todo These things (and more) should be part of the general hypergraph
  library (which must be directly applicable to clause-sets).
*/

#ifndef STATISTICS_Jhh9YT324J
#define STATISTICS_Jhh9YT324J

#include <Transitional/DPv/BasicDataStructure.hpp>

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
    
    
    template <typename T>
    T& operator <<(T& os, const Clause_set& cls){
      VariableSet vs(cls);
      os << "c(F) = " <<clause_count(cls) << '\n';
      for (VariableSet::const_iterator i = vs.vars.begin(); i != vs.vars.end(); ++i) {
	os << "vdg_" << (*i).v << "(F) = " << vdg(*i, cls) << '\t' << "ldg_" << (*i).v << "(F) = " << ldg(Literal((*i).v), cls) << '\t' << "ldg_" << ((*i).v * -1)  << "(F) = " << ldg(literal_compliment(Literal((*i).v)), cls) << '\t' << "c(DP_" << (*i).v << "(F)) = " << clause_left(*i, cls) << '\n';
      }
      return os;
    }

  }
}
#endif
