// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file Concepts/plans/Assignments.hpp
  \brief Plans for concepts for partial and total assignments.
  For an earlier attempt see
  OKsolver/Experimental/AllgKlassen200203/PartialAssignments.hpp.
  \deprecated Needs to be transferred to Concepts/plans/PartialAssignments.hpp (or should
  this be the place for *total* assignments?).

  \todo Concepts for assignments as specialisations of concepts for maps (see my notes as
  discussed in the MSc thesis of B):
  If f is an assignment, then dom(f) is defined, and for v of type dom(f) the expression f(v)
  can be build (v is a variable with values, f(v) is of the value type). dom(f) is a set in the
  general sense (which can have iterators and element-decision). As a refinement iterators
  through all pairs (x, y) (so that std::map can easily yield models). See Clauses.hpp.
*/

