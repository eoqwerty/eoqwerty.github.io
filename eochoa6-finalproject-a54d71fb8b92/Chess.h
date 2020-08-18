//Emi Ochoa Nader Najjar Casey Levitt
//JHEDS: eochoa6 nnajjar2 clevitt1
#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include "Piece.h"
#include "Board.h"

class Chess {

public:
  // This default constructor initializes a board with the standard
  // piece positions, and sets the state to white's turn
  Chess();
  
  Chess(std::map<std::pair<char, char>, Piece*> oc, bool white);
  
  ~Chess(){ board.clearBoard(); }
  // Returns a constant reference to the board 
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  const Board& get_board() const { return board; }
  
  // Returns true if it is white's turn
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  bool turn_white() const { return is_white_turn; }
  
  // Attemps to make a move. If successful, the move is made and
  // the turn is switched white <-> black
  bool make_move(std::pair<char, char> start, std::pair<char, char> end);
  
  // Returns true if the designated player is in check
  bool in_check(bool white) const;
  
  // Returns true if the designated player is in mate
  bool in_mate(bool white) const;
  
  // Returns true if the designated player is in mate
  bool in_stalemate(bool white) const;
  
  // Returns character indicating direction of move
  //('u' 'd' 'l' 'r', '1' '2' '3' '4' (indicating the 4 quadrants))
  char checklin(std::pair<char, char> start, std::pair<char, char> end) const;

  //Checks that there are no pieces blocking path of move (returns true if path is clear)
  bool checkPath(std::pair<char, char> start, std::pair<char, char> end, char linear) const;

  //shallow copy of this.board.occ to the tempMap parameter
  void copyMap(std::map<std::pair<char, char>, Piece*>& tempMap) const;

  //move the piece in the start position to the end position in tempMap
  void setTempMap(std::map<std::pair<char, char>, Piece*>& tempMap, std::pair<char, char> start, std::pair<char, char> end) const;

  //checks if any move can be made by the current player without putting them in check by the opposing player
  bool validMoveExists(bool white) const;

  //delete all dynamically allocated pieces in this board and do a shallow copy of b (which is assumed to have dynamically allocated pieces) into board
  void setBoard(Board b);
  
  void setTurn(bool is_white) {is_white_turn = is_white;}
  
  
private:
  // The board
  Board board;
  
  // Is it white's turn?
  bool is_white_turn;
  
};

// Writes the board out to a stream
std::ostream& operator<< (std::ostream& os, const Chess& chess);

// Reads the board in from a stream
std::istream& operator>> (std::istream& is, Chess& chess);


#endif // CHESS_H
