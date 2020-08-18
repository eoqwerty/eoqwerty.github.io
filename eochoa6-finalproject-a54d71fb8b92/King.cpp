//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1
#include "King.h"
#include <cmath>

bool King::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const{
  //horizontal and vertical differences, respectively 
  int hdif;
  int vdif;
  hdif=(start.first-end.first);
  vdif=(start.second-end.second);
  //checks that it's not staying in the same place
  //and that it's moving by just one space either horizontally or vertically
  if((std::abs(hdif) >1 || std::abs(vdif) >1) || (hdif == 0 && vdif == 0)){
    return false;
  }
  //makes sure that the end move is within 8x8 board
  if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.second < '1'){
    return false;
  }
  return true;
}
