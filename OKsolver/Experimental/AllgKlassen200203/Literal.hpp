// Oliver Kullmann, 13.12.2002 (Swansea

#ifndef LITERALWAECHTER

#define LITERALWAECHTER

#include <ostream>
#include <functional>

// Literals ---------------------------------------------------------

namespace Literal {

  template <class Var, class Value>
  class Lit {
  public :
    Lit(const Var& v_, const Value& s_) :
      v(v_), s(s_) {}
    operator Var() const { return v; }
    operator Value() const { return s; }
  private:
    const Var v;
    const Value s;
  };

  template <class Var, class Value, template <class Var, class Value> class Lit >
  inline bool operator < (const Lit<Var, Value>& x, const Lit<Var, Value>& y) {
    return Var(x) < Var(y) or (Var(x) == Var(y) and Value(x) < Value(y));
  }

  template <class Var, class Value, template <class Var, class Value> class Lit >
  std::ostream& operator << (std::ostream& o, const Lit<Var, Value>& x) {
    return o << Value(x) << Var(x);
  }

  template <class Var, class Value, template <class Var, class Value> class Lit >
  inline const Lit<Var, Value> operator !(const Lit<Var, Value>& x) { return Lit<Var, Value>(Var(x), ! Value(x)); }

  template <class Literal>
  struct Comp : std::unary_function<Literal, Literal> {
    const Literal operator() (const Literal& x) { return ! x; }
  };

}

// Boolean variables -----------------------------------------------------

namespace BoolVarLit {

  class BoolVarUI {
  public :
    BoolVarUI(const unsigned n_) :
      n(n_) {}
  private :
    friend bool operator < (const BoolVarUI& a, const BoolVarUI& b);
    friend bool operator == (const BoolVarUI& a, const BoolVarUI& b);
    friend std::ostream& operator << (std::ostream& o, const BoolVarUI& v);
    const unsigned int n;
  };

  inline bool operator < (const BoolVarUI& a, const BoolVarUI& b) {
    return a.n < b.n;
  }
  inline bool operator == (const BoolVarUI& a, const BoolVarUI& b) {
    return a.n == b.n;
  }
  inline bool operator != (const BoolVarUI& a, const BoolVarUI& b) {
    return not (a == b);
  }
  inline std::ostream& operator << (std::ostream& o, const BoolVarUI& v) {
    return o << v.n;
  }
  
  
  enum Signs {pos = 0, neg = 1};
  class BoolVal {
  public :
    BoolVal(const Signs s_) :
      s(s_) {}
    operator Signs() const { return s; }
    const BoolVal operator !() const { return (s == pos) ? neg : pos; }
  private :
    const Signs s;
  };
  inline bool operator < (const BoolVal& a, const BoolVal& b) {
    return Signs(a) < Signs(b);
  }
  std::ostream& operator << (std::ostream& o, const BoolVal& s) {
    return (s == pos) ? o << "+" : o << "-";
  }
  
  typedef Literal::Lit<BoolVarUI, BoolVal> BoolLitUIV;
}


#endif

