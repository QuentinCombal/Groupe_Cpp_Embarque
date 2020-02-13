// Copyright [2020] <Moi>
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      mean_and_median.cpp
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Bernard
//
// \copyright Copyright ng2goodies 2015
//            Distributed under the MIT License
//            See http://opensource.org/licenses/MIT
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//


// C++ version
// Compilation with g++ 5.3.0 & g++ 6.3.0
// mingwin: g++ -std=c++14 -O3 -o mean_and_median mean_and_median.cpp

// Compilation with g++ 4.9.3
// cygwin:  g++ -D_GLIBCXX_USE_C99 -D_GLIBCXX_USE_C99_DYNAMIC -std=c++14 -O3 ...

// Other tool chain
// msvc:    ok!
// clang:   ok!


#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>

using std::string;
using std::vector;
using std::map;

// This simple program reads from a file a set of numbers (double format)
// computes a running mean value, computes the median after sort

// This program has several problems, can you spot them

int main(int argc, char *argv[]) {
  string file_name{argv[1]};
  map<string, double> tab;
  string adr_courante;
  double val_courante;
  std::ifstream fin(file_name, std::ios::in);

  map<string, double>::iterator it;

  while (fin >> adr_courante >> val_courante) {
    tab[adr_courante] = val_courante;
  }

  string entree;

  int fini = 1;

  while (fini == 1) {
    std::cout << "query>";
    std::cin >> entree;

    if (entree == "END") {
      std::cout << "Bye..." << std::endl;
      fini = 0;
    } else {
      it = tab.find(entree);
      if (it == tab.end()) {
      std::cout << "This ID does not exist" << std::endl;
      } else {
      std::cout << "value[" << entree << "] = " << tab[entree] << std::endl;
      }
    }
  }
}
