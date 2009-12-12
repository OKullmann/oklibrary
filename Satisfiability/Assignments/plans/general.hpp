// Oliver Kullmann, 23.9.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Assignments/plans/general.hpp
  \brief General plans on (partial and total) assignments, their concepts and
  implementations.


  \todo Move the OKlib/Concepts/plans/Assignments.hpp here.


  \todo How to handle "unassigned" ?
  <ul>
   <li> Determining the state of a variable w.r.t. a partial or total
   assignment, it can be unassigned or it can have a specific value. </li>
   <li> The boolean case:
    <ol>
     <li> We could use boost::logic::tribool, with its three possibilities
     true, false, indeterminate. </li>
     <li> A problem is that "indeterminate" means not a true third possibility
     (like "unassigned"), but "definitely true or false, but unknown". </li>
     <li> So comparing one indeterminate-value with any other tribool-value
     via ==, we always get indeterminate as result. </li>
     <li> This makes handling inconvenient (and counterintuitive). </li>
     <li> In OKlib/OKsolver/Experimental/AllgKlassen200203/Auxiliary.hpp we
     have Auxiliary::Sat_status and Auxiliary::Evaluation_status. </li>
     <li> As integers we always have false-equivalent=0. true-equivalent=1.
     third-value=2. </li>
     <li> We could establish a small battery of such enumerations: then we
     always had meaningfull names, while handling is simple (as integers).
     </li>
     <li> So for example "Sat_status", "Assignment_status",
     "Evaluation_status". </li>
     <li> Especially regarding cases with more than two values, perhaps the
     "additional value" (like "unassigned") is encoded by -1 --- in this
     way for all such types one can easily determine that additional value.
     </li>
     <li> Of course, using {-1,0,1} for booleans has another natural
     interpretation, namely -1 for false, +1 for true, and 0 for unassigned.
     </li>
     <li> From this point of view it would be more natural to use the maximal
     integer for the third value; so for such three-valued "booleans" we
     would use the codes 0, 1, std::numeric_limits<enum>::max(). </li>
     <li> However, with C++03 this is not a constant; one had to use the
     C macro INT_MAX instead. </li>
     <li> Actually, this is not guaranteed to work, since the underlying
     integral type of an enumeration is not specified by the standard. And so
     we can not find out the maximal value. </li>
     <li> In principal then even Sat_status and Assignment_status could have
     different underlying integral types; perhaps we should ignore this,
     since it shouldn't matter. </li>
     <li> So then perhaps we use -1 for the additional value (this also makes
     sure that the underlying integral type is signed). </li>
     <li> On the other hand, one can overload on enumeration types, and
     thus one could use a function like exceptional_value<T>(). Again,
     only with C++09 this would be a constant. </li>
     <li> For C++03 one needed to use class templates: Exceptional<T>::value.
     </li>
     <li> Perhaps this is best, to use such meta-functors, perhaps for the
     first and last "real" value, and the exceptional value. </li>
     <li> The placement could be in namespace
     OKlib::Satisfiability::Enumerations::traits. </li>
     <li> The disadvantage of using enums is that we don't have full
     type-safety; this is the advantage of the boost-type. </li>
     <li> Of course, we could establish this small battery as a battery of
     classes. On the other hand, with the current tendency to use a lightweight
     approach (allowing primitive types as often as possible), this likely
     wouldn't work convenient. </li>
     <li> The question is whether type-safety is worth all the effort; at this
     time I (OK) am inclined that it is not worth the effort. </li>
    </ol>
   </li>
   <li> Non-boolean cases:
    <ol>
     <li> The basic case could just be enumerations, starting with 0, using
     -1 for the additional value. </li>
     <li> Better to leave this open for each type, using the above
     meta-functors to determine first and last value, and possibly exceptional
     values. </li>
    </ol>
   </li>
   <li> Where to place such types? OKlib::Satisfiability::Enumerations
   perhaps. "Auxiliary" seems to unspecific; on the other hand "Enumerations"
   might be too special? "ValueTypes"? "Values"?; perhaps that is best. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Assignments
  \brief The part of the OKlibrary concerned with assignments (partial and total)

  Alias "Asg".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Assignments {
    }
    namespace Asg = Assignments;
  }
}

