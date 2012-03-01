// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Mhash.hpp
  \brief Plans regarding installation of the Mhash library


  \bug MHash segmentation fault
  <ul>
   <li> Building with gcc version 4.4.1 on a 64-bit machine we get
   \verbatim
/bin/sh: line 4:  9858 Segmentation fault      ${dir}$tst
FAIL: keygen_test
   \endverbatim
   </li>
   <li> This is a known bug in Mhash; see for example
   https://bugzilla.redhat.com/show_bug.cgi?id=513254 . </li>
   <li> Unfortunately, according to
   http://sourceforge.net/projects/mhash/files/ , the project is from
   2007-08-18 0:00:00 GMT not under active development. </li>
   <li> So, either we fix the bug ourselves, or we stop using mhash. </li>
   <li> In ExternalSources/sources/Mhash/mhash-0.9.9.9-1.fc11.src.rpm
   apparently we have the mhash-package plus patches. </li>
   <li> The question is, how do we extract the package and the patches, and
   apply then the patches?
    <ul>
     <li> This can be done using the "rpm2cpio" tool at
     http://cblfs.cross-lfs.org/index.php/Rpm2cpio :
     \verbatim
ExternalSources/sources/Mhash> rpm2cpio mhash-0.9.9.9-1.fc11.src.rpm | cpio -idmv
Mhash> for p in *.patch; do md5sum $p; done
339a340b9c8685030ac2cc1aac5bea7f  mhash-0.9.9.9-alignment.patch
2d28da083e9e22ac4735097e943d990b  mhash-0.9.9.9-align.patch
f2a3978293d598a6f8211eca8eccc7bb  mhash-0.9.9.9-autotools-namespace-stomping.patch
ac111f673f3b21f5eadcd8155c207d96  mhash-0.9.9.9-fix-mem-leak.patch
c779d428e6c7cf255009465580713664  mhash-0.9.9.9-fix-snefru-segfault.patch
efc1497e6e77fb441a73ec496e76f488  mhash-0.9.9.9-fix-whirlpool-segfault.patch
b7860fed495cd6145a67870630b4975a  mhash-0.9.9.9-force64bit-tiger.patch
4bd100f0e2deaec0ddf8682558836940  mhash-0.9.9.9-keygen_test_fix.patch
2062e7357e22db1edd1bfd47011152f9  mhash-0.9.9.9-maxint.patch
     \endverbatim
     </li>
     <li> Likely we just want the patch files, and so there is no need
     to install the rpm2cpio tool. </li>
     <li> Now building Mhash, where
     Buildsystem/ExternalSources/SpecialBuilds/mhash.mak has been changed so
     that
     \verbatim
        cd $(mhash_build_dir_okl); $(postcondition) \
	./configure --prefix=$(mhash_installation_dir_okl) --with-CC=$(gcc412_call_okl); $(postcondition) \
     \endverbatim
     becomes
     \verbatim
        cd $(mhash_build_dir_okl); $(postcondition) \
        cp $$(dirname $(mhash_source_dir_okl))/*.patch .; $(postcondition) \
        for p in *.patch; do patch -p1 < $${p}; done; $(postcondition) \
	./configure --prefix=$(mhash_installation_dir_okl) --with-CC=$(gcc412_call_okl); $(postcondition) \
     \endverbatim
     means we no longer get errors during "oklib mhash" in ExternalSources. </li>
     <li> After applying these patches, OKlibrary applications which were
     previously disabled now compile against Mhash and run correctly. </li>
     <li> That is, re-enabling HashMD5 in Structures/Cryptology/, HashMD5
     builds correctly, and runs:
     \verbatim
Structures/Cryptology> oklib all
Structures/Cryptology> HashMD5-O3-DNDEBUG definitions.mak
Hash:7d0d9cf937736417d796482bc3a00a2c
Structures/Cryptology> md5sum definitions.mak
7d0d9cf937736417d796482bc3a00a2c  definitions.mak
     \endverbatim
     </li>
    </ul>
   </li>
   <li> For now we stop installing and using it. </li>
  </ul>


  \todo Complete MHash documentation
   
*/

