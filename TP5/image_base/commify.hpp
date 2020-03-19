#ifndef COMMIFY_H
#define COMMIFY_H

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

class Commify
{
private:
  std::vector<int> three_digit_chunks;
public:
  explicit Commify(int value){
    int temp_value;
    do{
      temp_value = value/1000;
      three_digit_chunks.push_back(value-temp_value*1000);
      value = temp_value;
    }while(value != 0);
  std::reverse(three_digit_chunks.begin(), three_digit_chunks.end());
  }

  friend std::ostream& operator<<(std::ostream &os, const Commify &c){
    int value;
    os << c.three_digit_chunks[0];
    for (unsigned int i = 1; i < c.three_digit_chunks.size(); ++i)
    {
      value = c.three_digit_chunks[i];
      if (value > 100)
        os << ',' << c.three_digit_chunks[i];
      else if (value > 10)
        os << ',' << "0" << c.three_digit_chunks[i];
      else
        os << ',' <<"00" << c.three_digit_chunks[i];
    }
    return os;
  }
};

#endif // COMMIFY_H
