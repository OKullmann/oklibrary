// Tony Bao, 22.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProofSystems/DPv/BasicDataStructure.hpp
  \brief Deprecated. Contains simple classes for variables, literals, clauses, clause-sets and literal- and variables-sets.
  \deprecated
  \todo Classes like this should be simple models of the general concepts.
*/

#ifndef BASICDATASTRUCTURE_HJG9PTG9G275hg2
#define BASICDATASTRUCTURE_HJG9PTG9G275hg2

#include <cstdlib>
#include <set>

namespace OKlib {
  namespace DPv {
        
    typedef int int_type;
        
    struct Literal {
 
      Literal(const int_type l) : l(l) {};
      Literal() : l(0) {};
      int_type l;
      
      friend inline bool operator ==(const Literal lhs, const Literal rhs) {
	return lhs.l == rhs.l;
      }
      friend inline bool operator <(const Literal lhs, const Literal rhs) {
	return lhs.l < rhs.l;
      }
    };
    
    inline bool operator !=(const Literal lhs, const Literal rhs) {
      return not (lhs.l == rhs.l);
    }
    
    inline bool operator >(const Literal lhs, const Literal rhs) {
      return rhs.l < lhs.l;
    }
       
    inline bool operator <=(const Literal lhs, const Literal rhs) {
      return lhs.l < rhs.l or lhs.l == rhs.l;
    }
    
    inline bool operator >=(const Literal lhs, const Literal rhs) {
      return lhs.l > rhs.l or lhs.l == rhs.l;
    }

    
    struct Variable {
      
      Variable(const int_type v) : v(std::abs(v)) {};
      Variable(const Literal& literal) : v(std::abs(literal.l)){};
      Variable() : v(0){};

      int_type v;
      
      friend inline bool operator ==(const Variable lhs, const Variable rhs) {
	return lhs.v == rhs.v;
      }
      friend inline bool operator <(const Variable lhs, const Variable rhs) {
	return lhs.v < rhs.v;
      }
    };
    
    inline bool operator !=(const Variable lhs, const Variable rhs) {
      return not (lhs.v == rhs.v);
    }
    
    inline bool operator >(const Variable lhs, const Variable rhs) {
      return rhs.v < lhs.v;
    }
    
    inline bool operator <=(const Variable lhs, const Variable rhs) {
      return lhs.v < rhs.v or lhs.v == rhs.v;
    }
    
    inline bool operator >=(const Variable lhs, const Variable rhs) {
      return lhs.v > rhs.v or lhs.v == rhs.v;
    }
    
    struct Clause {
      typedef std::set<Literal> base_type;
      typedef base_type::value_type value_type;
      typedef base_type::const_iterator const_iterator;

      base_type c;

      template <typename T>
      void show(T& os) const {
	os << "\\{ ";
	for(const_iterator i = c.begin(); i != c.end(); ++i) {
	  os << (*i).l;
	  if (i != --c.end()) os << ", ";
	}
	os << "\\} ";
      }

      friend inline bool operator ==(const Clause lhs, const Clause rhs) {
	return lhs.c == rhs.c;
      }
      friend inline bool operator <(const Clause lhs, const Clause rhs) {
	return lhs.c < rhs.c;
      }
    };
    
    inline bool operator !=(const Clause lhs, const Clause rhs) {
      return not (lhs.c == rhs.c);
    }
    
    inline bool operator >(const Clause lhs, const Clause rhs) {
      return rhs.c < lhs.c;
    }
    
    inline bool operator <=(const Clause lhs, const Clause rhs) {
      return lhs.c < rhs.c or lhs.c == rhs.c;
    }
    
    inline bool operator >=(const Clause lhs, const Clause rhs) {
      return lhs.c > rhs.c or lhs.c == rhs.c;
    }
    
    void cl_insert(const Literal& l, Clause& cl){
      cl.c.insert(l); 
    }
    
    struct Clause_set {
      typedef std::set<Clause> base_type;
      typedef base_type::value_type value_type;
      typedef base_type::const_iterator const_iterator;
      
      base_type cs;
      
      template <typename T> friend
      T& operator <<(T& os, const Clause_set& cls);
      
      template <typename T>
      void show(T& os) const {
	os << "{ ";
	for(const_iterator i = cs.begin(); i != cs.end(); ++i){ 
	  (*i).show(os); if (i != --cs.end()) os << ", ";
	}
	os << " }";
      }
    };
    
    
    void cls_insert(const Clause& cl, Clause_set& cls){
      cls.cs.insert(cl); 
    }
    
    typedef std::set<Variable> VarSet;
    typedef std::set<Literal> LitSet;
    
    struct LiteralSet {
      
      typedef LitSet::const_iterator const_iterator;
      LiteralSet(const Clause_set& cls) {
	for (Clause_set::const_iterator i = cls.cs.begin(); i != cls.cs.end(); ++i) 
	  for (Clause::const_iterator j = (*i).c.begin(); j != (*i).c.end(); ++j) 
	    lits.insert(*j);
      }
      
      template <typename T>
      void show(T& os) {
	for(const_iterator i = lits.begin(); i != lits.end(); ++i) 
	  os << (*i).l << ' ';
      }
      
      LitSet lits;
    };
    
    struct VariableSet {
      typedef VarSet::const_iterator const_iterator;
      typedef VarSet::iterator iterator;
      typedef Variable base_type;
      VariableSet(const Clause_set& cls) {
	LiteralSet set_of_literals(cls);
	for (LiteralSet::const_iterator i = set_of_literals.lits.begin(); i != set_of_literals.lits.end(); ++i)
	  vars.insert(std::abs((*i).l));
      }
      
      template <typename T>
      void show(T& os) {
	for(const_iterator i = vars.begin(); i != vars.end(); ++i) 
	  os << (*i).v << ' ';
      }
      
      VarSet vars;
    };
    
  }
}
#endif
