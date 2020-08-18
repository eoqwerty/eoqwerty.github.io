//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1

#include <iostream>
#include <utility>
#include <map>
#include "Board.h"
#include "CreatePiece.h"
#include "Terminal.h"

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Board::Board(){}

// Returns a const pointer to the piece at a prescribed location if it exists,       
// or nullptr if there is nothing there.                                       
const Piece* Board::operator()(std::pair<char, char> position) const {
  if((occ.find(position) == occ.end())){
    return NULL;
  }
  else{
    return occ.at(position);
  }
}

bool Board::add_piece(std::pair<char, char> position, char piece_designator) {
  Piece* p = create_piece(piece_designator);
  if(p == NULL){ //invalid piece
    return false;
  }
  if(position.first < 'A' || position.first > 'H' || position.second < '0' || position.second > '8'){ //invalid position
    delete(p);
    return false;
  }
  
  if(operator()(position) != NULL){ //already occupied
    delete(p);
    return false;
  }
  occ[position] = p;
  return true;
}

bool Board::has_valid_kings() const {
  int numWhite = 0;
  int numBlack = 0;
  for(std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.cbegin(); //iterate through occ and ensure there is 1 white king and 1 black king
      it!= occ.cend();
      ++it){
    
    if((it->second)->to_ascii()=='k'){
      numBlack++;
    }
    if((it->second)->to_ascii()=='K'){
      numWhite++;
    }
  }
  if(numBlack != 1 || numWhite !=1){
    return false;
  }
  return true;
}

void Board::display() const {
  Terminal::color_bg(Terminal::WHITE);
  Terminal::color_fg(false, Terminal::BLUE);
  std::cout << "    A  B  C  D  E  F  G  H    ";
  Terminal::set_default();
  std::cout << std::endl;
  bool blue = true;
  for(char r = '8'; r > '0'; r--){
    blue = !blue;
    for(int c = 'A'-1; c < 'I'+1; c++){
      if (c == 'A'-1 || c == 'I')
	{
	  Terminal::color_bg(Terminal::WHITE);
	  Terminal::color_fg(false, Terminal::BLUE);
	  std::cout << " " << r << " ";
	}
      else
	{
	  if (blue)
	    {
	      Terminal::color_bg(Terminal::CYAN);
	    }
	  else
	    {
	      Terminal::color_bg(Terminal::MAGENTA);
	    }
	  blue = !blue;
	  std::pair<char, char> p;
	  p.first = c;
	  p.second = r;
	  const Piece* piece = operator()(p);
	  if(piece){
	    if(piece->is_white())
	      {
		Terminal::color_fg(true, Terminal::WHITE);
	      }
	    else
	      {
		Terminal::color_fg(true, Terminal::BLACK);
	      }
	    std::cout << " "  << piece->to_ascii() << " ";
	  }
	  else{
	    std::cout << "   ";
	  }
	}
      Terminal::set_default();
    }
    std::cout << std::endl;
  }
  Terminal::color_bg(Terminal::WHITE);
  Terminal::color_fg(false, Terminal::BLUE);
  std::cout << "    A  B  C  D  E  F  G  H    ";
  Terminal::set_default();
  std::cout << std::endl;
}
void Board::set_occ(std::pair<char, char> start, std::pair<char, char> end, bool white){
  if(Board::promotionCheck(start, end)){ //if a pawn is being promoted (moved to the opposite end of the board)
    delete(Board::operator()(start)); //delete the dynamically allocated pawn
    if(white){ //and add a queen to the end position
      add_piece(end, 'Q');
    }
    else{
      add_piece(end, 'q');
    }
  }
  else{ //otherwise move a piece to another position normally
    Piece* p = occ[start];
    occ[end] = p;
  }
  occ.erase(start);
}


bool Board::promotionCheck(std::pair<char, char> start, std::pair<char, char> end){
  if((occ[start]->to_ascii() == 'P' && end.second == '8') || (occ[start]->to_ascii() == 'p' && end.second == '1')){
    return true;
  }
  return false;
}

void Board::clearBoard(){
  for(std::map<std::pair<char, char>, Piece*>::iterator i = occ.begin(); i != occ.end(); ++i){
    delete i->second; //delete dynamically allocated piece
  }
}

void Board::setWMap(std::map<std::pair<char, char>, Piece*> oc) {
  for (std::map<std::pair<char, char>, Piece*>::iterator it = oc.begin();
       it!= oc.end();
       it++){
    occ[it->first] = create_piece((it->second)->to_ascii()); //dynamically allocate new pieces based off of the ones in oc and put them in occ
  }  
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Board& board) {
  for(char r = '8'; r >= '1'; r--) {
    for(char c = 'A'; c <= 'H'; c++) {
      const Piece* piece = board(std::pair<char, char>(c, r));
      if (piece) {
	os << piece->to_ascii();
      } else {
	os << '-';
      }
    }
    os << std::endl;
  }
  return os;
}

