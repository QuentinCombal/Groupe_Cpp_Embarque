// Copyright [2020] <Quentin Paul Jiang>
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      map2.cpp
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
// mingwin: g++ -std=c++14 -O3 -o map2 map2.cpp

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
  string input;

  while ( !end ) {
    // Read query
    std::cout << "Query> ";
    std::cin >> input;

    if (input == "END") {
      end = 1;
      std::cout << "Bye..." << "\n";

    } else if (input[0] == '+') {
      // Input is a value to look for
      // Look for values between v-1% and v+1% and output any matching element
      val = std::stod(input);
      double min = val*0.99;
      double max = val*1.01;

      for (auto it = buf.begin(); it != buf.end() ; ++it) {
        // it-> first is the key and iy->second is the corresponding value
        if ( (it->second > min) && (it->second < max) )
          std::cout << "Value[" << it->first << "] = " << it->second <<"\n";
      }

    } else if (buf.find(input) == buf.end()) {
      // Input is a key that does not exist
      std::cout << "ID could not be found" << "\n";

    } else {
      // Input is a key that exists
      std::cout << "Value[" << input << "] = " << buf[input] <<"\n";
    }
  }
}
