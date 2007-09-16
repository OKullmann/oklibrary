// Oliver Kullmann, 22.8.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/plans/TestBaseClass.hpp
  \brief Plans for the module with the (user) base of the test hierarchy

  \todo TestSystem::TestBase:
  - Use message-classes.
  - For the full-log-function: Perhaps m should be indented? And in extensive-level perhaps
    message-end should be indicated.
  - When putting the description on the log-stream (in perform_and_catch), then there should
    be a heading (at least for level full).
  - For the typename-output in perform_and_catch use demangling.

  \todo Unknown exceptions:
  In case an unknown exception is thrown, there should be a global option to
  let this exception through (perhaps kind of a "handler").

  \todo Encapsulation:
  It would be desirable to have TestBase::set_depth and TestBase::log_stream private
  (both functions are only used by OKLIB_TEST_RETHROW), since they belong only to the
  test-system. Perhaps we have a protected member-function in TestBase, which is used
  to call sub-tests, and this function then has access to the private data members log_p
  and depth_; the problem with that approach might be, that then it is harder to find out
  in case of a failing sub-tests, where really this sub-test has been called --- perhaps already
  calling this sub-test member-function, then the descriptions have to be given.

  \todo Testing
  Thoroughly test all functionality.

  \todo Transition:
  Once the transition is done, rename "TestBaseClass_DesignStudy.hpp" to
  "TestBaseClass.hpp", and check the doxygen-documentation (which yet doesn't work due
  to multiple definitions of the same entities).
  

*/

