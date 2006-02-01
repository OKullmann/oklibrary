// Matthew Henderson, 25.1.2006 (Swansea)

/*!
  \file InputOutput_Plans.hpp
  \brief 
  \todo Satisfiability of total assignment.

  Syntax: 

      assert( phi(X) )

  Semantics:

      Application of partial assignment phi to cnf clause set X.

  template <class Range>
  tribool all_elements_true( Range range ) {
    typedef range_const_iterator<Range>::type const_iterator;
    const const_iterator& end(boost::end(range));
    tribool result(indeterminate);
    for (const_iterator element(boost::begin(range)); element!=end; ++element) {
      if ( not some_element_true(*element) ) {
        result = false;
        continue;
      }
    } 
    result = true;
    return result;
  }

  template <class Range>
  tribool some_element_true( Range range ) {
    typedef range_const_iterator<Range>::type const_iterator;
    const const_iterator& end(boost::end(range));
    tribool result(indeterminate);
    for (const_iterator element(boost::begin(range)); element!=end; ++element) {
      if ( element_true(*element)  ) {
        result = true;
        continue;
      }
    } 
    result = false;
    return result;
  }

  bool element_true( literal_type literal ) {
     // ??????????????  
  }

  // Operator for application of partial assignment to cnf formula.

  template <class Range>
  tribool operator()( Range range ) {
    return all_elements_true(range);
  } 



*/

namespace OKlib {
  namespace InputOutput {
  }
}
