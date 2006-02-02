// Oliver Kullmann, 17.12.2005 (Swansea)

/*!
  \file FailedLiteralReduction_Plans.hpp
  \brief Plans for the module FailedLiteralReduction
  \todo Transferring the implementation used in OKsolver.
  \todo The (old) OKsolver used a timestamp (see Utilities/
  Timestamp.hpp), incremented with each new literal processed
  (attempting to find a failed literal), which was used in two ways:
   - assignments to variables used this timestamp, so that for a
     new round (with a new envisaged failed literal) assignments
     don't need to be undone);
   - When processing literal x, all implied literals y where marked
     with the timestamp, so that when processing y, where meanwhile
     no failed literal has been detected, we know that we can skip y.
*/

