//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1
#include "Rook.h"
#include <cmath>

bool Rook::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const{
  //gets horizontal and vertical differences respectively
  int hdif;
  int vdif;
  hdif=(start.first-end.first);
  vdif=(start.second-end.second);
  //checks that the piece moves eitiher horizontally or vertically, at
  if((std::abs(hdif) > 0 && std::abs(vdif) > 0) || (hdif == 0 && vdif == 0)){
    return false;
  }
  if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.second < '1'){
    return false;
  }
  return true;
   
}
