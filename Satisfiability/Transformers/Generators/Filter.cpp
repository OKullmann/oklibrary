// Oliver Kullmann, 7.10.2002 (Swansea)

#include <iostream>
#include <string>
#include <map>

#include "StringHandling.hpp"

using namespace std;
using namespace StringHandling;

int main() {
  map<string,unsigned int> Count;
  string var;
  while (cin >> var) {
    string assign;
    cin >> assign;
    if (assign == "0" or var.find("ident") != 0)
      continue;
    deleteHead_var(var, 5);
    string::size_type ch = var.find("choice");
    var.erase(ch, 6);
    string choice = var.substr(ch, string::npos);
    var.erase(ch);
    cout << var << " " << choice << "\n";
    ++Count[choice];
  }
  cout << "\n";
  for (map<string,unsigned int>::iterator i = Count.begin(); i != Count.end(); ++i)
    cout << i -> first << ": " << i -> second << "\n";
}
