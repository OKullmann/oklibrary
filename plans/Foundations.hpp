// Oliver Kullmann, 14.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transitional/plans/Foundations.hpp
  \brief The general principles of the OKlibrary


  \todo Fundamental "axioms" DONE
  <ul>
   <li> There must be fundamental rules and "axioms" about the library
   and how to participate. </li>
   <li> DONE Important the research character. </li>
   <li> DONE SAT is considered at a very general level.
    <ol>
     <li> DONE Open for CSP (while maintaining the spirit of "generalised SAT", with emphasise
     on algorithms, not on language, and on partial assignments and clauses (for algorithms(!))). </li>
     <li> DONE Open for extensions like QBF. </li>
     <li> DONE Any applications included. </li>
     <li> DONE Though not "core business", for example graph and hypergraph algorithms and libraries
     are important, and are also part of the library. </li>
     <li> DONE While hypergraphs (which are just positive clause-sets) are very close to SAT, also
     areas like algebra and statistics are important for us, so the library supports developments
     also in these areas, but here much more emphasise is put on using external libraries. </li>
     <li> DONE Developing modern C++ is another area which is important in general, so supported
     by the library, but again emphasise on using external libraries (like Boost) as much as
     possible. </li>
     <li> DONE "Purely" theoretical algorithms are welcome. </li>
     <li> DONE All kind of "high flying" ideas are welcome --- however one of the main goals of the
     OKlibrary is the unity of SAT (and science in general), which concretely means that
     an outstanding goal is that all components work together through abstraction (by generic
     and generative programming). </li>
     <li> DONE Nearly everything can be encoded into SAT, and thus belongs potentially to the library. </li>
    </ol>
   </li>
   <li> DONE No "C code", but *modern C++ code*. </li>
   <li> DONE No other programming language (but many other systems like Coq or Sage). </li>
   <li> DONE The master repositories (with the names "OKlibrary", "OKplatform", "OKsystem")
   are owned by OK. Everybody can use a clone for his own development, thus naturally
   bound by the GPL licence, so the history must be open, but the name of the system
   must be changed. </li>
   <li> DONE Version numbers (of the library at all and of modules) are important for communicating
   the current standing, and their transition is solely the resonsibility of OK. </li>
   <li> Code which is compilable must be accompanied by (generic) tests! </li>
   <li> DONE The "virtual library" is the net of clones, which is of a fractal nature.
   In principle every point is the same (but "OK-site is the source" (of all evil)),
   everybody has the same power. (Yet we do not have experience with that --- so
   a lot needs to be learned. And definitely yet the building powers are not general
   enough (works yet only at the OK-site) --- but we are getting there!) </li>
   <li> DONE Extensive planning, and this within the OKlibrary:
    <ol>
     <li> Every action is announced by plans (in the corresponding plans-file), so that
     others know what's going on and can participate. Also only in this way the typical
     loss due to interruptions is avoided. </li>
     <li> These plans are continuously evolved (so that before programming starts,
     a good "loose specification" is available. </li>
     <li> Plans must be kept up-to-date, and also the corresponding milestone-entries. </li>
     <li> So much of the planning process happens *within* the OKlibrary (not, as usual, somewhere
     else). </li>
     <li> Mailinglists are for more informel discussions (finding out about interests, "chatting"
     about the plans (which is to be avoided in the plans-files)). So mailinglists are for
     "pre-discussion", and, potentially, "meta-discussions"; but as soon things concretise,
     they should move to the plans-files (only there they fully connect to the system). </li>
     <li> To summarise, the planning process is somewhat similar to "patchwork", which comes
     in small pieces, is always in a somewhat well-defined state, and can be interrupted
     and continued as needed. This might be especially true for core developers, while
     external developers might prefer a more traditional model, where only at the end
     the distilled work (using the "cherry-picking" method perferred for example by the
     Git developers) is submitted. However, after some initial period of getting used
     to the continuous process of the OKlibrary, also external developers shall be
     encouraged to use the "continuous model" (and not the "batch model"). </li>
    </ol>
   </li>
  </ul>

*/
