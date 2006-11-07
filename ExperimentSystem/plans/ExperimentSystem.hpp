// Oliver Kullmann, 2.11.2005 (Swansea)

/*!
  \file ExperimentSystem/plans/ExperimentSystem.hpp
  \brief Launching and monitoring experiments on remote machines.

  \todo First step is to transfer the experiment-system from OKgenerator
  (see directory OKsolver/Generator) to the (transitional) library.
  Necessary extensions (improvements):
   - Descriptors:
     + It must be possible to process "ad-hoc experiments"; the simplest
     way to do so is to allow as input a directory-name, and then all
     files in there will be processed.
     + Additional parameters specifying the experiment like the abortion-time.
   - Database (see module OKDatabase) must be extended to administer also
     ad-hoc experiments.

  \todo The launch-and-monitor system follows the old 3 steps (preparation,
  processing, transfer), but now all these steps can be launched automatically.
  So a database of available machines is needed (access via ssh). When starting
  processing an experiment, the pid is extracted, so that then the output of the
  ps-command for this process can be shown; also "ls -l" for the experiment-directory
  and "less" for the files in them must be supported. For launching a new experiment
  (always on a fixed machine; we simply do not support transferring experiments
  to different machines, but it must be easy to abort an experiment and transfer
  the data obtained so far) one can ask for the available machines (showing a
  status report about availability, how many experiments are currently running on
  it, how many processors the machine has, bogomips etc.) and then, via some
  command perhaps using a syntax like chmod for "prepare yes/no, process yes/no,
  transfer yes/no", say, +-+, an experiment can be started. Perhaps we use simple
  single tools for the different actions (querying the status, launching an experiment),
  and use simple copy-and-paste to transfer data from one step to another.

  \todo The tests must be written in such a fashion, that they are largely testable without
  accessing ssh; for testing the parts essentially using ssh one can then simply use
  the host machine of the testing process.
*/

/*!
  \namespace OKlib::ExperimentSystem
  \brief Components for performing experiments
*/

namespace OKlib {
  namespace ExperimentSystem {
  }
}

#endif
