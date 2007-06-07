// Oliver Kullmann, 7.6.2007 (Swansea)

/*!
  \file Buildsystem/plans/Release.hpp
  \brief Plans regarding package building etc.

  \todo Distributing the library
  <ul>
   <li> A major problem is how to filter out the "secret" parts.
    <ol>
     <li> Hopefully with "git-submodule" we can create sub-repositories
     (see Buildsystem/plans/VersionControl.hpp). </li>
     <li> Likely critical are only the plans-directories. The simplest measure is to
     create a sub-module without the plans-directories. And for special users we add then
     appropriate plans-directories. </li>
    </ol>
   </li>
   <li> Two download possibilities: Either the whole library (with all included libraries like
     Boost, doxygen, PostgreSQL etc.), or only the minimum.
     This gives two user types: "full user" and "minimal user".
     We must make sure, that also the minimal user can use the test and the complexity system
     (compiling it himself, or using the build system). And we must study, how a full user can keep his
     file structure separate from the library by using links.
   </li>
  </ul>

  \todo Competition extraction tools
   - these tools, for a given program
     build a package, which contains all necessary source code and can be build
     using make (producing then the executable). A problem here are the link-libraries.
     Without them, one could use the ability of g++ to produce compilation units (using
     the option "-E" like "g++ -E Program.cpp -o Program.ii"), putting everything into one big file.
     Knowing the link-libraries, just compiling Program.ii with the right .o-files yields the program.
     So when we know all compilation units, we can preprocess them all and putting them
     into the package; the package just compiles the units separately, and then links them together.

  \todo Package construction script
   - Writing the package-construction script. (Internal versions should
     have date and time in the name.)

*/

