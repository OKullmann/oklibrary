// Oliver Kullmann, 7.3.2003 (Swansea)

#ifndef AUXILIARYWAECHTER

#define AUXILIARYWAECHTER

#include <string>

namespace Auxiliary {

  enum Sat_status {unsatisfiable, satisfiable, unknown};

  enum Evaluation_status {inv_false, inv_true, not_inv};

  // DEPRICATED
  enum Pass_evaluation {satisfied, falsified, undefined};

  const std::string null_variable_tag = "NULL";
}

#endif
