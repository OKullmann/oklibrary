// Oliver Kullmann, 2.2.2006 (Swansea)
/* Copyright 2006 - 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Assignments/PartialAssignments/plans/general.hpp
  \brief Plans for the module on partial assignments


  \todo DONE
  Move Concepts/plans/PartialAssignments.hpp here.


  \todo DONE
  Move PartAssign.hpp here.


  \todo Update namespace usage.


  \todo Old implementations
  <ul>
   <li> See OKsolver/Experimental/AllgKlassen200203/PartialAssignments.hpp.
   </li>
   <li> Update PartialAssignments/PartAssign.hpp, so that it becomes
   a very simple implementation of the generic concepts. </li>
  </ul>


  \todo Evaluation
  <ul>
   <li> Evaluation of literals is the direct responsibility of the partial
   assignment class (yielding val0, val1 or unassigned). </li>
   <li> While for the evaluation of active clause-sets we have an algorithm
   \code
template <class PASS, class CLS>
Satisfiability::Values::Assignment_status evaluate
 (PASS phi, CLS F, OKlib::PartialAssignments::evaluation_tag<CLS>::type,
  const OKlib::RootEvaluationTag& tag);
   \endcode
   with
   <code> struct RootEvaluationTag {}; </code>
   possibly to be replaced by a derived class (which might be polymorphic)
   to enable compile- and run-time dispatch of evaluation algorithms. </li>
   <li> According to our general strategy, we should primarily implement
   evaluate as a class template (in namespace OKlib::PartialAssignments).
   </li>
  </ul>


  \todo Design study for evaluation
  <ul>
   <li> In the following likely boost::tribool should be replaced by
   Satisfiability::Values::Assignment_status:
   \code
template <class PartialAssignment, class Formula>
class Evaluate : std::binary_function<const PartialAssignment&, const Formula&, boost::tribool> {
  boost::tribool operator() (const PartialAssignment& phi, const Formula& F, const ::OKlib::RootEvaluationTag& tag = ::OKlib::RootEvaluationTag()) const {
    return evaluate_dispatch()(phi, F, ::OKlib::evaluation_tag<Formula>::type(), tag);
  }
};
template <class PartialAssignment, class Formula>
boost::tribool evaluate(const PartialAssignment& phi, const Formula& F, const ::OKlib::RootEvaluationTag& tag = ::OKlib::RootEvaluationTag()) {
  return Evaluate<PartialAssignment, Formula>()(phi, F, tag);
}

template <class PartialAssignment, class Formula>
boost::tribool evaluate_dispatch()(const PartialAssignment& phi, const Formula& F, ::OKlib::PartialAssignments::evaluation_conjunction, const ::OKlib::RootEvaluationTag&) {
  typedef boost::range_const_iterator<F>::type const_iterator;
  const const_iterator& end(boost::end(F));
  tribool result(indeterminate);
  for (const_iterator element(boost::begin(F)); element!=end; ++element)
    if (not result &= evaluate(phi, *i))
      return false;
  return result;
}
template <class PartialAssignment, class Formula>
boost::tribool evaluate_dispatch()(const PartialAssignment& phi, const Formula& F, ::OKlib::PartialAssignments::evaluation_disjunction, const ::OKlib::RootEvaluationTag&) {
  typedef boost::range_const_iterator<F>::type const_iterator;
  const const_iterator& end(boost::end(F));
  tribool result(indeterminate);
  for (const_iterator element(boost::begin(F)); element!=end; ++element)
    if (result |= evaluate(phi, *i))
      return true;
  return result;
}
template <class PartialAssignment, class Formula>
boost::tribool evaluate_dispatch()(const PartialAssignment& phi, const Formula& F, ::OKlib::PartialAssignments::evaluation_literal, const ::OKlib::RootEvaluationTag&) {
  return phi(F);
}
   \endcode
   </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Assignments::PartialAssignments
  \brief Components regarding partial assignments
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Assignments {
      namespace PartialAssignments {
      }
      namespace PAs = PartialAssignments;
    }
  }
}

