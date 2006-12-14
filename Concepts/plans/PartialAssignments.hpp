// Oliver Kullmann, 28.11.2006 (Swansea)

/*!
  \file Concepts/plans/PartialAssignments.hpp
  \brief Plans for concepts for partial assignments

  \todo Concept of a partial assignment phi:
   - phi is a container of literals (the sequence of current standardised
     literals in some implementation-defined order), at most one literal per variable.
   - All the following considerations generalise accordingly to partial multiassignments.
   - Since literals are never constant, a partial assignment can  not be inconsistent,
     while the empty partial assignment is the only partial assignment without
     restrictions.
   - literal_type<PartialAssignment>>::type
   - Template <typename Range> phi.insert(Range) inserts a range of
     literals (restricting the associated set of total assignments to those
     which satisfy all these literals).
   - With phi.literal(var) we obtain an iterator to the literal, either in constant time
     or in logarithmic time.
   - We need also operations to eliminate values (adding values perhaps is only needed
     when undoing deletions --- see next itme); perhaps we leave such operations
     to the literals? (But then we have problems with undoing?)
   - boost::logic::tribool phi(const Literal&) determines whether the
     literal is true resp. false for all assignments extending phi (if phi is
     inconsistent, then the result is undefined).

     We could leave this algorithm for the attention of literals (or atomic conditions)?
     There is also the incrementality problem (evaluation of partial assignments
     must be possible in an incremental way) --- perhaps we leave this to the
     active clause-sets?

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
