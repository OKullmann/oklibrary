#ifndef DPV_Hg8o7tq89

#define DPV_Hg8o7tq89

#include <utility>

namespace OKlib {
  
  namespace DPv {
    
    struct Variable {
      
      typedef unsigned int value_type;

      Variable(value_type v) : v(v) {};      
      Variable():v(0){};

    private:
      value_type v;
      
    };

    struct Literal {
      enum Sign {P = 0, N = 1};
      typedef std::pair<Variable,Sign> value_type;

      Literal(value_type l) : l(l) {}
      Literal():l(std::make_pair<Variable,Sign>(Variable(),P)) {};

    private:
      value_type l;
    };
    
    struct Clause {
      
    };
  }
}
#endif 
