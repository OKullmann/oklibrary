#ifndef DPV_Hg8o7tq89

#define DPV_Hg8o7tq89

#include <utility>
#include <algorithm>
#include <set>
#include <string>

#include "BasicDataStructure.hpp"

namespace OKlib {
  
  namespace DPv {
    
    namespace Error{
      
      struct Unknown_result {
	std::string what() const {
	  return "Reduction is in final form but can not determine SAT status";
	}
      };
      
      struct Illogical {
	Illogical(const std::string& msg) : msg(msg) {}
	std::string what() const {
	  return msg;
	}
	const std::string msg;
      };

    }
    
    Literal literal_compliment(const Literal& l){
      Literal tmp(l);
      tmp.l = -1 * l.l;
      return tmp;
    }
    
    Clause clause_compliment(const Clause& cl) {
      Clause tmp;
      for (Clause::const_iterator i = cl.c.begin();i!=cl.c.end();++i){
	cl_insert(literal_compliment((*i)), tmp);
      }
      return tmp;
    }
    
    bool resolvable(const Clause& lhs, const Clause& rhs){
      Clause tmp, result;
      tmp.c = clause_compliment(rhs).c;
      std::set_intersection(lhs.c.begin(), lhs.c.end(),tmp.c.begin(),tmp.c.end(),std::inserter(result.c, result.c.begin()));
      if (result.c.size() == 1) 
	return true; 
      else 
	return false;
      return false;
    }
    bool resolvable(const Variable& v, const Clause& lhs, const Clause& rhs){
      Clause tmp, result;
      tmp.c = clause_compliment(rhs).c;
      std::set_intersection(lhs.c.begin(), lhs.c.end(),tmp.c.begin(),tmp.c.end(),std::inserter(result.c, result.c.begin()));
      //std::cout << Variable(*(result.c.begin())).v;
      if (result.c.size() == 1 and Variable(*(result.c.begin())) == v) 
	return true; 
      else 
	return false;
      return false;
    }

    Clause resolvent(const Clause& lhs, const Clause& rhs){
      Clause result;
      if (resolvable(lhs,rhs)){
	Clause tl,tr,ctr, rv;
	tl.c = lhs.c;
	tr.c = rhs.c;
	ctr.c = clause_compliment(tr).c;
	std::set_intersection(tl.c.begin(), tl.c.end(),ctr.c.begin(),ctr.c.end(),std::inserter(rv.c, rv.c.begin()));
	Literal l(*(rv.c.begin()));
	if (tl.c.count(l) > 0)
	  tl.c.erase(tl.c.find(l));
	else
	  tl.c.erase(tl.c.find(literal_compliment(l)));
	if (tr.c.count(l) > 0)
	  tr.c.erase(tr.c.find(l));
	else
	  tr.c.erase(tr.c.find(literal_compliment(l)));
	std::set_union(tl.c.begin(), tl.c.end(),tr.c.begin(),tr.c.end(),std::inserter(result.c, result.c.begin()));
      }
      return result;
    }

    Clause_set DP_reduction(const Variable& v, const Clause_set& cls){
      Clause_set tmp, resolvents, to_remove;
      tmp.cs = cls.cs;
      for (Clause_set::const_iterator i = cls.cs.begin(); i != cls.cs.end(); ++i){
	for (Clause_set::const_iterator j = cls.cs.begin(); j != cls.cs.end(); ++j) {
	  if (resolvable(v,*i,*j)) {
	    cls_insert(resolvent(*i,*j),resolvents);
	    cls_insert(*i,to_remove);
	    cls_insert(*j,to_remove);
	  }
	}
      }
      for (Clause_set::const_iterator i = to_remove.cs.begin(); i != to_remove.cs.end(); ++i)
	tmp.cs.erase(tmp.cs.find(*i));
      for (Clause_set::const_iterator i = tmp.cs.begin(); i != tmp.cs.end(); ++i) {
	if ((*i).c.count(Literal(v.v)) > 0 or (*i).c.count(literal_compliment(Literal(v.v))) > 0) 
	  tmp.cs.erase(i);
      }
      
      std::set_union(tmp.cs.begin(), tmp.cs.end(),resolvents.cs.begin(),resolvents.cs.end(),std::inserter(tmp.cs, tmp.cs.begin()));
      return tmp;
    }
    
    bool final_form (const Clause_set& cls) {
      if (VariableSet(cls).vars.size() == 0) return true;
      return false;
    }
    
    bool is_sat(const Clause_set& cls) {
      
      if (final_form(cls)) {
	if (cls.cs.size() == 0) 
	  return true;
	else 
	  if (cls.cs.size() == 1) 
	    return false;
	  else
	    throw Error::Unknown_result();
      }
      else
	throw Error::Illogical("Reduction not yet finished!");
    }

  }
}
#endif 
