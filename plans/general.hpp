// Oliver Kullmann, 6.6.2007 (Swansea)

/*!
  \file Transitional/plans/Transitional.hpp
  \brief Plans for the Transitional-library which do not belong to a single module

  \todo Upgrade to gcc-4.2.0
  <ul>
   <li> Correct the template-errors:
    <ol>
     <li> Precise problem description: XXX </li>
     <li> Methods to overcome the problem: XXX </li>
     <li> Files affected: XXX </li>
    </ol>
   </li>
  </ul>


  \todo Super-modules:
  <ul>
   <li> Currently 82 modules --- that needs a superstructure.
   So we should introduce sub-directories of Transitional,
   corresponding to "super-modules". </li>
   <li> Problematic the correspondence between paths and namespaces:
    <ol>
     <li> We could be consistent, using full super-module names
     and incorporating them into the namespaces.
      <ul>
       <li> Perhaps a collection
       of namespace-aliases could be provided ?! In this way we could use
       abbreviations (if wished) without loosing expressivity ?! </li>
       <li> Perhaps every namespace has a well-defined abbreviation, and there
       are aliases for all full namespace-qualifications ?! </li>
       <li> Better "OKlib" is not aliased, but only namespaces inside,
       so for example (defined inside namespace ::OKlib):
       \code
namespace Sat = Satisfiability;
namespace SatAlg = Satisfiability::Algorithms;
       \endcode
       </li>
       <li> In every plans-file, besides the namespace-documentation, where also
       the namespace is introduced (as C++ entity), we then have also the alias
       definitions. </li>
       <li> For the includes only the long forms can be used, but this seems to be
       alright. </li>
      </ul>
     </li>
     <li> Or we could make an exception, not incorporating
     the super-modules into the namespace-hierarchy. Perhaps
     we start the super-module-names with "_", and introduce the
     general rule that such directories do not participate in the
     namespace-hierarchy? </li>
     <li> Or we could incorporate them, but using 3-letter abbreviations. </li>
     <li> Possible super-modules:
      <ol>
       <li> SAL sat-algorithms </li>
       <li> ACS active clause-sets </li>
       <li> GEN general tools </li>
       <li> GRA graphs </li>
       <li> HYP hypergraphs </li>
       <li> ALG algebra </li>
       <li> THE theory (like Turing machines etc.) </li>
       <li> TES test system </li>
       <li> BUI build system </li>
       <li> COS complexity system </li>
       <li> HEU heuristics </li>
      </ol>
      A (serious) problems with these abbreviations is, that all-capital-identifiers should
      only be used for macros --- so better only the first letter capital.
     </li>
     <li> Perhaps the following "parts" of the OKlibrary (containing the super-modules):
      <ol>
       <li> (Acs,Heu,Sal) under "Satisfiability" ("Sat") </li>
       <li> (Gra,Hyp,Alg,The) under "Structures" ("Str") </li>
       <li> (Gen) under "Programming" ("Pro") </li>
       <li> (Tes,Bui,Cos) under "System" ("Sys") </li>
       <li> "DataStructures" ("Das") </li>
       <li> "Applications" ("App") </li>
       <li> "Deprecated" ("Dep") (obsolete parts) </li>
      </ol>
     </li>
     </li> 
    </ol>
   </li>
   <li> Are there refactoring tools? Or can we finally finish module Refactoring ?!? </li>
   <li> A feasible way for migration seems that we just slowly add new modules (which will
   be super-modules), and move modules into them (first only plans-only-modules, so that
   we don't get build-problems). For that we have to find out how to use "git mv" (see
   "Building and using Git" in Buildsystem/plans/VersionControl.hpp). </li>
  </ul>


  \todo Research sub-modules
  <ul>
   <li> To solve the privace-issues, it seems that (yet) only plans-directories
   are an issue, and one could move sensitive files to sub-directories
   "research" (moving also the whole history(!) --- see "Moving" in
   Buildsystem/plans/VersionControl.hpp). </li>
   <li> Currently I (OK) am inclined to release everything as it is (including
   "research secrets"), but this under the GPL: The OKplatform is a research
   platform, not a "tool" for industrial exploitation. Seeing that valuable
   research information is in the library, researchers will also add their
   own ideas. The GPL-licence together with the possibility to check whether
   a clone-repository has been corrupted should give enough security against
   the worst misusers. </li>
   <li> However, then perhaps Annotations simply stays a different repository. </li>
   <li> At least for version transitions, is it possible to store clones somewhere
   at an "official place" ?
    <ol>
     <li> What about that "SourceForge" ? </li>
     <li> Perhaps "SourceForge" is more for active participation in the "classical way"? </li>
     <li> Perhaps there are public scientific repositories? Or cultural repositories? </li>
     <li> At least we should keep a history of main releases (backed-up). </li>
     <li> Perhaps once we really go public (not yet with that "proto-release") we
     have a "mirror" somewhere. </li>
     <li> Perhaps actually the SAT community could provide a repository for SAT-related
     software?! </li>
     <li> But all this implies at least, that the external sources are not part
     of the release --- otherwise it gets too big. </li>
    </ol>
   </li>
  </ul>


  \todo Repository Annotations:
  <ul>
   <li> Perhaps we should move some parts to Transitional, namely
    <ol>
     <li> "FullDocumentation" </li>
     <li> the latex-support (needs clean-up) </li>
    </ol>
    Transitional/Annotations shall be the basis for writing documents and even
    joint papers on the library.
   </li>
   <li> "History_OKsystem" seems obsolete, and should be removed. </li>
   <li> "Release-1_0" perhaps should move (in some form) to Transitional/plans. </li>
   <li> Research articles stay in the separate repository "Annotations" (only for internal
   use). </li>
   <li> Then we have some problems regarding the duplication of macros and bibtex-entries ?
   Cleaning-up is needed:
    <ol>
     <li> What to do with Latex_macros/Basis.tex and Latex_macros/Definitionen.tex ? </li>
     <li> What to do with Latex_bib/EigArt.bib ? </li>
     <li> Perhaps I (OK) could re-organise my macros and bibtex-entries, and a "common"
     part is moved to Transitional/Annotations ?! </li>
    </ol>
   </li>
  </ul>


  \todo Organisation of plans-directories
  <ul>
   <li> The module-plan-files is just called "general.hpp". </li>
   <li> A problem with that is, that under xemacs all such files look alike --- but perhaps
   xemacs can be configured to show also the comprising directory? </li>
   <li> Another "systematic" file in plans-directories is "wishlist.hpp",
   which is especially for users the place where they can articulate
   wishes. (Which might not be realised --- general.hpp contains all
   wishes which are to be realised (first they unfold in general.hpp,
   and finally they got moved to their own plans-file).) </li>
  </ul>


  \todo "OKlib" vs. "Transitional"
  <ul>
   <li> Perhaps we should give up the distinction between "Transitional" and
   "OKlib", but rename "Transitional" to "OKlib", and remove the (empty)
   repository "OKlibrary". </li>
   <li> Likely it will be always like that, a lot of experimental stuff lying
   around? But then we need clear indications what modules are usable already
   --- this should be achieved by their version numbers.
   See "Doxygen menu and main page review" in Buildsystem/Generic/documentation_building/plans/documentation_building.hpp.
   </li>
  </ul>


  \todo Concepts
  <ul>
   <li> Currently all concepts are collected in module Concepts. Perhaps every module
   better should get its own sub-module "Module/concepts" ?! </li>
  </ul>


  \todo Demos:
  <ul>
   <li> In sub-directories "Module/demos" all demonstration programs will be put. </li>
   <li> The files in Module/demos should have full doxygen-documentation. </li>
   <li> Additionally the doxygen-command "\example" could be used for more fine-grained
   linking? </li>
   <li> Or is "\example" meant for mentioning .cpp-files without their own
   doxygen-documentation?? </li>
  </ul>


  \todo Computer algebra system:
  Mupad used to be free, and thus we have the Mupad-module.
  Now this is no longer the case (check again), and we need
  a replacement:
  <ol>
   <li> Sage (http://modular.math.washington.edu/sage/): I (OK) don't like the
   python-hype. Otherwise it looks interesting. The core seems to be written in C++.
   It also includes some other systems. Seems reasonably documented.
   For programming some language has to be learned anyway. So perhaps the first choice. </li>
   <li> Axiom (http://wiki.axiom-developer.org/FrontPage): Not bad; there is a book
   on it; but somewhat unclear what it can. </li>
   <li> Yacas (http://yacas.sourceforge.net): Not bad; again not clear what it
   can do. </li>
   <li> Maxima (http://maxima.sourceforge.net/): looks reasonable, but perhaps too
   lisp-ish. </li>
   <li> Singular (http://www.singular.uni-kl.de/): Interesting, but more specialised.
   Perhaps it can be used besides a general-purpose system? </li>
   <li> GiNaC is C++ (!) --- is it still under development? </li>
   <li> Check the licence for Mupad (http://www.mupad.de/): It seems a licence has to be
   purchased (and from my last experience with Mupad it seems also the Linux support is weak). DONE </li>
  </ol>

*/

/*!
  \namespace OKlib
  \brief All components of the OKlibrary
*/

namespace OKlib {
}





