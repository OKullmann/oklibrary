// Oliver Kullmann, 2.2.2006 (Swansea)

/*!
  \file PartialAssignments/plans/PartialAssignments.hpp
  \brief Plans for the module on partial assignments

  \todo Concept of a partial assignment phi:
   - boost::logic::tribool(phi) returns false if phi is inconsistent,
     true if phi doesn't exclude any assignment, and indeterminate
     otherwise. (false corresponds to the tautological clause, true to the
     empty clause.)
   - Template <typename Range> phi.insert(Range) inserts a range of
     literals (restricting the associated set of total assignments to those
     which satisfy all these literals).
   - boost::logic::tribool phi(const Literal&) determines whether the
     literal is true resp. false for all partial assignments (if phi is
     inconsistent, then the result is undefined).
   - literal_type<PartialAssignment>>::type
   - phi is a container of literals (the sequence of current standardised
     literals in some implementation-defined order).
   - If literals are variable-based, then with phi.literal_status(var) we
     obtain a tribool telling whether the literal is always true,
     always false, or indeterminate, and with phi.literal(var) we obtain
     an iterator to the literal; both operations either need constant time
     or logarithmic time.

  \todo A refined concepts gives also (somehow) access to the updates
     in an alliance of active clause-sets (perhaps each active clause-set
     can be registered?). One idea:
     With phi.update_iterator() an active clause-set F can obtain an input iterator i_F
     from phi (specifically to be used by F).
     With phi.new_end(i_F) one gets the current past-the-end iterator (of the same type
     as i_F), so that from i_F to this iterator the update-literals can be found; two models:
     - i_F is independent of F, and is just the end-iterator of the list of all update-literals
     (for boolean clause-sets this is fully enough, since here this sequence is just a
     standardised partial assignment);
     - phi processes all the update-literals from i_F to the current end, and maintains a
     compressed form of the corresponding partial assignment (which should be just the
     standardisation).
     We see that effectively phi maintains *views* for every clause-set F, providing
     partial assignments which show F the "difference" between the last state of phi
     and the new state.

  \todo It seems sensible that evaluation of literals is the direct
  responsibility of the partial assignment class, while for the evaluation
  of active clause-sets we have an algorithm
  template <class PASS, class CLS>
  boost::logic::tribool evaluate(PASS phi, CLS F, OKlib::PartialAssignments::evaluation_tag<CLS>::type, const OKlib::RootEvaluationTag& tag)
  with
  struct RootEvaluationTag {};
  possibly to be replaced by derived class (which might be polymorphic)
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
  \todo Refinements of partial assignments offer access to a variable container.

  \todo Write an implementation of partial assignments for variable with index,
  offering constant time operation phi(literal).
*/

/*!
  \namespace OKlib::PartialAssignments
  \brief Components regarding partial assignments
*/

namespace OKlib {
  namespace PartialAssignments {
  }
}

