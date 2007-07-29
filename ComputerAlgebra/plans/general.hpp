// Oliver Kullmann, 29.6.2007 (Swansea)

/*!
  \file ComputerAlgebra/plans/general.hpp
  \brief General plans regarding computer algebra (and SAT)


  \todo General principles
  <ul>
   <li> The computer-algebra-system is also based on generic
   concepts, which mirror the more refined C++ concepts. </li>
   <li> These concepts express (typically) the most general
   level, without all the distinctions regarding
   efficiency. </li>
   <li> The computer-algebra-system thus also serves as an
   experimental area for the design of concepts in the OKlibrary
   (regarding the "mathematical" concepts, not the programming ones). </li>
   <li> Concepts and (generic) algorithm for the OKlibrary are first
   prototyped in the context of the computer-algebra-system (if
   possible). </li>
  </ul>
  Likely these principles should go to ComputerAlgebra/docus/general.hpp.


  \todo How to use Sage?
  <ul>
   <li> One gets the impression that Sage does not really support
   using the underlying systems, for example Maxima ? </li>
   <li> So perhaps it would be easier for us (at least at this
   stage) to not use Sage, but install Maxima ourselves? </li>
   <li> How can we customise Sage from within the OKlibrary? </li>
   <li> If we have code for Maxima, then obviously we use
   \verbatim
sage: maxima_console()
   \endverbatim
   So what use is Sage at all here? Isn't it just in the way, making
   it harder (or impossible) to configure Maxima properly? </li>
  </ul>


  \todo %Test system
  <ul>
   <li> Is there some special test system? </li>
  </ul>


  \todo Input checking
  <ul>
   <li> How to handle input checking/type checking? </li>
  </ul>


  \todo Partial compilation
  <ul>
   <li> Is there compilation? </li>
   <li> Is there partial compilation (using special properties to
   produce more efficient code)? </li>
  </ul>


  \todo Abstraction
  <ul>
   <li> How to handle abstraction? </li>
   <li> Can we abstractly specify types, functions ? </li>
  </ul>


  \todo Contexts
  <ul>
   <li> Is there the notion of a "context" ("block") ?
  </ul>

*/

