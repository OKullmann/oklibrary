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
        
      friend inline bool operator ==(const Literal& lhs, const Literal& rhs) {
	return lhs.l == rhs.l;
      }
      friend inline bool operator <(const Literal& lhs, const Literal& rhs) {
	return lhs.l < rhs.l;
      }
    };

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
    
       
    struct Clause {
      typedef std::set<Literal> base_type;
      typedef base_type::value_type value_type;
      typedef base_type::const_iterator const_iterator;

      base_type c;

      friend inline bool operator ==(const Clause lhs, const Clause rhs) {
	return lhs.c == rhs.c;
      }
      friend inline bool operator <(const Clause lhs, const Clause rhs) {
	return lhs.c < rhs.c;
      }
    };
    
    void cl_insert(const Literal& l, Clause& cl){
      cl.c.insert(l); 
    }

    struct Clause_set {
      typedef std::set<Clause> base_type;
      typedef base_type::value_type value_type;
      typedef base_type::const_iterator const_iterator;
  
      base_type cs;
      
      //template <class charT, class traits> friend
      //std::basic_ostream<charT,traits>& operator << (std::basic_ostream<charT,traits>& os, const Clause_set& obj);

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
      void show() {
	for(const_iterator i = lits.begin(); i!=lits.end();++i) std::cout<< (*i).l << ' ';
      }
      LitSet lits;
    };
    
    struct VariableSet {
      typedef VarSet::const_iterator const_iterator;
      VariableSet(const Clause_set& cls) {
	LiteralSet set_of_literals(cls);
	for (LiteralSet::const_iterator i = set_of_literals.lits.begin(); i != set_of_literals.lits.end(); ++i)
	  vars.insert(std::abs((*i).l));
      }
      
      void show() {
	for(const_iterator i = vars.begin(); i!=vars.end();++i) std::cout<< (*i).v << ' ';
      }
      
      VarSet vars;
    };

    void show(const Clause& cl) {
      std::cout << "{ ";
      for(Clause::const_iterator i = cl.c.begin(); i!=cl.c.end();++i) 
	std::cout << (*i).l << ' ';
    
      std::cout << "} ";
    }

    void show(const Clause_set& cls) {
      std::cout << "{ ";
      for(Clause_set::const_iterator i = cls.cs.begin(); i!=cls.cs.end();++i){ 
	show(*i);
      }
      std::cout << " }";
    }
    
  }
}
#endif
