// Oliver Kullmann, 25.2.2002 (Swansea)

// VarSet.hpp

#ifndef WAECHTERVARSET

#include <map>
#include <string>
#include <deque>


namespace Variables {

  namespace Error {
    
    struct empty_name {};
    // when creating a new variable with empty name

    struct invalid_assignment {};
    // when making an assignment for variables of type Var_Set
  }


  class Var_basic {
  protected :
    typedef unsigned long int Var_id;
    static const Var_id exception = 0; // not a valid variable identifier
  };

  class Var;

  class Var_Set : public Var_basic {

  public :

    Var_Set();

    Var get_var(string);

    Var_Set& operator = (const Var_Set&) { throw Error::invalid_assignment(); }

  private :

    map<string, Var_id> idm; // identifier map

    deque<string> nm; // name map

    Var_id current;

    friend class Var;
  };

  inline Var_Set::Var_Set() {
    current = exception;
  }


  class Var : public Var_basic {

  public :

    Var() {}

    string get_name() const;

    bool operator < (Var) const;
    bool operator == (Var) const;

  private :

    Var_id id;
    Var_Set *S;

    friend class Var_Set;

  };

  inline bool Var::operator < (Var v) const {
    return (&(S -> idm) < &(v.S -> idm)) || (&(S -> idm) == &(v.S -> idm) && id < v.id);
  }

  inline bool Var::operator == (Var v) const {
    return S == v.S && id == v.id;
  }

  inline string Var::get_name() const {
    if (id == exception)
      return "";
    return (S -> nm)[id - 1];
  }

  inline Var Var_Set::get_var(string s) {
    if (s == "")
      throw Error::empty_name();
    Var v; v.S = this;
    map<string, Var_id>::iterator p = idm.find(s);
    if (p == idm.end()) {
      idm[s] = ++current;
      nm.push_back(s);
      v.id = current;
    }
    else
      v.id = p -> second;
    return v;
    }

}



#endif

#define WAECHTERVARSET

