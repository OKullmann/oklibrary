#include <iostream>

#include "Input_output.hpp"
#include "BasicDataStructure.hpp"
#include "Statistics.hpp"
#include "DPv.hpp"

int main(const int argc, const char* const argv[]) {
  
  std::cout << argc << argv[0] << argv[1];
  OKlib::DPv::Input::FStream input(argv[1]);
  
  OKlib::DPv::Dimacs a;
  input.filestream >> a; 
  
  for (OKlib::DPv::Dimacs::const_iterator i = a.formula.cs.begin(); i != a.formula.cs.end(); ++i) {
    
    for (OKlib::DPv::Clause::const_iterator j = (*i).c.begin(); j !=(*i).c.end();++j)
      
    std::cout << (*j).l << '\t';
    std::cout << '\n';
  }
  
  OKlib::DPv::VariableSet vs(a.formula);
  vs.show(); std::cout << '\n';
  OKlib::DPv::LiteralSet ls(a.formula);
  ls.show(); std::cout << '\n';

  std::cout << "clause count is " << OKlib::DPv::clause_count(a.formula) << '\n';
  std::cout << "literal 1 count is " << OKlib::DPv::ldg(1,a.formula) << '\n';
  std::cout << "literal -1 count is " << OKlib::DPv::ldg(-1,a.formula) << '\n';
  std::cout << "literal 2 count is " << OKlib::DPv::ldg(2,a.formula) << '\n';
  std::cout << "literal -2 count is " << OKlib::DPv::ldg(-2,a.formula) << '\n';
  std::cout << "literal 3 count is " << OKlib::DPv::ldg(3,a.formula) << '\n';
  std::cout << "literal -3 count is " << OKlib::DPv::ldg(-3,a.formula) << '\n';
  std::cout << "Variable 1 count is " << OKlib::DPv::vdg(1,a.formula) << '\n';
  std::cout << "Variable 2 count is " << OKlib::DPv::vdg(2,a.formula) << '\n';
  std::cout << "Variable 3 count is " << OKlib::DPv::vdg(3,a.formula) << '\n';

  OKlib::DPv::Clause x,y,z;
  cl_insert(OKlib::DPv::Literal(1),x);
  cl_insert(OKlib::DPv::Literal(-4),x);
  
  cl_insert(OKlib::DPv::Literal(-1),z);
  cl_insert(OKlib::DPv::Literal(-4),z);
  cl_insert(OKlib::DPv::Literal(8),z);
  cl_insert(OKlib::DPv::Literal(-5),z);
  show(x); std::cout << '\n';
  y.c = OKlib::DPv::clause_compliment(x).c;
  show(y); std::cout << '\n';
  
  if (resolvable(1,x,z))
    std::cout << "Can \n";
  else
    std::cout << "Can Not \n";
  show(resolvent(x,z));std::cout << '\n';

  std::cout << "resolve on 1 gives " << clause_left(1,a.formula) << '\n';
  show(DP_reduction(1,a.formula)); std::cout << '\n';
  std::cout << "resolve on 2 gives " << clause_left(2,a.formula) << '\n';
  show(DP_reduction(2,a.formula)); std::cout << '\n';
  std::cout << "resolve on 3 gives " << clause_left(3,a.formula) << '\n';
  show(DP_reduction(3,a.formula)); std::cout << '\n';
  std::cout << "resolve on 4 gives " << clause_left(4,a.formula) << '\n';
  show(DP_reduction(4,a.formula)); std::cout << '\n';

}
