// Oliver Kullmann, 9.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/GAP.hpp
  \brief Plans regarding building of the GAP system

  
  \todo Installing GAP
  <ul>
   <li> Basic installation (in ExternalSources/builds)
   \verbatim
builds/GAP> tar -xjf ../../sources/Gap/gap4r4p10.tar.bz2
builds/GAP> cd gap4r4/pkg
builds/GAP/gap4r4/pkg> tar -xjf ../../../../sources/Gap/packages-2008_06_23-19_20_UTC.tar.bz2
builds/GAP/gap4r4/pkg> cd ..
builds/GAP/gap4r4> ./configure
builds/GAP/gap4r4> make 
builds/GAP/gap4r4> cp bin/gap.sh ../../../../bin/gap
builds/GAP/gap4r4> cd pkg
builds/GAP/gap4r4/pkg> cp ../../../../sources/Gap/InstPackages.sh .
builds/GAP/gap4r4/pkg> chmod u+x InstPackages.sh


   \endverbatim
   This worked smoothly.
   </li>
   <li> Can we have a different installation directory (as usual for us)? </li>
   <li> What about this "xtom1r1p2.tar.bz2" ? </li>
   <li> Running the tests (manually) also works (and likely we don't need to do
   something about it). </li>
  </ul>


  \todo Monoids
  <ul>
   <li> DONE (included in GAP)
   What about "Monoid" http://schmidt.nuigalway.ie/monoid/ ? </li>
   <li> Is this contained in GAP ?
   \verbatim
gap> LoadPackage("monoid");
dirs := DirectoriesPackageLibrary( "monoid", "tst" );;
Read(Filename( dirs, "installtest.g" ) );
   \endverbatim
   works fine. </li>
  </ul>


  \todo GRAPE
  <ul>
   <li> Using
   \verbatim
gap> LoadPackage( "grape" );
gap> IsIsomorphicGraph( JohnsonGraph(7,3), JohnsonGraph(7,4) );
  true
gap> Size( AutGroupGraph( JohnsonGraph(5,2) ) );
  120
   \endverbatim
   works fine!
   </li>
  </ul>

*/

