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
     <li> The same patch files are available from a Fedora git repository
     at http://pkgs.fedoraproject.org/gitweb/?p=mhash.git :
     \verbatim
ExternalSources/sources/Mhash> git clone git://pkgs.fedoraproject.org/mhash.git
ExternalSources/sources/Mhash> for x in *.patch; do diff $x mhash/$x; done
     \endverbatim
     This repository contains only the patch files, a "spec" file, and
     an md5sum checksum. </li>
     <li> After applying these patches, OKlibrary applications which were
     previously disabled now compile against Mhash and run correctly:
     \verbatim
# Apply patches and create new tarball
ExternalSources/sources/Mhash> tar jxvf mhash-0.9.9.9.tar.bz2
ExternalSources/sources/Mhash> git clone git://pkgs.fedoraproject.org/mhash.git mhash-fedora
# Apply latest patches; note that mutils-align.patch doesn't apply to
# mhash-0.9.9.9
ExternalSources/sources/Mhash> cp mhash-fedora/*.patch mhash-0.9.9.9/.
ExternalSources/sources/Mhash> cd mhash-0.9.9.9
ExternalSources/sources/Mhash/mhash-0.9.9.9> for p in mhash-0.9.9.9-*.patch; do patch -p1 < $p; done
ExternalSources/sources/Mhash/mhash-0.9.9.9> cd ../
ExternalSources/sources/Mhash> mv mhash-0.9.9.9.tar.bz2{,-orig}
ExternalSources/sources/Mhash> tar jcvf mhash-0.9.9.9.tar.bz2 mhash-0.9.9.9
ExternalSources/sources/Mhash> cd ../../
ExternalSources/> oklib cleanallmhash mhash
# No error occurs


# Re-enable disabled Mhash-based programs
OKlib> git log --name-status -r d423e6377697ffa332aeb5dc9b436dcad1a62c1d^..d423e6377697ffa332aeb5dc9b436dcad1a62c1d 
commit d423e6377697ffa332aeb5dc9b436dcad1a62c1d
Author: Oliver Kullmann <O.Kullmann@Swansea.ac.uk>
Date:   Sat May 22 19:02:33 2010 +0100

    Disabled building and using Mhash.

    Due to a bug in Mhash (and since we do not really need it now).

M       Buildsystem/Configuration/ExternalSources/all.mak
M       Buildsystem/ExternalSources/SpecialBuilds/plans/Mhash.hpp
M       Buildsystem/ExternalSources/SpecialBuilds/plans/milestones.hpp
D       Structures/Cryptology/HashMD5.cpp
A       Structures/Cryptology/HashMD5.cpp_disabled
D       Structures/Cryptology/HashMD5lib.cpp
A       Structures/Cryptology/HashMD5lib.cpp_disabled
M       Structures/Cryptology/definitions.mak
M       Structures/Cryptology/plans/general.hpp


OKlib> mv Structures/Cryptology/HashMD5.cpp{_disabled,}
OKlib> mv Structures/Cryptology/HashMD5lib.cpp{_disabled,}

# Build programs
OKlib> cd Structures/Cryptology
Structures/Cryptology> oklib all
# Builds correctly.

# Test programs
Structures/Cryptology> HashMD5-O3-DNDEBUG < definitions.mak
Hash:2927db35dd111162c18e040673c932af
Structures/Cryptology> md5sum definitions.mak
2927db35dd111162c18e040673c932af  definitions.mak
     \endverbatim
     </li>
     <li> The git repository at the official Mhash sourceforge project
     http://sourceforge.net/projects/mhash/develop contains
     Mhash-0.9.9.9 with additional editorial corrections:
     \verbatim
# Checking that changes in Mhash repository are editorial
ExternalSources/sources/Mhash> git clone git://mhash.git.sourceforge.net/gitroot/mhash/mhash
ExternalSources/sources/Mhash> cd mhash
ExternalSources/sources/Mhash/mhash> git log --pretty=oneline
42e331f294e341ef61d9e307226f5a903e04337f Update .gitignore
c25104c6751852c6f115577f11513b8e22faf4d6 Don't track .Plo files
bfe19776b4d80a1c2d516daa1033480e0b02912e Refining ignore files
ac5bc6cf99b6828bcf61a61e731e5bcbcf51aff9 Removed files resulting from configure, removed Makefile.in from ignore files, and added the Makefile.in files.
21da4869cd2d99ae9ffae12585b6ffed7aef0ddf Imported release 0.9.9.9
     \endverbatim
     </li>
     <li> Applying the patches from the Fedora repository to
     those files from the official Mhash sourceforge repository
     yields a working solution:
     \verbatim
# Apply patches and use create new tarball
ExternalSources/sources/Mhash> git clone git://mhash.git.sourceforge.net/gitroot/mhash/mhash mhash-0.9.9.9
ExternalSources/sources/Mhash> git clone git://pkgs.fedoraproject.org/mhash.git mhash-fedora
# Apply latest patches; note that mutils-align.patch doesn't apply to
# mhash-0.9.9.9
ExternalSources/sources/Mhash> cp mhash-fedora/*.patch mhash-0.9.9.9/.
ExternalSources/sources/Mhash> cd mhash-0.9.9.9
ExternalSources/sources/Mhash/mhash-0.9.9.9> for p in mhash-0.9.9.9-*.patch; do patch -p1 < $p; done
ExternalSources/sources/Mhash/mhash-0.9.9.9> cd ../
ExternalSources/sources/Mhash> mv mhash-0.9.9.9.tar.bz2{,-orig}
ExternalSources/sources/Mhash> tar jcvf mhash-0.9.9.9.tar.bz2 mhash-0.9.9.9
ExternalSources/sources/Mhash> cd ../../
ExternalSources/> oklib cleanallmhash mhash
# No error occurs


# Re-enable disabled Mhash-based programs
# (see above for confirmation that these are all disabled apps)
OKlib> mv Structures/Cryptology/HashMD5.cpp{_disabled,}
OKlib> mv Structures/Cryptology/HashMD5lib.cpp{_disabled,}

# Build programs
OKlib> cd Structures/Cryptology
Structures/Cryptology> oklib all
# Builds correctly.

# Test programs
Structures/Cryptology> HashMD5-O3-DNDEBUG < definitions.mak
Hash:2927db35dd111162c18e040673c932af
Structures/Cryptology> md5sum definitions.mak
2927db35dd111162c18e040673c932af  definitions.mak
     \endverbatim
     </li>
     <li> DONE (we don't want to apply them in the build process)
     Now building Mhash, where
     Buildsystem/ExternalSources/SpecialBuilds/mhash.mak has been changed so
     that
     \verbatim
cd $(mhash_build_dir_okl); $(postcondition) \
/configure --prefix=$(mhash_installation_dir_okl) --with-CC=$(gcc412_call_okl); $(postcondition) \
     \endverbatim
     becomes
     \verbatim
cd $(mhash_build_dir_okl); $(postcondition) \
cp $$(dirname $(mhash_source_dir_okl))/*.patch .; $(postcondition) \
for p in *.patch; do patch -p1 < $${p}; done; $(postcondition) \
./configure --prefix=$(mhash_installation_dir_okl) --with-CC=$(gcc412_call_okl); $(postcondition) \
     \endverbatim
     means we no longer get errors during "oklib mhash" in ExternalSources.
     </li>
    </ul>
   </li>
   <li> For now we stop installing and using it. </li>
  </ul>


  \todo Complete MHash documentation
   
*/

