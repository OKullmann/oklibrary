# Oliver Kullmann, 6.3.2002 (Swansea)


#
# ===============================================================================
# Targets
# ===============================================================================
#
# Applications
# ------------
# all                           Compile unoptimised and optimised object files 
#                               and programs.
# unoptimised                   Compile unoptimised object files and programs.
# optimised                     Compile optimised object files and programs.

# COMMENT OK: Here we need to speak about compilation of *test*-programs!

# 
# Test system
# -----------
# check                         Perform unoptimised and optimised tests from old
#                               test system.
# new_check                     Perform unoptimised and optimised tests from new
#                               test system.
# test                          Perform unoptimised tests from old test system.
# testop                        Perform optimised tests from old test system.
# new_test                      Perform unoptimised tests from new test system.
# new_testop                    Perform optimised tests from new test system.
#
# Documentation
# -------------
# html                          Create doxygen documentation (should be used only at module-level).
#
# Cleaning
# --------
# cleanall                      Remove object files, test object files, 
#                               dependency files, test timestamps, applications 
#                               and test programs
# clean                         Remove object files, test object files, 
#                               dependency files and test timestamps.
# cleanprograms                 Remove all applications and test programs.
# cleanalltests                 Remove timestamp files for unoptimsed and 
#                               optimised test program from old test systems.
# new_cleanalltests             Remove timestamp file for unoptimised and 
#                               optimised test program from new test system.
# cleantest                     Remove timestamp file for unoptimised test 
#                               program from old test system.	
# cleantestop                   Remove timestamp file for optimised test program 
#                               from old test system.
# new_cleantest                 Remove timestamp file for unoptimised test 
#				program from new test system.
# new_cleantestop               Remove timestamp file for optimised test program 
#                               from new test system.
# clean_obj                     Remove all non-test object files.
# cleantestobj                  Remove all test object files.
# cleandep                      Remove all dependency files.
# cleanprograms                 Remove all applications and test programs.
#
# ===============================================================================
# General make variable for customisation of compilation
# ===============================================================================
#
# CXX                   Name of the C++ compiler executable.
# default value:        recommended library version of g++
#
# CC                    Name of the C compiler executable.
# default value:        recommended library version of gcc
#
# CXXFLAGS		For specifying options for the C++ compiler.
# CPPFLAGS              For specifying options for the preprocessor. 
# CFLAGS                For specifying options for the C compiler.
# These three variables are not used by the build system, but are free for the user.
#
# ===============================================================================
# Enviroment/Make build system variables
# ===============================================================================
#
# The most important build system parameters come as environment/Make variable
# pairs. 
# 
# (OKPLATFORM,OKplatform)
# (OKSYSTEM,OKsystem)
# (SYSTEMDIRECTORIES,SystemDirectories)
# (BOOST,Boost)
# (EXTERNALSOURCES,ExternalSources)
#
# In each case the default behaviour is for the build system to use the 
# environment variable unless the Make variable is defined, in which case
# the Make variable is used.
#
# OKPLATFORM/OKplatform
#
# This variable pair specifies the top level directory of the OKplatform.
# The subdirectories $(ExternalSources) and $(SystemDirectories) are created by 
# the build system in the directory specified by the value of this variable.

#
# If both the environment variable OKPLATFORM and the Make variable OKplatform 
# are undefined then the build system gives an error message.
#
# OKSYSTEM/OKsystem
#
# This variable pair specifies the top level directory of the OKsystem
# source tree. The build system assumes that the directory specified by the value
# of this variable is the directory containing the subdiretories "Annotations",
# "Transitional" and "OKlibrary".
#
# SYSTEMDIRECTORIES/SystemDirectories
#
# This variable pair specifies the location of the top level directory of 
# the OKlibrary build tree. The build system creates the subdirectories "aux", "bin", 
# "doc" and "lib" in the directory specified by the value of this variable.
#
# BOOST/Boost
#
# This variable pair specifies the location of the installation directory 
# of the Boost distribution to be used by the build system for compiling the 
# library.
#
# EXTERNALSOURCES/ExternalSources
#
# This variable pair specifies the location of the directory used by the 
# build system to build local versions of the main software required for
# the OKlibrary.

# ===============================================================================
# User variables: 
# ===============================================================================
#
# User variables are intended for use by the user, typically to specify options.
#
# system_versions	If defined (with any non-empty value) then the build
#			system will use the system versions of gcc and boost
#			rather than the recommended local versions.

# COMMENT OK: as discussed, we need system versions separately for gcc and boost

# 
# Compile_tool          A command to prefix each call of the compiler. So, for
#                       example to use the time program to time the execution
#                       of the compiler.
# Link_tool             A command to prefix each call of the linker. So, for
#                       example to use the time program to time the execution
#                       of the linker.
# Test_tool             A command to prefix each call of a test program. So, for
#                       example to use the time program to time the execution
#                       of the test program.
#
#
# Doxygen
# ---------------------
# doxygen-parameters    For doxygen parameters.
#
# ===============================================================================
# System variables (can be optionally redefined) : 
# ===============================================================================
# 
# System variable are for internal use of the build system, but can be 
# redefined by the user with suitable precaution. It is expected in most
# cases that there is no need to redefine any of the values of these variables
# and any redefinition needs to be thoroughly thought through.
#
# Compilation
# -----------
#
# gcc_version_number            Can be defined to specify the version number
#                               of a locally installed GCC to be used by the
#                               build system instead of the system compiler. By
#                               default, has the value of the recommended version
#                               of gcc.
# boost_version_number          Can be defined to specify the version number
#                               of a locally installed Boost to be used by the
#                               build system instead of the system Boost. By
#                               default, has the value of the recommended version
#                               of boost.
#
# Standard_options      Language-standard options. Defined in makefile_generic.mak.
#                       Passed to the compiler for every program.
# default value         -ansi -pedantic
#
# Warning_options       Compiler warnings. Defined in makefile_generic.mak. Passed to 
#                       the compiler for every program.
# default value         -Wall
#
# name_addition         A string to be added to the name of each program.
# default value         ???
#
# System directories
# -----------------
#
# bin_dir               Directory for placement of non-test executables
# default value         ???
#
# lib_dir               Directory for placement of object files and link libraries
# default value         ???
#
# aux_dir               Directory for placement of doxygen error messages log. 
#                       Contains subdirectories dependencies, latex and tests.
# default value         ???
#
# latex_dir             Directory for placement of latex aux files.
# default value         
#
# dependencies_dir      Directory for placement of dependency files.
# default value         
#
# doc_dir               Directory containing subdirectories dvi and html.
# default value         
#
# html_dir              Directory for placement of doxygen documentation.
# default value         
#
# test-bin_dir          Directory for placement of all test executables.
# default value         
#
# test-lib_dir          Directory for placement of module test object files.
# default value         
#
# test-aux_dir          Directory for placement of module test Error, Log, 
#                       Message and test timestamp files.
# default value         
#
# Doxygen
# ---------------------
# Doxygen_modifier      Modifier to doxygen command, for example to redirect 
#                       output to a log file.
# doxy_file             Location of Doxygen config file.
#
# Module-dependent variables
# --------------------------
#
# General_options       Defined in makefile_definitions.mak for each module.
#                       Passed to the compiler for each module program.
# Optimisation_options  Defined in makefile_definitions.mak for each module.
#                       Passed to the compiler for each module program. Also
#                       used as the addition to the name of executables.
# source_libraries      Defined in makefile_definitions.mak for each module
#                       and extended by makefile_generic.mak. Passed to the
#                       compiler for each module program. Intended to contain
#                       a list of directories (each preceeded with -I) for the
#                       compiler to search for header files.
# link_libraries        Defined in makefile_definitions.mak for each module.
#                       Passed to the compiler for each module program.
#                       Intended to contain a list, where each element of the
#                       list has the form "-LX -lY", where X is a path to a
#                       directory for the compiler to search for link libraries
#                       and Y is the name of some link library.
#
# #############################################################

# COMMENT OK
# Regarding the placement, perhaps we should have sub-make-files for the different parts
# (compilation, documentation, etc.) ?! Then the special documentation could go to these
# sub-make-files.
# However, if the documentation is spread over several files, then easily it gets unreadable.
# So we have to be careful
#
# Please check, whether it's still true that we can call a generic make-file from other places.

# ===============================================================================
# Internal variables: 
# ===============================================================================
#
# Variables for programs
# ----------------------
# programs                              Defined in makefile_definitions.mak
#                                       for each module. Specifies names of
#                                       applications for current module.
# ------------------------------------------------------------
#
# test_program (deprecated)             The name of the current module test
#                                       program  (old test system).
# new_test_program                      The name of the current module test
#                                       program.
# standard_test_program_object_file     Specifies the location of the test
#                                       program object file.
#
# Variables for the directory structure of system directories.
# ------------------------------------------------------------
#
# directories		All subdirectories of system directories.
#
# Variables for directory structure of srcdir
# -------------------------------------------
# srcdir                The source code directory of the "current" module.
# testobjects-dir       The directory of test object files for the current 
#                       module.
#
# Variables for compiler & compiler options
# -----------------------------------------
#
# All_options           The concatenation of General_options and 
#                       Optimisation_options.
#
# ##################################################################################
#
# Main compilation variables
# --------------------------
#
# compilation_units_cpp         The list of C++ source code files with
#                               extension .cpp in the current module.
# compilations_units_c          The list of C source code files with
#                               extensions .c in the current module.
#
# the above two variables contain just the filenames without any path-information
# so compilation units must be located in srcdir
#
# test_compilation_units        The list of C++ source code files for the
#                               tests in the current module.
# dependency_files_cpp          The list of dependency files for the C++
#                               source code files in the current module.
# dependency_files_c            The list of dependency files for the C
#                               source code files in the current module.
# dependency_files              The concatenation of the lists 
#                               dependency_files_cpp and
#                               dependency_files_c
# test_dependency_files         The dependency files for the test in the
#                               current module.
# object_files_cpp              The list of C++ object files for the 
#                               current module.
# object_files_c                The list of C object files for the current
#                               module.
# object_files                  The concatenation of the two lists
#                               object_files_cpp and object_files_c.
# object_files_cpp_optimised    The list of optimised C++ object files for
#                               the current module.
# object_files_c_optimised      The list of optimised C object files for 
#                               the current module.
# object_files_optimised        The concatenation of the two lists 
#                               object_files_cpp_optimised and
#                               object_files_c_optimised.
# test_object_files             The list of C++ object files for the tests
#                               in the current module.
# test_object_files_optimised   The list of C object files for the tests in
#                               the current module.

# COMMENT OK: the above variables need more precise definition (do they contain the path? the extension?)

# test_file                     Timestamp to mark the creation of the test 
#                               program for the current module.
# testop_file                   Timestamp to mark the creation of the 
#                               optimised test program for the current 
#                               module.
#

# alternative_library_path      If boost_version_number is defined then the value
#                               of this variable specifies the location of the
#                               Boost link libraries directory.
#
# Test system variables
# ---------------------
# test_level                    By default has the value "basic" but can be
#                               redefined to either of the values "full" or
#                               "extensive". Specifies the level of testing
#                               carried out be (new) test system.
# error_file                    Specifies the location of the error log file
#                               for the test program of the current module.
# message_file                  Specifies the location of the messages log file
#                               for the test program of the current module.
# log_file                      Specifies the location of the log file for the
#                               test program of the current module.
# error_stream                  Specifies a stream for the streaming of error
#                               messages.
# message_stream                Specifies a stream for the streaming of non-
#                               error messages.
# log_stream                    Specifies a stream for the streaming of log
#                               messages.
# error_level                   Specifies verbosity level for error messages,
#                               by default 0 (lowest verbosity).
# message_level                 Specifies verbosity level for non-error 
#                               messages, default 0 (lowest verbosity).
# log_level                     Specifies verbosity level for log 
#                               messages, default 0 (lowest verbosity).
# error_lang                    Specifies language for error messages, by
#                               default equal to en_GB.
# message_lang                  Specifies language for non-error messages, by
#                               default equal to en_GB.
# log_lang                      Specifies language for log messages, by
#                               default equal to en_GB.
# test_parameters               Parameters to be passed to the test program for
#                               the current module.
#
# test_timestamp                Timestamp for the creation of test programs with
#                               the new test system.
# testop_timestamp              Timestamp for the creation of optimised test
#                               programs with the new test system.
#
#
# -------------------------------------------------------------------------------
# Make functions
# -------------------------------------------------------------------------------
#
# get-link-libraries      If the current module contains a file 
#                         Module.link_libraries then echo the contents
#                         of this file, otherwise echo the Make variable
#                         $(link_libraries).
# get-link-libraries_optimised
#                         If the current module contains a file 
#                         Module.link_libraries_optimised then echo the 
#                         contents of this file, otherwise echo the Make 
#                         variable $(link_libraries).

####################################################################################

SHELL = /bin/sh
.SUFFIXES :

define last-element
$(word $(words $1),$1)
endef

ifndef srcdir

this-makefile := $(call last-element,$(MAKEFILE_LIST))
srcdir := $(shell cd $(dir $(this-makefile)); pwd)
module-name := $(notdir $(srcdir))

endif

# ##########################################################
# Original definitions of OKplatform and OKBuildsystem, are 
# in Transtional/Buildsystem/generic.mak and cut-and-pasted
# to :
#  Transitional/Buildsystem/ExternalSources.mak
#  Transitional/Buildsystem/makefile
#  Transitional/Buildsystem/OKsystem.mak
#  Transitional/Buildsystem/recursive.mak
#  Transitional/makefile
#  Annotations/makefile
# ##########################################################

ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKBuildsystem
 ifdef OKBUILDSYSTEM
   OKBuildsystem := $(OKBUILDSYSTEM)
 else
   ifdef OKsystem
     OKBuildsystem := $(OKsystem)/Transitional/Buildsystem
   else
     ifdef OKSYSTEM
       OKBuildsystem := $(OKSYSTEM)/Transitional/Buildsystem
     else
       OKBuildsystem := $(OKplatform)/OKsystem/Transitional/Buildsystem
     endif
   endif
 endif
endif

# ##########################################################

include $(OKBuildsystem)/system_definitions.mak

export

include $(OKBuildsystem)/external_sources_versions.mak

export

include $(OKBuildsystem)/standardgoals.mak

export

include $(srcdir)/makefile.definitions.mak

# Definitions required from makefile.definitions:
# General_options
# Optimisation_options
# test_program (for the old test-system)
# programs
# source_libraries
# link_libraries

source_libraries += $(OKsystem_include)

bin_dir := $(system_directories)/bin
lib_dir := $(system_directories)/lib
doc_dir := $(system_directories)/doc
aux_dir := $(system_directories)/aux

latex_dir := $(aux_dir)/latex
dependencies_dir := $(aux_dir)/dependencies

html_dir := $(doc_dir)/html

test-bin_dir := $(bin_dir)/tests
test-lib_dir := $(lib_dir)/tests/$(module-name)
test-aux_dir := $(aux_dir)/tests/$(module-name)

directories := $(bin_dir) $(lib_dir) $(aux_dir) $(latex_dir) $(dependencies_dir) $(doc_dir) $(html_dir) $(test-bin_dir) $(test-lib_dir) $(test-aux_dir)

# --------------------------------------------------

ifneq ($(programs),)
  programs := $(addprefix $(bin_dir)/, $(programs))
endif

ifneq ($(test_program),) # old test-system needed
  test_program := $(addprefix $(bin_dir)/, $(test_program))
  programs += $(test_program)
endif

new_test_program := $(test-bin_dir)/$(module-name)
standard_test_program_object_file := $(lib_dir)/TestProgram_DesignStudy.o

testobjects-dir := $(srcdir)/testobjects

# ---------------------------------------------------

name_addition := $(shell echo $(Optimisation_options) | tr --delete " ")
All_options := $(General_options) $(Optimisation_options)

programs_optimised := $(programs:=$(name_addition))

new_test_program_optimised := $(new_test_program)$(name_addition)
standard_test_program_object_file_optimised := $(lib_dir)/TestProgram_DesignStudy$(name_addition).o

compilation_units_cpp := $(wildcard $(srcdir)/*.cpp)
compilation_units_cpp := $(notdir $(compilation_units_cpp))
compilation_units_c := $(wildcard $(srcdir)/*.c)
compilation_units_c := $(notdir $(compilation_units_c))

test_compilation_units := $(wildcard $(testobjects-dir)/*.cpp)

dependency_files_cpp := $(compilation_units_cpp:.cpp=.d)
dependency_files_cpp := $(addprefix $(dependencies_dir)/, $(dependency_files_cpp))
dependency_files_c := $(compilation_units_c:.c=.d) 
dependency_files_c := $(addprefix $(dependencies_dir)/, $(dependency_files_c))
dependency_files := $(dependency_files_cpp) $(dependency_files_c)

test_dependency_files := $(notdir $(test_compilation_units))
test_dependency_files := $(test_dependency_files:.cpp=.d)
test_dependency_files := $(addprefix $(test-aux_dir)/, $(test_dependency_files))
dependency_files := $(dependency_files) $(test_dependency_files)

object_files_cpp := $(compilation_units_cpp:.cpp=.o) 
object_files_cpp := $(addprefix $(lib_dir)/, $(object_files_cpp))
object_files_c := $(compilation_units_c:.c=.o)
object_files_c := $(addprefix $(lib_dir)/, $(object_files_c))
object_files := $(object_files_cpp) $(object_files_c)
object_files_cpp_optimised := $(compilation_units_cpp:.cpp=$(name_addition).o)
object_files_cpp_optimised := $(addprefix $(lib_dir)/, $(object_files_cpp_optimised))
object_files_c_optimised := $(compilation_units_c:.c=$(name_addition).o)
object_files_c_optimised := $(addprefix $(lib_dir)/, $(object_files_c_optimised))
object_files_optimised := $(object_files_cpp_optimised) $(object_files_c_optimised)

test_object_files := $(notdir $(test_compilation_units))
test_object_files := $(test_object_files:.cpp=.o)
test_object_files := $(addprefix $(test-lib_dir)/, $(test_object_files))

test_object_files_optimised := $(notdir $(test_compilation_units))
test_object_files_optimised := $(test_object_files_optimised:.cpp=$(name_addition).o)
test_object_files_optimised := $(addprefix $(test-lib_dir)/, $(test_object_files_optimised))


test_file := $(srcdir)/test
testop_file := $(srcdir)/testop

# -----------------------------------------------------------------------------------



# -----------------------------------------------------------------------------------

CXX := g++
CC := gcc
Standard_options := -ansi -pedantic
Warning_options := -Wall
Compile_tool := time -p
Link_tool := time -p
Test_tool := time -p

# -----------------------------------------------------------------------------------

test_level := basic

error_file := $(test-aux_dir)/Error
message_file := $(test-aux_dir)/Message
log_file := $(test-aux_dir)/Log

error_stream := "cerr|ofstream-w=$(error_file)|ofstream-w=$(log_file)"
message_stream := "cout|ofstream-w=$(message_file)|ofstream-w=$(log_file)"
log_stream := "ofstream-w=$(log_file)"

error_level := 0
message_level := 0
log_level := 0

error_lang := en_GB
message_lang := en_GB
log_lang := en_GB

test_parameters := $(test_level) $(error_stream) $(message_stream) $(log_stream) $(error_level) $(message_level) $(log_level) $(error_lang) $(message_lang) $(log_lang)

# -----------------------------------------------------------------------------------

test_timestamp := $(test-aux_dir)/new_test_$(test_level)
testop_timestamp := $(test-aux_dir)/new_testop_$(test_level)

# -----------------------------------------------------------------------------------

system_versions :=

ifeq ($(system_versions),)
    gcc_version_number:= $(gcc_recommended_version_number)
    boost_version_number:= $(boost_recommended_version_number)
endif

alternative_library_path:=
# COMMENT OK: this needs revision (for example "alternative" isn't right anymore)
ifneq ($(gcc_version_number),) # alternative gcc specified
  CXX := $(ExternalSources)/Gcc/$(gcc_version_number)/bin/g++
  CC := $(ExternalSources)/Gcc/$(gcc_version_number)/bin/gcc
  alternative_library_path:= -L$(ExternalSources)/Gcc/$(gcc_version_number)/lib64 -Wl,-rpath,$(ExternalSources)/Gcc/$(gcc_version_number)/lib64 -L$(ExternalSources)/Gcc/$(gcc_version_number)/lib -Wl,-rpath,$(ExternalSources)/Gcc/$(gcc_version_number)/lib
  ifeq ($(boost_version_number),)
    ifndef Boost
      $(error Variable Boost must be defined!)
    endif
    boost_version_number := $(patsubst -I%,%,$(Boost))
    boost_version_number := $(patsubst %/,%,$(boost_version_number))
    boost_version_number := $(notdir $(boost_version_number))
    boost_version_number := $(patsubst boost-%,%,$(boost_version_number))
  endif
  alternative_library_path := $(alternative_library_path) -L$(ExternalSources)/Boost/$(boost_version_number)+$(gcc_version_number)/lib -Wl,-rpath,$(ExternalSources)/Boost/$(boost_version_number)+$(gcc_version_number)/lib
  Boost = -I$(ExternalSources)/Boost/$(boost_version_number)+$(gcc_version_number)/include/boost-$(boost_version_number)
else # no alternative gcc specified
  ifneq ($(boost_version_number),) # alternative boost specified
  alternative_library_path := $(alternative_library_path) -L$(ExternalSources)/Boost/$(boost_version_number)/lib -Wl,-rpath,$(ExternalSources)/Boost/$(boost_version_number)/lib
  Boost = -I$(ExternalSources)/Boost/$(boost_version_number)/include/boost-$(boost_version_number)
  endif
endif

# Explanation: With "-Wl,-rpath,Path" we tell the compiler to pass the option "-rpath Path" to the linker, which will add Path to the places searched for this program for run-time-libraries.

# -----------------------------------------------------------------------------------

# #######################################################
# Documentation includes
# #######################################################

include $(OKBuildsystem)/makefile_generic_include/documentation/documentation.mak

include $(OKBuildsystem)/makefile_generic_include/documentation/documentation_index.mak

# -----------------------------------------------------------------------------------

all : unoptimised optimised

$(directories) :
	@mkdir -p $@

unoptimised : $(object_files) $(programs)

optimised : $(object_files_optimised) $(programs_optimised)

check : test testop

new_check : new_test new_testop

ifneq ($(test_program),) # old test-system needed
test : $(test_file)
testop : $(testop_file)
else
test :
testop :
endif

new_test : $(test_timestamp)
new_testop : $(testop_timestamp)

$(test_file) : $(test_program)
	$(Test_tool) $(test_program)
	touch $(test_file)
$(testop_file) : $(test_program)$(name_addition)
	$(Test_tool) $(test_program)$(name_addition)
	touch $(testop_file)

$(test_timestamp) : $(new_test_program) | $(test-aux_dir)
	$(Test_tool) $(new_test_program) $(test_parameters)
	touch $(test_timestamp)
$(testop_timestamp) : $(new_test_program_optimised)
	$(Test_tool) $(new_test_program_optimised) $(test_parameters)
	touch $(testop_timestamp)

force :

# ---------------------------------------------------------------

ifeq ($(firstword $(filter $(special_goals) $(cleaning_goals) $(forced_goals), $(MAKECMDGOALS))),)
include $(dependency_files)
endif

$(dependency_files_cpp) : $(dependencies_dir)/%.d : $(srcdir)/%.cpp | $(dependencies_dir)
	$(CXX) -MM -MF $@ -MT $(lib_dir)/$*.o -MT $(lib_dir)/$*$(name_addition).o -MT $@ $(source_libraries) $<

$(dependency_files_c) : $(dependencies_dir)/%.d : $(srcdir)/%.c | $(dependencies_dir)
	$(CC) -MM -MF $@ -MT $(lib_dir)/$*.o -MT $(lib_dir)/$*$(name_addition).o -MT $@ $(source_libraries) $<

$(test_dependency_files) : $(test-aux_dir)/%.d : $(testobjects-dir)/%.cpp | $(test-aux_dir)
	$(CXX) -MM -MF $@ -MT $(test-lib_dir)/$*.o -MT $(test-lib_dir)/$*$(name_addition).o -MT $@ $(source_libraries) $<

$(object_files_cpp_optimised) : $(lib_dir)/%$(name_addition).o : $(srcdir)/%.cpp | $(lib_dir)
	$(Compile_tool) $(CXX) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CXXFLAGS) $(Optimisation_options) $(source_libraries) $<

$(object_files_cpp) : $(lib_dir)/%.o : $(srcdir)/%.cpp | $(lib_dir)
	$(Compile_tool) $(CXX) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CXXFLAGS) $(General_options) $(source_libraries) $<

$(object_files_c_optimised) : $(lib_dir)/%$(name_addition).o : $(srcdir)/%.c | $(lib_dir)
	$(Compile_tool) $(CC) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CFLAGS) $(Optimisation_options) $(source_libraries) $<

$(object_files_c) : $(lib_dir)/%.o : $(srcdir)/%.c | $(lib_dir)
	$(Compile_tool) $(CC) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CFLAGS) $(General_options) $(source_libraries) $<

$(test_object_files) : $(test-lib_dir)/%.o : $(testobjects-dir)/%.cpp | $(test-lib_dir)
	$(Compile_tool) $(CXX) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CXXFLAGS) $(General_options) $(source_libraries) $<

$(test_object_files_optimised) : $(test-lib_dir)/%$(name_addition).o : $(testobjects-dir)/%.cpp | $(test-lib_dir)
	$(Compile_tool) $(CXX) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CXXFLAGS) $(Optimisation_options) $(source_libraries) $<


define get-link_libraries
$$(if [[ -e $(srcdir)/$*.link_libraries ]]; then $(srcdir)/$*.link_libraries; else echo $(link_libraries); fi)
endef
define get-link_libraries_optimised
$$(if [[ -e $(srcdir)/$*.link_libraries_optimised ]]; then $(srcdir)/$*.link_libraries_optimised; else if [[ -e $(srcdir)/$*.link_libraries ]]; then $(srcdir)/$*.link_libraries; else echo $(link_libraries); fi; fi)
endef

export

$(programs) : $(bin_dir)/% : $(lib_dir)/%.o | $(bin_dir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(General_options) $< $(alternative_library_path) $(get-link_libraries)

$(programs_optimised) : $(bin_dir)/%$(name_addition) : $(lib_dir)/%$(name_addition).o | $(bin_dir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(Optimisation_options) $< $(alternative_library_path) $(get-link_libraries_optimised)

$(new_test_program) : $(test-bin_dir)/% : $(test_object_files) $(standard_test_program_object_file) | $(test-bin_dir) $(test-aux_dir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(General_options) $^ $(alternative_library_path) $(get-link_libraries)

$(new_test_program_optimised) : $(test-bin_dir)/%$(name_addition) : $(test_object_files_optimised) $(standard_test_program_object_file_optimised) | $(test-bin_dir) $(test-aux_dir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(Optimisation_options) $^ $(alternative_library_path) $(get-link_libraries_optimised)

# --------------------------------

cleantest :
	- rm $(test_file)

cleantestop :
	- rm $(testop_file)

cleanalltest : cleantest cleantestop

new_cleantest :
	- rm $(test_timestamp)

new_cleantestop :
	- rm $(testop_timestamp)

new_cleanalltests : new_cleantest new_cleantestop

cleanobj :
	- rm $(object_files) $(object_files_optimised)

cleantestobj :
	- rm $(test_object_files) $(test_object_files_optimised)

cleandep :
	- rm $(dependency_files)

clean : cleanobj cleantestobj cleandep
	- rm $(test_file) $(testop_file) $(test_timestamp) $(testop_timestamp) 

cleanprograms :
	- rm $(programs) $(programs_optimised) $(new_test_program) $(new_test_program_optimised) 

cleanall : clean cleanprograms
	- rm $(error_file) $(message_file) $(log_file)

