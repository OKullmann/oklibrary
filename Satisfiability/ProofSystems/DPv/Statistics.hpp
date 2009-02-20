// Tony Bao, 19.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProofSystems/DPv/Statistics.hpp
  \brief Deprecated. Some functions for basic statistics about clause-sets.
  \deprecated
  \todo These things (and more) should be part of the general hypergraph
  library (which must be directly applicable to clause-sets).
*/

#ifndef STATISTICS_Jhh9YT324J
#define STATISTICS_Jhh9YT324J

#include <OKlib/Satisfiability/ProofSystems/DPv/BasicDataStructure.hpp>

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
