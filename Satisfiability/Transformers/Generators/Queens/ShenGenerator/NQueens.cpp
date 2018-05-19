// https://sites.google.com/site/haioushen/search-algorithm/solvean-queensproblemusingsatsolver

#include <vector>
#include <stack>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

void EqualOneToCNF(std::vector<int> literals, std::vector<std::vector<int>> & cnf){
  int N = literals.size();
  cnf.push_back(literals);

  std::vector<int> dnf;
  // this one is a->~b, a->~c, ...
  for (int j=0; j<N-1; j++){
    for (int k=j+1; k<N; k++){
      dnf.push_back(-literals[j]);
      dnf.push_back(-literals[k]);
      cnf.push_back(dnf);
      dnf.clear();
    }
  }
}

void LessEqualOneToCNF(std::vector<int> literals, std::vector<std::vector<int>> & cnf){
  int N = literals.size();
  std::vector<int> dnf;
  // this one is a->~b, a->~c, ...
  for (int j=0; j<N-1; j++){
    for (int k=j+1; k<N; k++){
      dnf.push_back(-literals[j]);
      dnf.push_back(-literals[k]);
      cnf.push_back(dnf);
      dnf.clear();
    }
  }
}

int main(int argc, char * argv[]){

  int N = std::atoi(argv[1]);
  int numVars = N*N;
  int kk=1;
  int ** VarName = new int * [N];
  for (int i=0; i<N; i++){
    VarName[i] = new int [N];
    for (int j=0; j<N; j++){
      VarName[i][j] = kk++;
    }
  }

  std::vector<std::vector<int>> cnf;

  std::vector<int> vars;

  for (int i=0; i<N; i++){
    // generator formula per row
    // v1 + v2 + v3 + v4 + ... = 1r
    for (int j=0; j<N; j++){
      vars.push_back(VarName[i][j]);
    }
    EqualOneToCNF(vars, cnf);
    vars.clear();
  }

  for (int i=0; i<N; i++){
    // generator formula per col
    // v1 + v2 + v3 + v4 + ... = 1r
    for (int j=0; j<N; j++){
      vars.push_back(VarName[j][i]);
    }
    EqualOneToCNF(vars, cnf);
    vars.clear();
  }

  // diagonal
  for (int i=0; i<N-1; i++){
    for (int j=0; j<N-i; j++){
      vars.push_back(VarName[j][i+j]);
    }
    LessEqualOneToCNF(vars, cnf);
    vars.clear();
  }

  for (int i=1; i<N-1; i++){
    for (int j=0; j<N-i; j++){
      vars.push_back(VarName[j+i][j]);
    }
    LessEqualOneToCNF(vars, cnf);
    vars.clear();
  }

  for (int i=0; i<N-1; i++){
    for (int j=0; j<N-i; j++){
      vars.push_back(VarName[j][N-1 - (i+j)]);
    }
    LessEqualOneToCNF(vars, cnf);
    vars.clear();
  }

  for (int i=1; i<N-1; i++){
    for (int j=0; j<N-i; j++){
      vars.push_back(VarName[j+i][N-1-j]);
    }
    LessEqualOneToCNF(vars, cnf);
    vars.clear();
  }

  std::ofstream fout;
  fout.open(argv[2], std::ofstream::out);

  fout << "p cnf " << numVars << " " << cnf.size() << std::endl;

  for (unsigned int i=0; i<cnf.size(); i++){
    for (unsigned int j=0; j<cnf[i].size(); j++){
      fout << cnf[i][j] << " ";
    }
    fout << " 0" << std::endl;
  }
  fout << std::endl;

  return 0;
}
