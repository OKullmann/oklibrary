// Oliver Kullmann, 2.2.2006 (Swansea)

/*!
  \file PartialAssignments/plans/PartialAssignments.hpp
  \brief Plans for the module on partial assignments

  \todo See Concepts/plans/PartialAssignments.hpp.

  \todo It seems sensible that evaluation of literals is the direct
  responsibility of the partial assignment class, while for the evaluation
  of active clause-sets we have an algorithm

  template <class PASS, class CLS>
  boost::logic::tribool evaluate(PASS phi, CLS F, OKlib::PartialAssignments::evaluation_tag<CLS>::type, const OKlib::RootEvaluationTag& tag)

  with
  struct RootEvaluationTag {};
  possibly to be replaced by a derived class (which might be polymorphic)
  to enable compile- and run-time dispatch of evaluation algorithms.
  According to our general strategy, we should primarily implement
  evaluate as a class template (in namespace OKlib::PartialAssignments).

  \todo Design study:
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

*/

/*!
  \namespace OKlib::PartialAssignments
  \brief Components regarding partial assignments
*/

namespace OKlib {
  namespace PartialAssignments {
  }
}

