// https://sites.google.com/site/haioushen/search-algorithm/solvean-queensproblemusingsatsolver

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

void EqualOneToCNF(const std::vector<int>& literals, std::vector<std::vector<int>>& cnf){
  const unsigned int N = literals.size();
  cnf.push_back(literals);
  if (N <= 1) return;

  std::vector<int> dnf;
  // this one is a->~b, a->~c, ...
  for (unsigned int j=0; j<N-1; ++j)
    for (unsigned int k=j+1; k<N; ++k) {
      dnf.push_back(-literals[j]);
      dnf.push_back(-literals[k]);
      cnf.push_back(dnf);
      dnf.clear();
    }
}

void LessEqualOneToCNF(const std::vector<int>& literals, std::vector<std::vector<int>> & cnf){
  const unsigned int N = literals.size();
  if (N <= 1) return;
  std::vector<int> dnf;
  // this one is a->~b, a->~c, ...
  for (unsigned int j=0; j<N-1; ++j)
    for (unsigned int k=j+1; k<N; ++k) {
      dnf.push_back(-literals[j]);
      dnf.push_back(-literals[k]);
      cnf.push_back(dnf);
      dnf.clear();
    }
}

int main(const int argc, const char* const argv[]){

  if (argc != 3) {
    std::cout << "Usage[NQueens]: N filename\n";
    return 0;
  }

  const unsigned int N = std::stoul(argv[1]);
  const unsigned int numVars = N*N;
  int** const VarName = new int* [N];
  {int kk=0;
   for (unsigned int i=0; i<N; ++i) {
     VarName[i] = new int [N];
     for (unsigned int j=0; j<N; ++j) VarName[i][j] = ++kk;
   }
  }

  std::vector<std::vector<int>> cnf;

  std::vector<int> vars;

  for (unsigned int i=0; i<N; ++i) {
    // row constraints
    for (unsigned int j=0; j<N; ++j) vars.push_back(VarName[i][j]);
    EqualOneToCNF(vars, cnf);
    vars.clear();
  }

  for (unsigned int i=0; i<N; ++i) {
    // column constraints
    for (unsigned int j=0; j<N; ++j) vars.push_back(VarName[j][i]);
    EqualOneToCNF(vars, cnf);
    vars.clear();
  }

  if (N >= 2) {
    // diagonal constraints
    for (unsigned int i=0; i<N-1; ++i) {
      for (unsigned int j=0; j<N-i; ++j) vars.push_back(VarName[j][i+j]);
      LessEqualOneToCNF(vars, cnf);
      vars.clear();
    }
    for (unsigned int i=1; i<N-1; ++i) {
      for (unsigned int j=0; j<N-i; ++j) vars.push_back(VarName[j+i][j]);
      LessEqualOneToCNF(vars, cnf);
      vars.clear();
    }
    for (unsigned int i=0; i<N-1; ++i) {
      for (unsigned int j=0; j<N-i; ++j) vars.push_back(VarName[j][N-1 - (i+j)]);
      LessEqualOneToCNF(vars, cnf);
      vars.clear();
    }
    for (unsigned int i=1; i<N-1; ++i) {
      for (unsigned int j=0; j<N-i; ++j) vars.push_back(VarName[j+i][N-1-j]);
      LessEqualOneToCNF(vars, cnf);
      vars.clear();
    }
  }

  std::ofstream fout(argv[2]);
  fout << "p cnf " << numVars << " " << cnf.size() << "\n";
  for (unsigned int i=0; i<cnf.size(); ++i) {
    for (unsigned int j=0; j<cnf[i].size(); ++j) fout << cnf[i][j] << " ";
    fout << " 0" << "\n";
  }
  fout << "\n";
}
