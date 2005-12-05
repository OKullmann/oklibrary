// Matthew Henderson, 5.12.2005 (Swansea)

/*!
  \file Transition_Plans.hpp

  \todo When the OKplatform/include directory is created all subdirectories OKplatform/include/AutarkySearch,  OKplatform/include/Backup, OKplatform/include/BigIntegers, etc... are also created. This involves modifying Buildsystem/makefile. Alternatively, the relevant subdirectory is created only when the same subdirectory of Transitional is entered by the recursive make.
  \todo When headers get copied to the OKplatform/include directory, they should now be copied to the relevant subdirectory. This involves modifying Buildsystem/makefile_generic.
  \todo When compiling applications which include a certain header the relevant subdirectory must be passed to the -I option.
  \todo There needs to be some testing of the transition. Perhaps we write a dummy application which includes every header?

*/
