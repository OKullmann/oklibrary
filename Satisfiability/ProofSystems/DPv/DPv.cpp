/*!
  \file Transitional/Satisfiability/ProofSystems/DPv/DPv.cpp
  \deprecated
*/

#include <iostream>
#include <cstdlib>
#include <string>

#include <Transitional/Satisfiability/ProofSystems/DPv/DPv.hpp>
#include <Transitional/Satisfiability/ProofSystems/DPv/Statistics.hpp>
#include <Transitional/Satisfiability/ProofSystems/DPv/Input_output.hpp>
#include <Transitional/Satisfiability/ProofSystems/DPv/BasicDataStructure.hpp>

int main(const int argc, const char* const argv[]){
  
  if (argc != 2) { 
    std::cout << "[DPv] Error: Program requires exact one cnf input \n";
    return EXIT_FAILURE;
  }
  
  const std::string filename = argv[1];
  
  OKlib::DPv::Input::FStream input(filename.c_str());
  
  OKlib::DPv::Dimacs data;
  input.filestream >> data; 
  
  std::cout << "F = "; 
  data.formula.show(std::cout); 
  std::cout<<"\n";
  
  OKlib::DPv::stat_type s1;
  OKlib::DPv::VariableSet vs1(data.formula);
  OKlib::DPv::Best_order(OKlib::DPv::DPv_opt_stats(vs1, data.formula,s1));

  int response = 9;
  int empty = 9;
  
  OKlib::DPv::Clause_set result;
  result.cs = data.formula.cs;
  OKlib::DPv::VariableSet vs(result);
  empty = vs.vars.size();
  while (empty != 0 and response != 0) {
    std::cout << "Variables: "; vs.show(std::cout); std::cout<< '\n';
    std::cout << result;
    std::cout << "Pick a Variable: ";
    std::cin >> response;
    result.cs = DP_reduction(OKlib::DPv::Variable(response), result).cs;
    vs.vars = OKlib::DPv::VariableSet(result).vars;
    empty = vs.vars.size();
    std::cout << "F = "; result.show(std::cout); std::cout<<"\n";
  }
}
