#include <OKlib/Satisfiability/FiniteFunctions/SATParser.hpp>
#include <OKlib/Satisfiability/FiniteFunctions/QuineMcCluskey.hpp>
#include<fstream>
#include<iostream>
#include<set>

int main(int argc, const char* argv[]) {
    std::ifstream inputFile (argv[1]);
    std::vector< std::vector<int> > clauseSet = readDIMACSFormat(&inputFile); 
    std::vector< std::vector<int> > resultSet = quineMcCluskey(clauseSet);
    // List Clauses
    for (std::vector< std::vector<int> >::iterator iter = resultSet.begin(); iter != resultSet.end(); iter++) {
       printClause(*iter); 
    }
}
