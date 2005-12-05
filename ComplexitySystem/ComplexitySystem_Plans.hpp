// Oliver Kullmann, 3.12.2005 (Swansea)

/*!
  \file ComplexitySystem_Plans.hpp
  \brief Plans for the complexity system
  \todo In principle similar to the test system:
  For every Module as we have now TestModule.cpp
  we get MeasureModule.cpp, and for every Submodule
  as we have now Submodule_Tests.hpp and
  Submodule_Testobjects.hpp we get
  Submodule_Measurements.hpp and
  Submodule_Measurementobjects.hpp.
  In Submodule_Measurements.hpp for every class X
  as we have now the class template Test_X we get
  the class template Measure_X. As Test_X is derived
  from a base class providing auto-insertion in a
  global container, Measure_X shall be derived
  from a base class providing auto-insertion into
  a global container.
  The main function of Measure_X then is to make calls
  event(time, description) in order to notify the base class
  about measured execution times (time in seconds).
  What is measured is actually a simple test (so that something
  is computed, and the action to be timed is not optimised away;
  repetitions of the same computations should be avoided).
  \todo Also the (new) level system is the same as for
  the test system.
  \todo There is a timestamp-file "measurement" for each
  of the three levels.
*/

