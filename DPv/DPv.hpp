#ifndef DPV_Hg8o7tq89

#define DPV_Hg8o7tq89

#include <utility>
#include <set>

namespace OKlib {
  
  namespace DPv {
    
    struct Variable {
      
      typedef unsigned int value_type;

      Variable(const value_type& v) : v(v) {};      
      Variable():v(0){};

    private:
      value_type v;
      
    public:
      friend inline bool operator ==(const Variable lhs, const Variable rhs) {
	return lhs.v == rhs.v;
      }
      friend inline bool operator <(const Variable lhs, const Variable rhs) {
	return lhs.v < rhs.v;
      }
    };

    struct Literal {
      enum Sign {P = 0, N = 1};
      typedef Variable base_type;
      typedef std::pair<Variable,Sign> value_type;

      Literal(const value_type& l) : l(l) {};
      Literal(const base_type& v) : l(std::make_pair(v,P)) {};
      Literal(const base_type& v, const Sign S) : l(std::make_pair(v,S)) {};
      Literal() : l(std::make_pair<base_type,Sign>(base_type(),P)) {};

    private:
      value_type l;
 
    public:
      friend inline bool operator ==(const Literal lhs, const Literal rhs) {
	return lhs.l.first == rhs.l.first;
      }
      friend inline bool operator <(const Literal lhs, const Literal rhs) {
	return lhs.l.first < rhs.l.first;
      }
    };
    
    
    struct Clause {
      typedef std::set<Literal> value_type;
      typedef Literal base_type;
      
      Clause(const value_type& c) : c(c) {};
      Clause() : c(value_type()) {};

    private:
      value_type c;
    };

    struct Clause_set {
      typedef std::set<Clause> value_type;
      typedef Clause base_type;

      Clause_set(const value_type& cs) : cs(cs) {};
      Clause_set() : cs(value_type()) {};
    private:
      value_type cs;
    };
  }
}
#endif 
