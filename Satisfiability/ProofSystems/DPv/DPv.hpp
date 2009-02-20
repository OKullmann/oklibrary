// Tony Bao, 19.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProofSystems/DPv/DPv.hpp
  \brief Deprecated. Contains functions DP-reductions.
  \deprecated
  \todo Improved implementations (as models of general concepts) in a new module
  DavisPutnamReduction.
*/

#ifndef DPV_Hg8o7tq89
#define DPV_Hg8o7tq89

#include <utility>
#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <OKlib/Satisfiability/ProofSystems/DPv/BasicDataStructure.hpp>

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
    
    Clause_set& DP_reduction_inplace(const Variable& v, Clause_set& cls) {
      cls.cs = DP_reduction(v,cls).cs;
      return cls;
    }

    bool final_form (const Clause_set& cls) {
      if (VariableSet(cls).vars.size() == 0) return true;
      return false;
    }

    enum Return_Value {SAT, USAT, Unknown};

    Return_Value is_sat(const Clause_set& cls) {
      
      if (final_form(cls)) {
	if (cls.cs.size() == 0) 
	  return SAT;
	else 
	  if (cls.cs.size() == 1) 
	    return USAT;
	  else
	    throw Error::Unknown_result();
      }
      else
	return Unknown;
    }
    
    typedef std::vector<std::pair<unsigned int, std::vector<Variable> > > stat_type;
    

    template <typename VariableSet, typename ClauseSet, typename Stats> 
    Stats& DPv_opt_stats(VariableSet& vs, ClauseSet& cls, Stats& values) {
      ClauseSet tmp;
      tmp.cs = cls.cs;
      unsigned int stat = 0;
      std::vector<typename VariableSet::base_type> list;
      for (typename VariableSet::const_iterator i = vs.vars.begin(); i != vs.vars.end(); ++i) {
	list.push_back(*i);
      }
      bool more_permutation = true;
      while (more_permutation){
	std::vector<typename VariableSet::base_type> order;
	for (typename std::vector<typename VariableSet::base_type>::iterator i = list.begin(); i != list.end(); ++i) {
	  order.push_back(*i); 
	  if (list.size() < 8) std::cout << (*i).v << ' ';
	  stat = stat + clause_count(DP_reduction_inplace(*i,tmp));
	}
	if (list.size() < 8) std::cout << "Total: " << stat << '\n';
	values.push_back(std::make_pair(stat, order));
	tmp.cs = cls.cs; stat = 0;
	more_permutation = std::next_permutation(list.begin(), list.end());
      }
      return values;
    }
    
    template <typename T>
    struct Stat_comp {
      bool operator() (const T& x, const T& y) const {
	return x.first < y.first; 
      }
    };

    
    template <typename T>
    void Best_order(const T& stat) {
      
      typename T::const_iterator max_i;
      typename T::const_iterator min_i;
      
      max_i = std::max_element(stat.begin(), stat.end(), Stat_comp<std::pair<unsigned int, std::vector<Variable> > >());
      min_i = std::min_element(stat.begin(), stat.end(), Stat_comp<std::pair<unsigned int, std::vector<Variable> > >());
   
      std::cout << "Largest total clause numbers is " << (*max_i).first << " with variable order ";
      for (std::vector<Variable>::const_iterator i = (*max_i).second.begin(); i!=(*max_i).second.end();++i) 
	std::cout << (*i).v << ' ';
      std::cout << '\n';
      
      std::cout << "Smallest total clause numbers is " << (*min_i).first << " with variable order ";
       for (std::vector<Variable>::const_iterator i = (*min_i).second.begin(); i!=(*min_i).second.end();++i) 
	std::cout << (*i).v << ' ';
      std::cout << '\n';
    }
  }
}
#endif 
