// Oliver Kullmann, 21.4.2002 (Swansea)

#ifndef EXPERIMENTUEBERTRAGUNGWAECHTER

#define EXPERIMENTUEBERTRAGUNGWAECHTER

#include <string>

#include <xercesc/util/XMLException.hpp>

#include "Tools.hpp"
#include "DatabaseHandler01.hpp"


namespace ExperimentUebertragung {

  using std::string;

  // Error handling ----------------------------------------

  namespace Error {

    class General {
    public :
      General(const string& s) : w(s) {}
      virtual string what() const throw() { return w; }
    private :
      string w;
    };

    struct not_a_number : General {
      not_a_number(const string& s) : General(s) {}
    };
    struct not_an_integer : General {
      not_an_integer(const string& s) : General(s) {}
    };

    struct missing_attribute : General {
      missing_attribute(const string& s) : General(s) {}
    };
    struct wrong_attribute_value : General {
      wrong_attribute_value(const string& s) : General(s) {}
    };

    struct no_experiment_id : General {
      no_experiment_id() : General("") {}
    };

    struct not_implemented : General {
      not_implemented(const string& s) : General(s) {}
    };

    struct non_positive_n : General {
      non_positive_n(const string& s) : General(s) {}
    };
    struct non_positive_l : General {
      non_positive_l(const string& s) : General(s) {}
    };
    struct non_positive_c : General {
      non_positive_c(const string& s) : General(s) {}
    };

    struct Internal_error : General {
      Internal_error(const string& s) : General(s) {}
    };

  }

  void transfer(const string& dirname);

}

#endif

#define EXPERIMENTUEBERTRAGUNGWAECHTER

