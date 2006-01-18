// Oliver Kullmann, 3.12.2005 (Swansea)

/*!
  \file ComplexitySystem_Plans.hpp
  \brief Plans for the complexity system
  \todo In principle similar to the test system:
  For every Module as we have now TestModule.cpp
  we get MeasureModule.cpp, and for every Submodule
  as we have now Submodule_Tests.hpp and
  Submodule_Testobjects.cpp we get
  Submodule_Measurements.hpp and
  Submodule_Measurementobjects.cpp.
  In Submodule_Measurements.hpp for every class X
  as we have now the class template Test_X we get
  the class template Measure_X.
  There is a measure function in Measure_X which returns
  an event description and the time measured (time in seconds).
  What is measured is actually a simple test (so that something
  is computed, and the action to be timed is not optimised away;
  repetitions of the same computations should be avoided).
  \todo Also the (new) level system is the same as for
  the test system.
  \todo There is a timestamp-file "measurement" for each
  of the three levels.
 
  \todo To test a function f in SubmoduleClass of Submodule
  create a class, derived from ComplexityBaseClass, with a private
  member function take_measurements. (Also with public measure_type
  and constructor providing self insertion.)
  OK: self insertion has been replaced for the test system (or will be),
  and perhaps also here we should not mix the functionalities of measuring
  and storing the measure objects.

  \todo Measurements are instances of a wrapper class
  around boost::timer objects but with extra data members for
    - compiler version, 
    - boost version, 
    - time and date.
  OK: I think that this information should be global (for the
  whole container of measurement objects), not just for
  one measurement.

  \todo Measurement classes have a template parameter for a 
  measurement handling policy.
  OK: For what is this policy good for?

  \todo Measurement classes have an end() public member function
  which gets the elapsed time from the boost::timer and passes
  this to the measurement handling policy.
  OK: Don't call it "end()" (because of "begin() + end()").
  And actually we might not need such a function, or?

  \todo Create several policies for handling measurement
  objects. For instance, SaveToFile, OutputAsError, ...
  OK: I think such a policy should be responsible for a whole
  container of measurement objects (all gathered in the run).

  \todo For example, to test f in SubmoduleClass of Submodule
  do something like the following:
  
  class Measure_SubmoduleClass : public ::OKlib::ComplexitySystem
    ::ComplexityBase {
  public :
    typedef Measure_SubmoduleClass measure_type;
    Measure_SubmoduleClass() {
      insert(this);
    }
  private :
    void take_measurements() {
      measurement<SaveToFile> timer;
      for (int i=0; i<100; ++i) {
        f();  // Function to be measured
      }
      measurement.end(); // Measurement finished. Handling of
                         // measurement by policy begun.
    }
   };
   OK: As said above, we should avoid calling a function repeatedly without
   doing something. And it seems reasonable to me that take_measurements
   returns the results.
   It seems that the idea of MH is, that every measurement object can place
   its results where it wants to, while I think that a simpler design is
   enough, where we have a container of measurement objects, the measurement
   objects deliver their individual results, and then a container with all
   results and all additional information is dumped to file.

  \todo How do we specify a filename for the measurements to be saved
  to?
  OK: All this kind of information should be input to the measurement
  *program* and stored in the measurement *container* (which is serialised
  to a file).

*/
