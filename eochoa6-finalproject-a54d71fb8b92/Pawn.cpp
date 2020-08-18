//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1
#include "Pawn.h"
#include <cmath>


bool Pawn::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const{
  //checks whether it's the pawns first move based on what row they're in
  bool first_move = false;
  if((is_white() && start.second == '2') || (!is_white() && start.second == '7')){
    first_move = true;
  }
  //horizontal and vertical differences, respectively
  int hdif = start.first-end.first;
  int vdif = start.second-end.second;
  int move_one;
  int move_two;

  //define legal first move and second move, depending on color
  if(is_white()){
    move_one = -1;
    move_two = -2;
  }
  else{
    move_one = 1;
    move_two = 2;
  }

  //checks that it's only moving vertically, and that it's only moving 2 spaces if it's the first move
  if(std::abs(hdif) != 0 || (vdif != move_one && vdif != move_two) || (vdif == move_two && first_move == false)){
    return false;
  }

  //checks that end is within the 8x8 board
  if(end.first > 'H' || end.first < 'A' || end.second > '8' || end.second < '1'){
    return false;
  }
  
  return true;
    
}
		
bool Pawn::legal_capture_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
  //defines horizontal and vertical movements, respectively
  int hdif = start.first-end.first;
  int vdif = start.second-end.second;
  //defines legal vertical movement, according to color pawn
  int vertical;
  if (is_white()){
    vertical = -1;
  }
  else {
    vertical = 1;
  }
  //check that move is one space diagonal in the correct vertical direction
  if (std::abs(hdif) != 1 || vdif != vertical){
    return false;
  }
  //check that end is within 8x8 board
  if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.second < '1'){
    return false;
  }

  return true;


}
