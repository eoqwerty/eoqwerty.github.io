//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1
#include "Knight.h"
#include <cmath>

bool Knight::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const{
  int dx = end.first - start.first;
  int dy = end.second - start.second;
  if(start.first + dx > 'H' || start.first + dx < 'A' || start.second + dy > '8' || start.second + dy < '1'){
    return false;
  }
  if((std::abs(dx) != 1 && std::abs(dx) != 2) || (std::abs(dy) != 1 && std::abs(dy) != 2) || (std::abs(dx) == 1 && std::abs(dy) != 2) || (std::abs(dx) == 2 && std::abs(dy) != 1)){
    return false;
  }
  return true;
}
