//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1
#include "Queen.h"
#include <cmath>

bool Queen::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const{
  int hdif;
  int vdif;
  //gets horizontal and vertical movement, respectively
  hdif=(start.first-end.first);
  vdif=(start.second-end.second);
  //checks that start and end are not the same, and that if a piece moves both vertically and horizontally
  //their differences must be equal
  if((((std::abs(hdif) > 0 && std::abs(vdif) > 0) && std::abs(hdif) != std::abs(vdif))) || (hdif == 0 && vdif == 0)){
    return false;
  }
  //checks that the end location is within the 8x8 board
  if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.second < '1'){
    return false;
  }
  return true;

}
