//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1
#include "Bishop.h"
#include <cmath>

bool Bishop::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const{
  //gets horizontal and vertical movement, respectively
  int hdif = (start.first-end.first);
  int vdif = (start.second-end.second);
  //checks that horizontal difference = vertical difference, and that neither are 0 (it doesn't stay in the same spot)
  if(std::abs(hdif) != std::abs(vdif) || hdif == 0){
    return false;
  }
  //checks that the end location is within the 8x8 board
  if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.second < '1'){
    return false;
  }
  return true;
}
