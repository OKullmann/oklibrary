#include <iostream>
#include <cstdlib>

#include "DPv.hpp"
#include "Statistics.hpp"
#include "Input_output.hpp"
#include "BasicDataStructure.hpp"

int main(const int argc, const char* const argv[]) {
  if (argc != 2) { 
    std::cout << "[DPv] Error: Program requires exact one cnf input \n";
    return EXIT_FAILURE;
  }
  
  OKlib::DPv::Input::FStream input(argv[1]);
  
  OKlib::DPv::Dimacs data;
  input.filestream >> data; 
  
  std::cout << "F = "; show(data.formula); std::cout<<'\n';
  
  int response = 9;
  int empty = 9;
  
  OKlib::DPv::Clause_set result;
  result.cs = data.formula.cs;
  OKlib::DPv::VariableSet vs(result);
  empty = vs.vars.size();
  while (empty != 0) {
    std::cout << "Variables: "; vs.show(); std::cout<< '\n';
    stat(result);
    std::cout << "Pick a Variable ";
    std::cin >> response;
    result.cs = DP_reduction(OKlib::DPv::Variable(response), result).cs;
    vs.vars = OKlib::DPv::VariableSet(result).vars;
    empty = vs.vars.size();
    //std::cout << "term " << empty;
    std::cout << "F = "; show(result); std::cout<<'\n';
  }

}
