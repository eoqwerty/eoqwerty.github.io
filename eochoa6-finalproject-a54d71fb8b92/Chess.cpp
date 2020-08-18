//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1
#include "Chess.h"
/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Chess::Chess() : is_white_turn(true) {
  // Add the pawns
  for (int i = 0; i < 8; i++) {
    board.add_piece(std::pair<char, char>('A' + i, '1' + 1), 'P');
    board.add_piece(std::pair<char, char>('A' + i, '1' + 6), 'p');
  }
  
  // Add the rooks
  board.add_piece(std::pair<char, char>( 'A'+0 , '1'+0 ) , 'R' );
  board.add_piece(std::pair<char, char>( 'A'+7 , '1'+0 ) , 'R' );
  board.add_piece(std::pair<char, char>( 'A'+0 , '1'+7 ) , 'r' );
  board.add_piece(std::pair<char, char>( 'A'+7 , '1'+7 ) , 'r' );
  
  // Add the knights
  board.add_piece(std::pair<char, char>( 'A'+1 , '1'+0 ) , 'N' );
  board.add_piece(std::pair<char, char>( 'A'+6 , '1'+0 ) , 'N' );
  board.add_piece(std::pair<char, char>( 'A'+1 , '1'+7 ) , 'n' );
  board.add_piece(std::pair<char, char>( 'A'+6 , '1'+7 ) , 'n' );
	
  // Add the bishops
  board.add_piece(std::pair<char, char>( 'A'+2 , '1'+0 ) , 'B' );
  board.add_piece(std::pair<char, char>( 'A'+5 , '1'+0 ) , 'B' );
  board.add_piece(std::pair<char, char>( 'A'+2 , '1'+7 ) , 'b' );
  board.add_piece(std::pair<char, char>( 'A'+5 , '1'+7 ) , 'b' );
	
  // Add the kings and queens
  board.add_piece(std::pair<char, char>( 'A'+3 , '1'+0 ) , 'Q' );
  board.add_piece(std::pair<char, char>( 'A'+4 , '1'+0 ) , 'K' );
  board.add_piece(std::pair<char, char>( 'A'+3 , '1'+7 ) , 'q' );
  board.add_piece(std::pair<char, char>( 'A'+4 , '1'+7 ) , 'k' );
}

Chess::Chess(std::map<std::pair<char, char>, Piece*> oc, bool white){ //used to create temp chess instances to test if a potential move will put the respective player in check
  board.setWMap(oc);
  is_white_turn = white;
}


// Attemps to make a move. If successful, the move is made and 
// the turn is switched white <-> black  
bool Chess::make_move(std::pair<char, char> start, std::pair<char, char> end) {
  //checking that there is indeed a piece at the start position
  if(board.operator()(start) == NULL){
    return false;
  }
  
  //checking if end position is occupied by an enemy piece
  bool end_occupied = false;
  if(board.operator()(end) != NULL && (board.operator()(start)->is_white() != board.operator()(end)->is_white())){
    end_occupied = true;
  }
  else if(board.operator()(end) != NULL){
    return false;
  }
  
  //checking that player is not trying to move other player's pieces
  if((is_white_turn && !(board.operator()(start))->is_white()) || (!is_white_turn && (board.operator()(start))->is_white()))
    {
      return false;
    }
  
  bool capture = false;
  //checking move shape is legal
  if (board.operator()(start)->legal_capture_shape(start, end) && end_occupied){
    capture = true;
  }
  else if(!(board.operator()(start)->legal_move_shape(start, end) && !end_occupied)){
    return false;
  }
	     
  //checking path is clear
  if(Chess::checklin(start, end) != 0){
    // checkpath returns true if path is clear
    if(!Chess::checkPath(start, end, Chess::checklin(start, end))){
      return false;
    }
  }
  if(!((is_white_turn && end_occupied && board.operator()(end)->to_ascii() == 'k') || (!is_white_turn && end_occupied && board.operator()(end)->to_ascii() == 'K'))){ //allowed to be in check if the move captures the opposing king
    //checking that this move doesn't put them in check
    std::map<std::pair<char, char>, Piece*> tempMap;
    Chess::copyMap(tempMap);
    Chess::setTempMap(tempMap, start, end); //make the move on a temporary map
    Chess c(tempMap, is_white_turn);
    
    if(c.in_check(c.is_white_turn))
      {
	return false;
      }
  }
  if(capture){
    delete board.operator()(end); //remove the successfully captured piece
  }
  board.set_occ(start, end, is_white_turn);
  is_white_turn = !is_white_turn;
  
  return true;  
}

// Returns true if the designated player is in check  
bool Chess::in_check(bool white) const {
  std::pair<char, char> kPos;
  for(std::map<std::pair<char, char>, Piece*>::const_iterator i = board.get_occ().cbegin(); i != board.get_occ().cend(); i++){ //find the player's king's position
    if(white && (i->second)->to_ascii() == 'K'){
      kPos.first = (i->first).first;
      kPos.second = (i->first).second;
    }
    if(!white && (i->second)->to_ascii() == 'k'){
      kPos.first = (i->first).first;
      kPos.second = (i->first).second;
    }
  }
  for(std::map<std::pair<char, char>, Piece*>::const_iterator i = board.get_occ().cbegin(); i != board.get_occ().cend(); i++){ //check if any of the opposing pieces can capture the player's king
    if((i->second)->is_white() != white){
      std::pair<char, char> tPos = {(i->first).first, (i->first).second};
      if((i->second)->legal_capture_shape(tPos, kPos) && Chess::checkPath(tPos, kPos, Chess::checklin(tPos, kPos))){
	return true;
      }
    }
  }
  return false;
}

void Chess::copyMap(std::map<std::pair<char, char>, Piece*>& tempMap) const{
  for(std::map<std::pair<char, char>, Piece*>::const_iterator i = board.get_occ().cbegin(); i != board.get_occ().cend(); i++){
    tempMap[i->first] = i->second;
  }
}

void Chess::setTempMap(std::map<std::pair<char, char>, Piece*>& tempMap, std::pair<char, char> start, std::pair<char, char> end) const{
  Piece* p = tempMap[start];
  tempMap[end] = p;
  tempMap.erase(start);
}

bool Chess::validMoveExists(bool white) const{
  std::map<std::pair<char, char>, Piece*> tempMap;
  Chess::copyMap(tempMap);
  for(std::map<std::pair<char, char>, Piece*>::const_iterator i = board.get_occ().cbegin(); i != board.get_occ().cend(); i++){
    if((white && (i->second)->is_white()) || (white == false && (i->second)->is_white() == false)){ //iterate through all of the current player's pieces in occ
      std::pair<char, char> initialPos = {(i->first).first, (i->first).second};
      //iterate through each possible position on the board and see if a move of this piece to that position will result in the current player being in check. If not, return true because a valid move exists
      for(char r = '8'; r >= '1'; r--){
	for(char c = 'A'; c <= 'H'; c++){
	  std::pair<char, char> tempPos = {c, r};
	  Chess chass(tempMap, is_white_turn);
	  if (chass.make_move(initialPos, tempPos)){
	    return true;
	  }
	}
      }
    }
  }
  return false;
}

bool Chess::in_mate(bool white) const {
  if(!(Chess::in_check(white))){ //if the current player is in checkmate, they must be in check
    return false;
  }
  if(Chess::validMoveExists(white)){ //if there is a way for the current player to get out of check, they are not in checkmate
    return false;
  }
  return true;
}


bool Chess::in_stalemate(bool white) const{
  if(Chess::validMoveExists(white) || Chess::in_check(white)){ //same as in_mate but the player is not currently in check
    return false;
  }
  return true;
}

char Chess::checklin(std::pair<char, char> start, std::pair<char, char> end) const{
  //checking for vertical line                                                                    
  if(start.first == end.first){
    if(start.second-end.second > 0){ //path is down
      return 'd';
    }
    if(start.second-end.second < 0){ //path is up
      return 'u';
    }
  }
  
  // checking for horizontal line
  if(start.second == end.second){
    if(start.first - end.first > 0){ //path is left
      return 'l';
    }
    if(start.first - end.first < 0){ //path is right
      return 'r';
    }
  }
  int difH = end.first - start.first;
  int difV = end.second - start.second;
  
  //checking for diagonal                                                                      
  if(std::abs(start.first - end.first) == std::abs(start.second - end.second)){
    if(difH > 0){
      if(difV >0){
	return '1'; //up/right
      }
      return '4'; //down/right
    }
    if(difH < 0){
      if(difV <0){
	return '3'; //down/left
      }
      return '2'; //up/left
    }
  }
  return 0;
}

bool Chess::checkPath(std::pair<char, char> start, std::pair<char, char> end, char linear) const{
  std::pair<char, char> temp = {start.first, start.second};
  if(linear == 0){ //assume pieces that move with a non-linear path can move through other pieces 
    return true;
  }
  do{ //iterate through each position the given piece will move through and ensure the path is clear from start to end (with the exception of the end position)
    temp.second+=(linear=='u' || linear == '1' || linear == '2') ? 1 : (linear == 'l' || linear == 'r') ? 0 : -1;
    temp.first+=(linear == '1' || linear == '4' || linear == 'r') ? 1 : (linear == 'u' || linear == 'd') ? 0 : -1;
    if(!(board.operator()(temp) == NULL)){
      if (temp.first==end.first && temp.second==end.second)
	{
	  break; //this is a capture
	}
      return false;
    }
  }while(temp.first!=end.first || temp.second!=end.second);
  return true;
}

void Chess::setBoard(Board b){
  board.clearBoard();
  board = b;
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
std::ostream& operator<< (std::ostream& os, const Chess& chess) {
  // Write the board out and then either the character 'w' or the character 'b',
  // depending on whose turn it is
  return os << chess.get_board() << (chess.turn_white() ? 'w' : 'b');
}

std::istream& operator>> (std::istream& is, Chess& chess){
  char x;
  int count = 0;
  int col, row;
  Board newBoard;
  while(is >> x && count <64){ //while there are characters to be read from the input stream, add all the correct pieces to the right locations on a temporary board and use setBoard to apply these changes to this instance's board
    col = 'A'+(count%8);
    row = '8'-(count/8);
    if(x != '-'){
      newBoard.add_piece(std::pair<char, char>( col, row ), x );
    }
    count++;
  }
  chess.setBoard(newBoard);
  if (x == 'w'){ //set the current turn based on the input stream
    chess.setTurn(true);
  }
  else{
    chess.setTurn(false);
  }
  return is;
}
