// Copyright [2020] <Quentin Paul Jiang>
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      map1.cpp
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
// mingwin: g++ -std=c++14 -O3 -o map1 map1.cpp

// Compilation with g++ 4.9.3
// cygwin:  g++ -D_GLIBCXX_USE_C99 -D_GLIBCXX_USE_C99_DYNAMIC -std=c++14 -O3 ...

// Other tool chain
// msvc:    ok!
// clang:   ok!


#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

using std::string;
using std::vector;
using std::map;


int main(int argc, char *argv[]) {
  string file_name{argv[1]};
  map<string, double> buf;
  std::ifstream fin(file_name, std::ios::in);

  string key;
  double val;

  while (fin >> key >> val) {
    buf[key] = val;
  }

  // Read query until "END" is entered
  bool end = 0;
  while ( !end ) {
    std::cout << "Query> ";
    std::cin >> key;
    if (key == "END")
      end = 1;
    else if (buf.find(key) == buf.end())
      std::cout << "ID could not be found" << "\n";
    else
      std::cout << "Value[" << key << "] = " << buf[key] <<"\n";
  }
}
