// Oliver Kullmann, 14.12.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/UnitClausePropagation/plans/UnitClausePropagation.hpp
  \brief Plans regarding the specific algorithms used in UnitClausePropagation.cpp / UnitClausePropagationW.cpp


  \todo Better names
  <ul>
   <li> Instead of "UnitClausePropagation.cpp" we should use some more specific
   name. </li>
   <li> The algorithms in this application should perhaps be centred around
   usage of the concept of "binary clauses with removal", and using the
   full clause-literal graph. </li>
   <li> And also UnitClausePropagation/ClsAdaptorUCP.hpp should have some
   more specific name. </li>
  </ul>


  \todo Improve efficiency of UnitClausePropagation(W).cpp
  <ul>
   <li> See
   Experimentation/Investigations/Reductions/UnitClausePropagation/plans/general.hpp
   for investigations. </li>
   <li> See ProblemInstances/Clauses/plans/general.hpp for the main underlying
   data structures. </li>
   <li> Apparently C++ input/output (at least with gcc 4.1.2) is very slow: 
    <ol>
     <li> Just the execution of "Smusat_Horn-O3-DNDEBUG 20000" takes 70s,
     three times as long as it takes minisat2 to read and process the file
     (where writing should be faster than reading). </li>
     <li> See Satisfiability/Transformers/Generators/Smusat_Horn.cpp for some
     attempts to speed up output. </li>
     <li> Parsing of minisat2 uses a simple StreamBuffer class (with a buffer
     size 2^20), and then simple handwritting parsing (character by
     character). </li>
     <li> Perhaps we should do the same for Dimacs-input. </li>
     <li> Or could it be that the relatively large buffer is important
     here? Is it possible to increase the buffer-size of the standard
     library? </li>
     <li> Apparently not; so the first step would be, in the context of
     the C++ standard library(!), to create a new stream-buffer class, with
     just a larger buffer, and see how this improves speed. </li>
     <li> The Boost class boost::iostreams::stream_buffer allows to specify
     the buffer size in the constructor. </li>
     <li> Of course, especially for parsing integers we could make further
     simplifying assumptions. </li>
    </ol>
   </li>
   <li> One would assume that the head-tail structure of watched literals
   in the special case of pure UCP, where no assignment has to be undone, is
   superior (since here once the watched literals meet, there are no further
   literals to be considered!). </li>
   <li> At some time for the user there should be exactly one UCP-preprocessing
   tool (not two, as we have it yet). </li>
  </ul>

*/
