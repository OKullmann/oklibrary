// Matthew Henderson, 5.12.2005 (Swansea)

/*!

  \file Refactoring/plans/Transition_Plans.hpp

  \todo What is the status of this file???

  \todo In Buildsystem/makefile the creation of the include directory and
  copying of headers and makefiles to this directory has to be removed. 
  (L22,L25,L27,L31,L41)
  \todo In Buildsystem/makefile_generic the definition of includedir should 
  now point to $(prefix)/OKsystem/OKlibrary. The copying of headers on 
  lines 242-245 is no longer required.
  (L103,L110,L134,L137,L165,L242-L245)
  \todo Precise action plan needed.
  \todo Write application Transition.cpp which takes two command line 
  arguments, reference_directory and working_directory, and refactors all 
  the code contained in headers in working_directory according to the 
  directory structure in reference_directory.
  \todo Freeze development.
  \todo Checkout fresh copy of Transitional to prefix/OKsystem.
  \todo Build library and run make check.
  \todo make cleanall.
  \todo Create prefix/OKsystem/OKlibrary directory.
  \todo In OKsystem issue command : transition Transitional OKlibrary
  (where transition is the compiled application Transition.cpp).
  \todo In OKsystem/OKlibrary/Buildsystem swap names of makefile and 
  makefile_new and swap names of makefile_generic and makefile_generic_new.
  \todo Build library.
  \todo Add OKlibrary to Repository.
  \todo Add all subdirectories and files to OKlibrary module.
  \todo Commit all files.
  \todo Checkout fresh copy of OKlibrary, compile and test.
*/
