// Oliver Kullmann, 6.6.2007 (Swansea)

/*!
  \file Transitional/plans/general.hpp
  \brief Plans for the Transitional-library which do not belong to a single module


  \todo License:
  <ul>
   <li> Due to the research focus, we should use the GPL:
   http://www.fsf.org/licensing/licenses/gpl.html </li>
   <li> Every file gets a short version as header, with copyright statement
   and reference to the main text. </li>
   <li> What about the copyright statements?
    <ol>
     <li> Initially every file has OK's copyright. </li>
     <li> But once other external developers have contributed essentially to some file,
     then perhaps they should also have copyright on this file (that is, the corresponding
     group leader)? Perhaps we should ask the FSF how to handle such situations. </li>
     <li> Perhaps for legal reasons it is not feasible to have such a fractured
     copyright-situation, but then at least perhaps in every plans-directory we have
     a files "contributors.hpp" where just the developers are mentioned which contributed
     to the module (they enter themselves)? </li>
    </ol>
   </li>
   <li> Part of the licence must be that the full history is handed down. Is this
   guaranteed by the original text, or do we need to amend it? </li>
   <li> When external developers check in new files, then those must have the OK-copyright
   etc. clause (otherwise the submission is rejected). </li>
   <li> The head of every file would look like as follows:
   \verbatim
// Name of file creator, date of file creation (place of file creation)
// Copyright 20XX Oliver Kullmann
// This file is part of the OKlibrary.
//  OKlibrary is free software; you can redistribute it and/or modify it under the terms of
//  the GNU General Public License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//  The OKlibrary is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details. You should have received a copy of the
//  GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
   \endverbatim
   where "XX" is to be replaced by the current year. </li>
   <li> For every release, the year of the release must be added to every file's copyright notice
   (if not already present). </li>
   <li> Download GPLv3 and put under Configuration. DONE </li>
   <li> It seems LGPL is suitable; we have to check this (is it compatible
   with all other licenses (for external sources) involved? I guess so). DONE (for the research-platform, where everybody contributes ideas, we need stronger protection) </li>
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


  \todo Completing the repository
  <ul>
   <li> The new repository would have a directory structure
    <ol>
     <li> OKplatform/ </li>
     <li> OKplatform/system_directories </li>
     <li> OKplatform/ExternalSources </li>
     <li> OKplatform/ExternalSources/installations </li>
     <li> OKplatform/ExternalSources/doc </li>
     <li> OKplatform/ExternalSources/Sources </li>
     <li> OKplatform/OKsystem </li>
     <li> OKplatform/OKsystem/OKlib </li>
    </ol>
    where OKplatform/OKsystem/OKlib would be the current
    OKplatform/OKsystem/Transitional.
   </li>
   <li> Potentially the build-system could be moved to
   OKplatform/OKsystem/BuildSystem ? Or the whole new super-module
   "System" (see below) ?? However the test system belongs to the OKlibrary.
   Only the build-system does not contain C++ code, and thus could be moved
   outside the OKlibrary. </li>
   <li> So potentially additionally OKplatform/OKsystem/BuildSystem. </li>
   <li> Discussion of the advantages and disadvantages of such a completed ("higher")
   directory structure in the repository:
    <ul>
     <li> Advantages:
      <ol>
       <li> The full directory structure is fixed and explicit. </li>
       <li> It is easier to exploit the full directory structure. </li>
       <li> Package building becomes (a bit) easier. </li>
      </ol>
     </li>
     <li> Disadvantages:
      <ol>
       <li> The full directory structure is fixed. </li>
       <li> As know, "projecting the directory structure in the environment", we are
       more flexible. </li>
       <li> The Git-history-interruption (due to the move) is somewhat ugly. </li>
       <li> Git only notices a directory if it has some (indexed) file in it. </li>
       <li> It seems unusual (so well). </li>
      </ol>
     </li>
    </ul>
   </li>
  </ul>


  \todo Super-modules:
  <ul>
   <li> Currently 82 modules --- that needs a superstructure.
   So we should introduce sub-directories of Transitional,
   corresponding to "super-modules". </li>
   <li> Problematic the correspondence between paths and namespaces:
    <ol>
     <li> We should be consistent, using full super-module names
     and incorporating them into the namespaces.
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
    </ol>
   </li>
   <li> Perhaps the following "parts" of the OKlibrary (containing the super-modules; abbrevations
   shall not be all-caps (reserved for macros)):
      <ol>
       <li> "Satisfiability" ("Sat")
        <ol>
         <li> Alg ("Algorithms"; (generic) algorithms for SAT solving and variations) </li>
          <ol>
           <li> LSe ("LocalSearch") </li>
          </ol>
         </li>
         <li> Asg ("Assignments")
          <ol>
           <li> Aut ("Autarkies") </li>
           <li> pAs ("PartialAssignments") </li>
          </ol>
         </li>
         <li> Heu ("Heuristics") </li>
         <li> Ifs ("Interfaces"; input, output etc.) </li>
         <li> PrI ("ProblemInstances")
          <ol>
           <li> Var ("Variables") </li>
           <li> Lit ("Literals") </li>
           <li> Cla ("Clauses") </li>
           <li> Cls ("ClauseSets") </li>
           <li> aLi ("ActiveLiterals") </li>
           <li> aCL ("ActiveClauses") </li>
           <li> aCS ("ActiveClauseSets") </li>
           <li> AaC ("AlliancesActiveClauseSets") </li>           
          </ol>
         </li>
         <li> PSy ("ProofSystems") </li>
         <li> Qua ("Quantification") </li>
         <li> Red ("Reductions") </li>
         <li> Sol ("Solvers"; contains "OKsolver") </li>
         <li> SpS ("SpecialStructures") </li>
         <li> Tra ("Transformers")
          <ol>
           <li> Gen ("Generators") </li>
          </ol>
         </li>
        </ol>
       </li>
       <li> "Applications" ("App")
        <ol>
         <li> CrA ("Cryptanalysis") </li>
         <li> Col ("Colouring")
          <ol>
           <li> FrA ("FrequencyAssignment") </li>
          </ol>
         </li>
         <li> Emb ("Embeddings") </li>
         <li> Gam ("Games") </li>
         <li> Lat ("LatinSquares") </li>
         <li> QuP ("QuantumPhysics") </li>
         <li> Ram ("RamseyTheory") </li>
        </ol>
       </li>
       <li> "Experimentation" ("Exp")
        <ol>
         <li> Dab ("Database") </li>
         <li> ExS ("ExperimentSystem") </li>
         <li> RaG ("RandomGenerator") </li>
         <li> SaC ("SatCompetition") </li>
        </ol>
       </li>
       <li> "Combinatorics" ("Com")
        <ol>
         <li> Gra ("Graphs") </li>
         <li> Hyp ("Hypergraphs") </li>
        </ol>
       </li>
       <li> "Structures" ("Str")
        <ol>
         <li> Alg ("Algebra") </li>
         <li> Ord ("OrderTheory") </li>
         <li> Num ("NumberTheory") </li>
        </ol>
       </li>
       <li> "Logic" ("Log")
        <ol>
         <li> CTh ("ComplexityTheory") </li>
         <li> Cpa ("Computability") (contains "TuringMachines" TuM) </li>
         <li> fMT ("FiniteModelTheory") </li>
         <li> aTP ("AutomatedTheoremProving") (general frameworks) </li>
         <li> FiO ("FirstOrderLogic") (here for example the decidable cases,
         and the resolution calculus) </li>
         <li> AlS ("AlgebraicSpecification") (CASL etc.) </li>
        </ol>
       </li>
       <li> "Programming" ("Pro")
        <ol>
         <li> Gen ("GeneralTools") </li>
         <li> Pll ("Parallelism") </li>
        </ol>
       </li>
       <li> "System" ("Sys")
        <ol>
         <li> CoM ("ComplexityMeasurement") </li>
         <li> TsS ("TestSystem") </li>
         <li> Bui ("Buildsystem") </li>
        </ol>
       </li>
       <li> "DataStructures" ("DaS") </li>
       <li> "ComputerAlgebra" ("CAl")
       </li>
       <li> "LogicalReflection" ("LoR")
        <ol>
         <li> PrE ("ProgramExtraction") </li>
         <li> FoP ("FormalisedProofs") (contains the framework for formalised
         upper bound proofs) </li>
         <li> PrA ("ProofAssistants") (contains sub-modules Coq and Isabelle
         for interfacing and other tasks) </li>
         <li> VrC ("VerifiedComponents") </li>
        </ol>
       </li>
       <li> "Statistics" ("Sta")
        <ol>
         <li> R </li>
        </ol>
       </li>
       <li> "Optimisation" ("Opt")
        <ol>
         <li> LiP ("LinearProgramming") </li>
         <li> QuP ("QuadraticProgramming") </li>
         <li> SeP ("SemidefiniteProgramming") </li>
        </ol>
       </li>
       <li> "Numerical" ("Num") </li>
       <li> "Visualisation" ("Vis")
        <ol>
         <li> GDr ("GraphDrawing") </li>
        </ol>
       </li>
       <li> "UserInterfaces" ("UIc")
        <ol>
         <li> For researchers to create solvers (by assembling
         given components). </li>
         <li> Helping with the complex configuration of generic
         components. </li>
         <li> Headquarters for experimentation 
          <ol>
           <li> database access etc. </li>
           <li> monitoring experiment execution </li>
           <li> experiment planning </li>
          </ol>
         </li>
        </ol>
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


  \todo Computer algebra system: DONE (decided to use SAGE)
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





