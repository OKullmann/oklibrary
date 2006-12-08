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
# CXXFLAGS		For specifying options for the C++ compiler.
# CPPFLAGS              For specifying options for the preprocessor. 
# CFLAGS                For specifying options for the C compiler.
#
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
#
# ===============================================================================
# (SV) System variables (can be optionally redefined) : 
# ===============================================================================
# 
# System variable are for internal use of the build system, but can be 
# redefined by the user with suitable precaution. It is expected in most
# cases that there is no need to redefine any of the values of these variables
# and any redefinition needs to be thoroughly thought through.
#
# ##################################################################################

SHELL = /bin/sh
.SUFFIXES :

# ################################################################
# FUNCTIONS (F)
# ################################################################

# ----------------------------------------------------------
# last-element (F)
#
# Returns last element of its argument.
# ----------------------------------------------------------
define last-element
$(word $(words $1),$1)
endef
# ----------------------------------------------------------

# ######################################################################

# ################################################################
# LOCAL MODULE DEFINITIONS
# ################################################################

# ----------------------------------------------------------
# srcdir (LV)
#
# The full path of the current directory of source code to 
# be compiled.
# ----------------------------------------------------------
ifndef srcdir
this-makefile := $(call last-element,$(MAKEFILE_LIST))
srcdir := $(shell cd $(dir $(this-makefile)); pwd)
endif
# ----------------------------------------------------------

# ----------------------------------------------------------
# module-name (LV)
# The name of the current module.
# ----------------------------------------------------------
ifdef srcdir
module-name := $(notdir $(srcdir))
endif
# ----------------------------------------------------------

# ################################################################
# Original definitions of OKplatform and OKBuildsystem, are 
# in Transtional/Buildsystem/generic.mak and cut-and-pasted
# to :
#  Transitional/Buildsystem/ExternalSources.mak
#  Transitional/Buildsystem/makefile
#  Transitional/Buildsystem/OKsystem.mak
#  Transitional/Buildsystem/recursive.mak
#  Transitional/makefile
#  Annotations/makefile
# ################################################################

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

# ######################################################################

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

# ----------------------------------------------------------
# source_libraries (LV)
#
# This variable is defined in the local 
# makefile.definitions.mak file. Here we are just appending
# the OKsystem include option -I$(OKsystem)
# ----------------------------------------------------------
source_libraries += $(OKsystem_include)
# ----------------------------------------------------------

# ################################################################
# BUILD DIRECTORY STRUCTURE
# ################################################################

# ----------------------------------------------------------
# bin_dir (SV)
#
# Top-level directory for all executables.
# ----------------------------------------------------------
bin_dir := $(system_directories)/bin
# ----------------------------------------------------------

# ----------------------------------------------------------
# lib_dir (SV)
#
# This is the top-level directory for all object-files.
# ----------------------------------------------------------
lib_dir := $(system_directories)/lib
# ----------------------------------------------------------

# ----------------------------------------------------------
# doc_dir (SV)
#
# This is the top-level directory for all documentation. 
# ----------------------------------------------------------
doc_dir := $(system_directories)/doc
# ----------------------------------------------------------

# ----------------------------------------------------------
# aux_dir (SV)
# 
# Top-level directory for all dependency files,
# error, message and log files, and latex auxilliary files.
# ----------------------------------------------------------
aux_dir := $(system_directories)/aux
# ----------------------------------------------------------

# ----------------------------------------------------------
# latex_dir (SV)
#
# Directory for all latex auxilliary files.
# ----------------------------------------------------------
latex_dir := $(aux_dir)/latex
# ----------------------------------------------------------

# ----------------------------------------------------------
# dependencies_dir (SV)
#
# Directory for all dependency files.
# ----------------------------------------------------------
dependencies_dir := $(aux_dir)/dependencies
# ----------------------------------------------------------

# ----------------------------------------------------------
# html_dir (SV)
#
# Top-level directory for doxygen documentation.
# ----------------------------------------------------------
html_dir := $(doc_dir)/html
# ----------------------------------------------------------

# ----------------------------------------------------------
# test-bin_dir (SV)
#
# Directory for all test executables.
# ----------------------------------------------------------
test-bin_dir := $(bin_dir)/tests
# ----------------------------------------------------------

# ----------------------------------------------------------
# test-lib_dir (SV)
#
# Directory for current module test object-files. 
# ----------------------------------------------------------
test-lib_dir := $(lib_dir)/tests/$(module-name)
# ----------------------------------------------------------

# ----------------------------------------------------------
# test-aux_dir (SV)
#
# Directory for current module test dependency files.
# ----------------------------------------------------------
test-aux_dir := $(aux_dir)/tests/$(module-name)
# ----------------------------------------------------------

# ----------------------------------------------------------
# directories (SV)
#
# All the build directories.
# ----------------------------------------------------------
directories := $(bin_dir) \
               $(lib_dir) \
               $(aux_dir) \
               $(latex_dir) \
               $(dependencies_dir) \
               $(doc_dir) \
               $(html_dir) \
               $(test-bin_dir) \
               $(test-lib_dir) \
               $(test-aux_dir)
# ----------------------------------------------------------

# ######################################################################

# ################################################################
# COMPILATION
# ################################################################

# ----------------------------------------------------------
# programs (LV)
#
# This variable is not defined here, but rather it's
# definition is in the local makefile.definitions.mak.
#
# If there are program names specified in the local
# makefile.definitions.mak then we prefix those program
# names with the full path to the executable directory, so
# that Make knows where to build those executables.
# ----------------------------------------------------------
ifneq ($(programs),)
  programs := $(addprefix $(bin_dir)/, $(programs))
endif
# ----------------------------------------------------------

# ----------------------------------------------------------
# test_program (LV)
#
# This variable is not defined here, but rather it's
# definition is in the local makefile.definitions.mak.
#
# If test programs (from the old test system) are specified
# in the local makefile.definitions.mak then we prefix
# those program names are prefixed here by the full path
# to the directory where test executables are built, so that
# Make knows where to build those executables. Also, those
# full program path names are appended to the variable
# "programs".
#
# ----------------------------------------------------------
ifneq ($(test_program),) # old test-system needed
  test_program := $(addprefix $(bin_dir)/, $(test_program))
  programs += $(test_program)
endif
# ----------------------------------------------------------


# ----------------------------------------------------------
# new_test_program (LV)
#
# The full path name for the test executable for the
# current module.
# ----------------------------------------------------------
new_test_program := $(test-bin_dir)/$(module-name)
# ----------------------------------------------------------

# ----------------------------------------------------------
# standard_test_program_object_file (SV)
#
# The full path name for the object file which is
# linked to by every test program.
# ----------------------------------------------------------
standard_test_program_object_file := $(lib_dir)/TestProgram_DesignStudy.o
# ----------------------------------------------------------

# ----------------------------------------------------------
# testobjects-dir (LV)
# 
# The subdirectory of the module source code
# directory which contains the test object-files.
# ----------------------------------------------------------
testobjects-dir := $(srcdir)/testobjects
# ----------------------------------------------------------

# ----------------------------------------------------------
# name_addition (LV)
#
# A string which is appended to the name of every executable.
# By default the value is just the optimisation options.
# ----------------------------------------------------------
name_addition := $(shell echo $(Optimisation_options) | tr --delete " ")
# ----------------------------------------------------------

# ----------------------------------------------------------
# programs_optimised (LV)
#
# The names of the optimised executables.
# ----------------------------------------------------------
programs_optimised := $(programs:=$(name_addition))
# ----------------------------------------------------------

# ----------------------------------------------------------
# new_test_program_optimised (LV)
#
# The names of the optimised test programs. (New test
# system).
# ----------------------------------------------------------
new_test_program_optimised := $(new_test_program)$(name_addition)
# ----------------------------------------------------------

# ----------------------------------------------------------
# standard_test_program_object_file_optimised (LV)
#
# The full path name of the optimised object-file to which
# every test program is linked.
# ----------------------------------------------------------
standard_test_program_object_file_optimised := $(lib_dir)/TestProgram_DesignStudy$(name_addition).o
# ----------------------------------------------------------

# ######################################################################

# ################################################################
# COMPILATION UNITS
# ################################################################

# ----------------------------------------------------------
# compilation_units_cpp (LV)
#
# The names of every C++ source code file in the current
# module source code directory.
# ----------------------------------------------------------
compilation_units_cpp := $(wildcard $(srcdir)/*.cpp)
compilation_units_cpp := $(notdir $(compilation_units_cpp))
# ----------------------------------------------------------

# ----------------------------------------------------------
# compilation_units_c (LV)
#
# The names of every C source code file in the current
# module source code directory.
# ----------------------------------------------------------
compilation_units_c := $(wildcard $(srcdir)/*.c)
compilation_units_c := $(notdir $(compilation_units_c))
# ----------------------------------------------------------

# ----------------------------------------------------------
# test_compilation_units (LV)
#
# The full path names of every C++ source code file in
# the testobjects subdirectory of the current module
# ----------------------------------------------------------
test_compilation_units := $(wildcard $(testobjects-dir)/*.cpp)
# ----------------------------------------------------------

# ----------------------------------------------------------
# dependency_files_cpp (LV)
#
# The full path name of the dependency files for every C++
# source code file in the current module.
# ----------------------------------------------------------
dependency_files_cpp := $(compilation_units_cpp:.cpp=.d)
dependency_files_cpp := $(addprefix $(dependencies_dir)/, $(dependency_files_cpp))
# ----------------------------------------------------------

# ----------------------------------------------------------
# dependency_files_c (LV)
#
# The full path name of the dependency files for every C
# source code file in the current module.
# ----------------------------------------------------------
dependency_files_c := $(compilation_units_c:.c=.d) 
dependency_files_c := $(addprefix $(dependencies_dir)/, $(dependency_files_c))
# ----------------------------------------------------------

# ----------------------------------------------------------
# test_dependency_files (LV)
#
# The full path name of the dependency files for every 
# test source code file in the current module.
# ----------------------------------------------------------
test_dependency_files := $(notdir $(test_compilation_units))
test_dependency_files := $(test_dependency_files:.cpp=.d)
test_dependency_files := $(addprefix $(test-aux_dir)/, $(test_dependency_files))
# ----------------------------------------------------------

# ----------------------------------------------------------
# dependency_files (LV)
#
# The full path name of the dependency files for every C
# and C++ source code file in the current module.
# ----------------------------------------------------------
dependency_files := $(dependency_files_cpp) \
                    $(dependency_files_c) \
                    $(dependency_files) \
                    $(test_dependency_files)
# ----------------------------------------------------------

# ----------------------------------------------------------
# object_files_cpp (LV)
#
# The full path name of the object files for every C++
# source code file in the current directory.
# ----------------------------------------------------------
object_files_cpp := $(compilation_units_cpp:.cpp=.o) 
object_files_cpp := $(addprefix $(lib_dir)/, $(object_files_cpp))
# ----------------------------------------------------------

# ----------------------------------------------------------
# object_files_c (LV)
# 
# The full path name of the object files for every C
# source code file in the current directory.
# ----------------------------------------------------------
object_files_c := $(compilation_units_c:.c=.o)
object_files_c := $(addprefix $(lib_dir)/, $(object_files_c))
# ----------------------------------------------------------

# ----------------------------------------------------------
# object_files (LV)
# 
# The full path name of the object files for every C and C++
# source code file in the current directory.
# ----------------------------------------------------------
object_files := $(object_files_cpp) $(object_files_c)
# ----------------------------------------------------------

# ----------------------------------------------------------
# object_files_cpp_optimised (LV)
# 
# The full path name of the optimised object files for every
# C++ source code file in the current directory.
# ----------------------------------------------------------
object_files_cpp_optimised := $(compilation_units_cpp:.cpp=$(name_addition).o)
object_files_cpp_optimised := $(addprefix $(lib_dir)/, $(object_files_cpp_optimised))
# ----------------------------------------------------------

# ----------------------------------------------------------
# object_files_c_optimised (LV)
# 
# The full path name of the optimised object files for every
# C source code file in the current directory.
# ----------------------------------------------------------
object_files_c_optimised := $(compilation_units_c:.c=$(name_addition).o)
object_files_c_optimised := $(addprefix $(lib_dir)/, $(object_files_c_optimised))
# ----------------------------------------------------------

# ----------------------------------------------------------
# object_files_optimised (LV)
# 
# The full path name of the optimised object files for every
# C and C++ source code file in the current directory.
# ----------------------------------------------------------
object_files_optimised := $(object_files_cpp_optimised) $(object_files_c_optimised)
# ----------------------------------------------------------

# ----------------------------------------------------------
# test_object_files (LV)
#
# The full path names of the object files for the tests in the
# current module.
# ----------------------------------------------------------
test_object_files := $(notdir $(test_compilation_units))
test_object_files := $(test_object_files:.cpp=.o)
test_object_files := $(addprefix $(test-lib_dir)/, $(test_object_files))
# ----------------------------------------------------------

# ----------------------------------------------------------
# test_object_files (LV)
#
# The full path names of the object files for the tests in the
# current module.
# ----------------------------------------------------------
test_object_files_optimised := $(notdir $(test_compilation_units))
test_object_files_optimised := $(test_object_files_optimised:.cpp=$(name_addition).o)
test_object_files_optimised := $(addprefix $(test-lib_dir)/, $(test_object_files_optimised))
# ----------------------------------------------------------

# ----------------------------------------------------------
# test_file (SV)
#
# The old test system timestamp file for the current module
# test program. 
# ----------------------------------------------------------
test_file := $(srcdir)/test
# ----------------------------------------------------------

# ----------------------------------------------------------
# testop_file (SV)
#
# The old test system timestamp file for the current module
# optimised test program. .
# ----------------------------------------------------------
testop_file := $(srcdir)/testop
# ----------------------------------------------------------

# ################################################################
# GENERAL MAKE VARIABLES FOR CUSTOMISATION OF COMPILATION
# ################################################################

# ----------------------------------------------------------
# CXX (SV)
#
# Name of the C++ compiler executable. 
# default value: recommended library version of g++
# ----------------------------------------------------------
CXX := g++
# ----------------------------------------------------------

# ----------------------------------------------------------
# CC (SV)
#
# Name of the C compiler executable.
# default value: recommended library version of gcc
# ----------------------------------------------------------
CC := gcc
# ----------------------------------------------------------

# ----------------------------------------------------------
# Standard_options (SV)
#
# Language-standard options. Defined in makefile_generic.mak.
# Passed to the compiler for every program.
# Default value : -ansi -pedantic
# ----------------------------------------------------------
Standard_options := -ansi -pedantic
# ----------------------------------------------------------

# ----------------------------------------------------------
# Warning_options (SV)
#
# Compiler warnings. Defined in makefile_generic.mak. Passed to 
# the compiler for every program.
# Default value : -Wall
# ----------------------------------------------------------
Warning_options := -Wall
# ----------------------------------------------------------

# ----------------------------------------------------------
# Compile_tool (SV)     
#
# A command to prefix each call of the compiler. So, for
# example to use the time program to time the execution
# of the compiler.
# Default value : time -p (measuring the wall clock time, 
# the process time and the system time used in total for 
# the execution)
# ----------------------------------------------------------
Compile_tool := time -p
# ----------------------------------------------------------

# ----------------------------------------------------------
# Link_tool (SV)
#             
# A command to prefix each call of the linker. So, for
# example to use the time program to time the execution
# of the linker.
# Default value : time -p (measuring the wall clock time, 
# the process time and the system time used in total for 
# the execution)
# ----------------------------------------------------------
Link_tool := time -p
# ----------------------------------------------------------

# ----------------------------------------------------------
# Test_tool (SV)
#
# A command to prefix each call of a test program. So, for
# example to use the time program to time the execution
# of the test program. 
# Default value : time -p (measuring the wall clock time, 
# the process time and the system time used in total for 
# the execution)
# Setting Test_tool="valgrind --quiet" runs all tests with 
# memory check.
# ----------------------------------------------------------
Test_tool := time -p
# ----------------------------------------------------------

# ######################################################################

# ################################################################
# VARIABLES FOR CUSTOMISATION OF NEW TEST SYSTEM
# ################################################################

# ----------------------------------------------------------
# test_level (SV)
#
# By default has the value "basic" but can be
# redefined to either of the values "full" or
# "extensive". Specifies the level of testing
# carried out be (new) test system.
# ----------------------------------------------------------
test_level := basic
# ----------------------------------------------------------

# ----------------------------------------------------------
# error_file (SV)
#
# Specifies the location of the error log file
# for the test program of the current module.
# ----------------------------------------------------------
error_file := $(test-aux_dir)/Error
# ----------------------------------------------------------

# ----------------------------------------------------------
# message_file (SV)
# 
# Specifies the location of the messages log file
# for the test program of the current module.
# ----------------------------------------------------------
message_file := $(test-aux_dir)/Message
# ----------------------------------------------------------

# ----------------------------------------------------------
# log_file (SV)
#
# Specifies the location of the log file for the
# test program of the current module.
# ----------------------------------------------------------
log_file := $(test-aux_dir)/Log
# ----------------------------------------------------------

# ----------------------------------------------------------
# error_stream (SV)   
#
# Specifies a stream for the streaming of error
# messages.
# ----------------------------------------------------------
error_stream := "cerr|ofstream-w=$(error_file)|ofstream-w=$(log_file)"
# ----------------------------------------------------------

# ----------------------------------------------------------
# message_stream (SV)
#
# Specifies a stream for the streaming of non-
# error messages.
# ----------------------------------------------------------
message_stream := "cout|ofstream-w=$(message_file)|ofstream-w=$(log_file)"
# ----------------------------------------------------------

# ----------------------------------------------------------
# log_stream (SV)
#
# Specifies a stream for the streaming of log
# messages.
# ----------------------------------------------------------
log_stream := "ofstream-w=$(log_file)"
# ----------------------------------------------------------

# ----------------------------------------------------------
# error_level (SV)                 
#
# Specifies verbosity level for error messages,
# by default 0 (lowest verbosity).
# ----------------------------------------------------------
error_level := 0
# ----------------------------------------------------------

# ----------------------------------------------------------
# message_level (SV)               
#
# Specifies verbosity level for non-error 
# messages, default 0 (lowest verbosity).
# ----------------------------------------------------------
message_level := 0
# ----------------------------------------------------------

# ----------------------------------------------------------
# log_level (SV)                   
#
# Specifies verbosity level for log 
# messages, default 0 (lowest verbosity).
# ----------------------------------------------------------
log_level := 0
# ---------------------------------------------------------- 

# ----------------------------------------------------------
# error_lang (SV)                   
#
# Specifies language for error messages, by
# default equal to en_GB.
# ----------------------------------------------------------
error_lang := en_GB
# ----------------------------------------------------------

# ----------------------------------------------------------
# message_lang (SV)                
#
# Specifies language for non-error messages, by
# default equal to en_GB.
# ----------------------------------------------------------
message_lang := en_GB
# ----------------------------------------------------------

# ----------------------------------------------------------
# log_lang (SV)             
#
# Specifies language for log messages, by
# default equal to en_GB.
# ----------------------------------------------------------
log_lang := en_GB
# ----------------------------------------------------------

# ----------------------------------------------------------
# test_parameters (LV)              
#
# Parameters to be passed to the test program for
# the current module.
# ----------------------------------------------------------
test_parameters := $(test_level) \
                   $(error_stream) \
                   $(message_stream) \
                   $(log_stream) \
                   $(error_level) \
                   $(message_level) \
                   $(log_level) \
                   $(error_lang) \
                   $(message_lang) \
                   $(log_lang)
# ----------------------------------------------------------

# ----------------------------------------------------------
# test_timestamp (LV)
#
# Timestamp for the creation of test programs with
# the new test system.
# ----------------------------------------------------------
test_timestamp := $(test-aux_dir)/new_test_$(test_level)
# ----------------------------------------------------------

# ----------------------------------------------------------
# testop_timestamp (LV)
#
# Timestamp for the creation of optimised test
# programs with the new test system.
# ----------------------------------------------------------
testop_timestamp := $(test-aux_dir)/new_testop_$(test_level)
# ----------------------------------------------------------

# ######################################################################

# ################################################################
# VARIABLES FOR GCC AND BOOST VERSIONS, INCLUDES AND LINKING
# ################################################################

# ----------------------------------------------------------
# system_version (LV)
#
# If defined (with any non-empty value) then the build
# system will use the system versions of gcc and boost
# rather than the recommended local versions.
#
# gcc_version_number (SV)           
#
# Can be defined to specify the version number
# of a locally installed GCC to be used by the
# build system instead of the system compiler. By
# default, has the value of the recommended version
# of gcc.
# 
# boost_version_number (SV)
#
# Can be defined to specify the version number
# of a locally installed Boost to be used by the
# build system instead of the system Boost. By
# default, has the value of the recommended version
# of boost.
#
# COMMENT OK: as discussed, we need system versions 
# separately for gcc and boost
#
# ----------------------------------------------------------
system_versions :=

ifeq ($(system_versions),)
    gcc_version_number:= $(gcc_recommended_version_number)
    boost_version_number:= $(boost_recommended_version_number)
endif
# ----------------------------------------------------------


# ----------------------------------------------------------
#
# alternative_library_path
#
#  Specifies the full options necessary to pass the full path
#  of the Gcc and Boost link-libraries to the compiler and
#  linker. With "-Wl,-rpath,Path" we tell the compiler to 
#  pass the option "-rpath Path" to the linker, which will 
#  add Path to the places searched for this program for 
#  run-time-libraries.
#
# boost_version_number
#
#  The version number of the Boost library.
#
# Boost
#
#  Include option to be passed to the compiler, specifying
#  the location of the Boost include directory. 
#
# COMMENT OK: this needs revision (for example "alternative"
# isn't right anymore)
# ----------------------------------------------------------
alternative_library_path:=

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

# ######################################################################


# ################################################################
# DOCUMENTATION MAKEFILE INCLUDE STATEMENTS
# ################################################################

include $(OKBuildsystem)/makefile_generic_include/documentation/documentation.mak

include $(OKBuildsystem)/makefile_generic_include/documentation/documentation_index.mak

# ######################################################################

# ################################################################
# MAIN TARGETS
# ################################################################

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

# ----------------------------------------------------------
# get-link-libraries      
#
# If the current module contains a file 
# Module.link_libraries then echo the contents
# of this file, otherwise echo the Make variable
# $(link_libraries).
# ----------------------------------------------------------
define get-link_libraries
$$(if [[ -e $(srcdir)/$*.link_libraries ]]; then $(srcdir)/$*.link_libraries; else echo $(link_libraries); fi)
endef
# ----------------------------------------------------------

# ----------------------------------------------------------
# get-link-libraries_optimised
#
# If the current module contains a file 
# Module.link_libraries_optimised then echo the 
# contents of this file, otherwise echo the Make 
# variable $(link_libraries).
# ----------------------------------------------------------
define get-link_libraries_optimised
$$(if [[ -e $(srcdir)/$*.link_libraries_optimised ]]; then $(srcdir)/$*.link_libraries_optimised; else if [[ -e $(srcdir)/$*.link_libraries ]]; then $(srcdir)/$*.link_libraries; else echo $(link_libraries); fi; fi)
endef
# ----------------------------------------------------------

export

$(programs) : $(bin_dir)/% : $(lib_dir)/%.o | $(bin_dir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(General_options) $< $(alternative_library_path) $(get-link_libraries)

$(programs_optimised) : $(bin_dir)/%$(name_addition) : $(lib_dir)/%$(name_addition).o | $(bin_dir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(Optimisation_options) $< $(alternative_library_path) $(get-link_libraries_optimised)

$(new_test_program) : $(test-bin_dir)/% : $(test_object_files) $(standard_test_program_object_file) | $(test-bin_dir) $(test-aux_dir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(General_options) $^ $(alternative_library_path) $(get-link_libraries)

$(new_test_program_optimised) : $(test-bin_dir)/%$(name_addition) : $(test_object_files_optimised) $(standard_test_program_object_file_optimised) | $(test-bin_dir) $(test-aux_dir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(Optimisation_options) $^ $(alternative_library_path) $(get-link_libraries_optimised)

# ######################################################################

# ################################################################
# CLEANING TARGETS
# ################################################################

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

# ######################################################################
