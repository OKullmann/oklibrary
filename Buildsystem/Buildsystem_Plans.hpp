// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem_Plans.hpp

  \todo makefiles in general should always have the suffix .mak,
  so that for examples xemacs regocnises the format.

  \todo Doxygen: Can makefiles be incorporated?!
  \todo ToDo: How to integrate a general todo list into Doxygen?!

  \todo MHash: Build a local version of mhash.

  \todo Optionally there should be also local versions of valgrind and doxygen.

  \todo Create a new target ("check_valgrind")
  which runs the test with "valgrind --leak-check=yes" (possibly with
  other options as well; perhaps as a default the option "--quit").

  \todo ExternalSources: all ToDo's

  \todo "Full test": makefile at the OKplatform level --- there the
  different versions of gcc and boost are visible. "make check" at
  this level should run through all appropriate combinations of gcc
  and libraries and test them.
  \todo makefile at level OKplatform: default goal should run "prebuild all".
  \todo Writing the package-construction script. (Internal versions should
  have date and time in the name.)
  \todo "Nightly build": Full check-out of the library (yet Transitional and 
  OKlibrary) and full compilation and testing (i.e., create the package,
  un-archive it, build it with "make" and then run "make check" in it).

  \todo Improving the directory structure (which yet is "flat"): Inclusion
  from for example Transitional should happen as 
  #include "OKlib/Transitional/SumProduct/Marginalisation.hpp".
  (Thus compilation of local copies of the library then only work with
  the *standard place* (i.e., OKplatform/include).)

  \todo As the link-libraries are handled now we also want to handle inclusion
  of the source-libraries and handling of the options. (Options should go
  into the name!)

  \todo Complexity system: "make measurements" will create an xml-file
  (via the boost serialisation library) with information about all
  operations which have been registered. A little viewing-program
  allows to monitor these measurements (as the evolve over time).
*/
