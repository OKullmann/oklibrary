// Oliver Kullmann, 3.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComplexitySystem/plans/general.hpp
  \brief Plans for the complexity system


  \todo Update namespaces.


  \todo Timing the Maxima tests and the application tests
  <ul>
   <li> Currently we have on csltok (with standard frequency 2.4GHz):
   \verbatim
> time oklib app_tests
real    0m17.889s
user    0m9.686s
sys     0m5.290s
> time oklib new_check
real    4m53.804s
user    4m35.116s
sys     0m16.470s
> time oklib new_check app_tests
real    5m9.457s
user    4m44.035s
sys     0m21.836s

> time oklib app_tests test_level=full
real    18m55.624s
user    18m43.219s
sys     0m8.319s
> time oklib new_check test_level=full
real    79m50.318s
user    79m16.117s
sys     0m16.927s
> time oklib new_check app_tests test_level=full
real    97m17.190s
user    96m37.758s
sys     0m24.457s

> time oklib app_tests test_level=extensive
real    14m4.718s
user    13m55.285s
sys     0m6.125s
> time oklib new_check test_level=extensive
real    113m10.107s
user    112m34.656s
sys     0m20.518s

   \endverbatim
   </li>
   <li> The complexity system needs also to include these timings. </li>
  </ul>


  \todo Designing the file structure
  <ul>
   <li> In principle similar to the (new) test system. </li>
   <li> For every Module as we have now sub-directories
   "tests" and "testobjects", we get sub-directories
   "measurements" and "measurementobjects", and for
   every Submodule we get measurements/Submodule.hpp
   and measurementobjects/Submodule.hpp. </li>
   <li> In measurements/Submodule.hpp for every class X
   as we have now the class template tests::X we get
   the class template measurements::X. </li>
   <li> There is a measure function in measurements::X which returns
   an event description and the time measured (time in seconds). </li>
   <li> What is measured is actually a simple test (so that something
   is computed, and the action to be timed is not optimised away;
   repetitions of the same computations should be avoided). </li>
  </ul>


  \todo Designing the level system
  <ul>
   <li> Also the (new) level system is the same as for
   the test system. </li>
  </ul>


  \todo Designing the makefile
  <ul>
   <li> There is a timestamp-file "measurement" for each
   of the three levels. </li>
  </ul>
 

  \todo Designing measurement classes
  <ul>
   <li> Measurements are instances of a wrapper class
   around boost::timer objects but with extra data members for
    <ol>
     <li> compiler version, </li>
     <li> %boost version, </li>
     <li> time and date. </li>
    </ol>
   OK: I think that this information should be global (for the
   whole container of measurement objects), not just for
   one measurement.
   </li>
   <li> To test a function f in SubmoduleClass of Submodule,
   create a class, derived from ComplexityBaseClass, with a private
   member function take_measurements. (Also with public measure_type
   and constructor providing self insertion.)
   <li> BUT, self insertion has been replaced for the test system (or will be),
   and perhaps also here we should not mix the functionalities of measuring
   and storing the measure objects. So look at the new test system for the role model. </li>
   <li> Measurement classes have an %end() public member function
   which gets the elapsed time from the boost::timer and passes
   this to the measurement handling policy. </li>
   <li> BUT, don't call it "end()" (because of "begin() + end()").
   And actually we might not need such a function, or? </li>
   <li> For example, to test f in SubmoduleClass of Submodule
   do something like the following:
   \code
class Measure_SubmoduleClass : public ::OKlib::ComplexitySystem::ComplexityBase {
  public :
    typedef Measure_SubmoduleClass measure_type;
    Measure_SubmoduleClass() {
      insert(this); // OK: obsolete
    }
  private :
    void take_measurements() {
      measurement<SaveToFile> timer; // OK: see below (we should do it globally)
      for (int i=0; i<100; ++i) { // OK: such repetitions are dangerous (optimised away)
        f();  // Function to be measured
      }
      measurement.end(); // Measurement finished. Handling of
                         // measurement by policy begun. // OK: not needed (return value)
    }
   };
   \endcode
   </li>
   <li> As said above, we should avoid calling a function repeatedly without
   doing something. And it seems reasonable to me that take_measurements
   returns the results. </li>
   <li> It seems that the idea of MH is, that every measurement object can
   place its results where it wants to, while I think that a simpler design is
   enough, where we have a container of measurement objects, the measurement
   objects deliver their individual results, and then a container with all
   results and all additional information is dumped to a file. (Like with the
   test system, but this time we need to store it.) </li>
  </ul>


  \todo Precision of measurements
  <ul>
   <li> Above we already mentioned the problem with the compiler; here now
   we consider how to measure the precise run-time (of whatever). </li>
   <li> There was a discussion midst of January 2009 at the general ACCU
   mailing list, started by the Minion group, on how to make precise
   measurements. </li>
   <li> Really counting processor cycles can be done by Valgrind.
    <ol>
     <li> This can mean a slowdown by a factor of, say, 50. </li>
     <li> But large parts of the complexity system are about small
     processes, so this slowdown shouldn't be a problem. </li>
     <li> But the cache issue doesn't seem to be addressed here? </li>
    </ol>
   </li>
   <li> Exclusion of other processes can happen by giving the measurement
   process a high priority.
    <ol>
     <li> But since the whole measurement process can take a while, this
     might disturb substantially the system. </li>
     <li> So likely this should only be done on systems with at least 2
     processors. </li>
    </ol>
   </li>
   <li> What to do about the cache behaviour?
    <ol>
     <li> There is the notion of a "cold start"? Is this related to cache
     behaviour? </li>
     <li> Again, Valgrind can help here, by the tool "Cachegrind" (and
     also Callgrind). </li>
     <li> However, this does not measure something, but annotate
     the program. </li>
     <li> So perhaps with respect to cache behaviour we only try to make
     sure, that the measurement runs happen under "normal" conditions,
     and otherwise we leave it to special considerations.
    </ol>
   </li>
  </ul>


  \todo Designing measurement accumulation
  <ul>
   <li> Here the results of measurements are accumulated, and output
   (to a file, and/or other streams). </li>
   <li> How do we specify a filename for the measurements to be saved to?
    <ol>
     <li> OK: All this kind of information should be input to the measurement
     *program* and stored in the measurement *container* (which is serialised
     to a file). </li>
     <li> It's like the (new) test system: We have the information at the
     makefile-level, then at the measurement-program-level, then at the level
     of the files in directories measurements and measurementsobjects, then
     at the level of the single measurement classes, and finally at the level
     of the single measurement objects. </li>
    </ol>
   </li>
  </ul>

*/

/*!
  \namespace OKlib::ComplexitySystem
  \brief Module for (persistent) complexity measurements
*/

namespace OKlib {
  namespace ComplexitySystem {
  }
}

