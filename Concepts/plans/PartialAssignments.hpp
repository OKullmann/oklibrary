// Oliver Kullmann, 28.11.2006 (Swansea)

/*!
  \file Concepts/plans/PartialAssignments.hpp
  \brief Plans for concepts for partial assignments

  \todo Concept of a partial assignment phi:
   - boost::logic::tribool(phi) returns false if phi is inconsistent,
     true if phi doesn't exclude any assignment, and indeterminate
     otherwise. (false corresponds to the tautological clause, true to the
     empty clause.)
   - Template <typename Range> phi.insert(Range) inserts a range of
     literals (restricting the associated set of total assignments to those
     which satisfy all these literals).
   - boost::logic::tribool phi(const Literal&) determines whether the
     literal is true resp. false for all assignments extending phi (if phi is
     inconsistent, then the result is undefined).
   - literal_type<PartialAssignment>>::type
   - phi is a container of literals (the sequence of current standardised
     literals in some implementation-defined order), at most one literal per variable.
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

  \todo Refinements of partial assignments offer access to a variable container.

*/
