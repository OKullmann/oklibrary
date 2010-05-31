// Oliver Kullmann, 10.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/TwoSolverComparison/plans/ProbabilityOfSuperiority.hpp
  \brief Plans regarding the method for comparing two solvers according to [Mladen Nikolic, 2010]

  [Mladen Nikolic, 2010, Statistical Methodology for Comparison of
  SAT Solvers] is to appear in SAT 2010 proceedings.


  \todo The wider framework
  <ul>
   <li> See the "SAT Evaluation Toolkit"
   (Experimentation/Competition/plans/general.hpp). </li>
   <li> DONE (see ProbabilityOfSuperiority.R)
   The first stage is to implement the statistical evaluation in R. </li>
  </ul>


  \todo DONE (renamed file to ProbabilityOfSuperiority, and renamed function
  to probabilityofsuperiority)
  Names
  <ul>
   <li> Enter file-creation-date for TwoSolverComparison.R. </li>
   <li> Improve filename:
    <ol>
     <li> The submodule carries the generic name, while every file in it
     should be named according to its *specific* content. </li>
     <li> So "TwoSolverComparison.R" (and accordingly this plans-file) should
     be named according to the specific method implemented. </li>
     <li> Also all other instances of "TwoSolverComparison" need to be checked
     (via grep), and renamed if necessary. </li>
    </ol>
   </li>
   <li> The main function also needs to be renamed, so that
   the specific method becomes clear (and no use of camel-case). </li>
  </ul>


  \todo Input
  <ul>
   <li> Apparently a helper function is needed, which reads the data files,
   and a convenience instantiation of the main function, where the inputs
   are not tables, but filenames. </li>
   <li> The helper function is called "read_XXX", where XXX stands for the
   name of the method (see "Names" above). </li>
   <li> This function needs (of course) specification. </li>
  </ul>


  \todo Output
  <ul>
   <li> The main function needs to output its result into a suitable
   structure (apparently a list), so that they are available for further
   computations. </li>
   <li> While printing of results only happens if input-variable monitor
   is TRUE (that shall be the default). </li>
  </ul>


  \todo Specifications for ProbabilityOfSuperiority.R
  <ul>
   <li> All functions need to be specified. </li>
   <li> gwtest </li>
   <li> gehanw </li>
   <li> rcor </li>
   <li> jackknifevariance </li>
   <li> bootstrapvariance </li>
   <li> ProbabilityOfSuperiority </li>
  </ul>


  \todo Write docus
  <ul>
   <li> How is the main function to be used? Examples? </li>
   <li> Input specification:
    <ol>
     <li> DONE (yes, this is the case)
     It appears that the input is just a data-frame file, which per row
     contains the times for the two solvers A, B to be compared. </li>
     <li> DONE (yes, needed)
     Is the knowledge of the cutoff-time needed for the evaluation? </li>
     <li> DONE (one row per instance
     As discussed in the paper, there are actually dependencies between
     the rows, in that a block of rows belongs to a single instances and its
     shuffled variations --- however it seems for the computations performed
     this is irrelevant? </li>
    </ol>
   </li>
   <li> Output specification:
    <ol>
     <li> Besides the precise specification in computational terms, also the
     "meaning" needs to be specified. </li>
     <li> Best if a precise probabilistic semantics can be given. </li>
     <li> In any case, we need also a good discussion of the "intuitive
     meaning". </li>
     <li> What shall be achieved by the comparison? Is the underlying (precise)
     model, that one solver is better than the other iff the expected run-time
     over the space of sample inputs is lower? </li>
    </ol>
   </li>
  </ul>

*/
