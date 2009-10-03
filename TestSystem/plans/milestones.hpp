// Oliver Kullmann, 22.8.2006 (Swansea)
/* Copyright 2006 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/plans/milestones.hpp

  \module_version TestSystem 0.1.3 (6.2.2007)


  \par Version 0.1.4 :

  In TestSystem/plans/TestBaseClass.hpp the following topics are handled:
   - TestSystem::TestBase

  In TestSystem/plans/TestProgram.hpp the following topics are handled:
   - Messages
   - Infos


  \par Version 0.1.5 :

  In TestSystem/plans/TestExceptions.hpp the following topics are handled:
   - More error-information
   - Test TestSystem::messages::ErrorDescription
   - OKLIB_TEST_RETHROW_NO_LOG
   - Transition
   - An overhaul of the test-macros (like OKLIB_TEST_EQUAL) seems to be needed

  Expand and update the tests, so that the test system is thoroughly tested.


  \par Version 0.1.6 :

  In TestSystem/plans/TestBaseClass.hpp the following topics are handled:
   - Unknown exceptions
   - Encapsulation
   - Testing
   - Transition

  In TestSystem/plans/general.hpp the following topics are handled:
   - Update namespace
   - Documentation
   - Build system


  \par Version 0.2 :
  The new test system is ready to use.

  

  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.1 :   24.8.2006, initial version number (the old test system is
     working, while the new test system is being established).
   - 0.1.1 : 30.8.2006, plans and todos have been updated, and initial doxygen documentation provided.
   - 0.1.2 : 5.12.2006, cleaned up creation of log-messages, and other editorial work.
   - 0.1.3 : 6.2.2007, the various messages output by the test system are now realised via the Messages-framework, and various macros to ease creation of tests have been implemented.

*/

