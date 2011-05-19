// Oliver Kullmann, 3.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Cryptology/plans/MHash.hpp
  \brief Plans regarding the usage of the MHash-library


  \todo Continuing with MHash ?
  <ul>
   <li> See "MHash segmentation fault" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Mhash.hpp. </li>
   <li> For now, the related applications have been disabled. </li>
   <li> We need to decide now (May 2011) what to do:
    <ol>
     <li> If there are really no new releases anymore, then we should abandon
     MHash. </li>
     <li> We need to find a substitution. </li>
     <li> Yet we only need md5. </li>
     <li> But finding a library (C++, of course) also offering AES and DES
     would be good (then we wouldn't need several libraries). </li>
     <li> See Structures/Cryptology/plans/general.hpp. </li>
    </ol>
   </li>
   <li> Update the usage of the MHash-library. </li>
   <li> DONE (no releases anymore)
   Are there newer version of the MHash-library? </li>
   <li> HashMD5.cpp shall become an application. </li>
   <li> What about "newHash" ? </li>
  </ul>


  \todo Computing the md5sum-value of a directory
  <ul>
   <li> With SplittingViaOKsolver (see
   Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp) we have a
   reasonable computation of the md5sum-value of a whole directory. </li>
   <li> Going through the files in numerical order, appending the
   current md5sum-value and computing the new md5sum-value. </li>
   <li> The shell-script however is very slow, and we need a C++
   application for it. </li>
   <li> The order of the files is a problem; see "The order of the
   md5sum-computation" in
   Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp. </li>
   <li> The question is whether the order of files in an arbitrary directory
   is really well-defined? </li>
   <li> When calling "ls", it could yield different results in different
   situations? </li>
   <li> We better use boost to get the filenames, and then sort them
   alphabetically (to simulate "ls", but in a reliable way). </li>
   <li> However it is a problem to first load all file-names into memory:
   there can be million of files? </li>
   <li> If there are 10^7 files, then it would need 10^8 bytes to store it
   (given that a character just needs a byte). </li>
   <li> But we need some wider characters here, and so likely it would need
   4*10^8 bytes (at least). </li>
   <li> So well, that still would be okay. We need to try it out. </li>
   <li> One would think that at most we have 10^8 files, which would make
   roughly 4*10^9 bytes; still feasible. </li>
  </ul>

*/
