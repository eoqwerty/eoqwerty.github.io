CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -Wextra -lm -g

all: chess

chess: Chess.o Board.o Bishop.o Rook.o King.o Queen.o Pawn.o Knight.o CreatePiece.o main.o 
	$(CC) -o chess Chess.o Board.o Bishop.o Rook.o King.o Queen.o Pawn.o Knight.o CreatePiece.o main.o

Chess.o: Chess.cpp Piece.h Board.h
	$(CC) $(CFLAGS) -c Chess.cpp

Board.o: Board.cpp Piece.h Pawn.h Rook.h Knight.h Bishop.h Queen.h King.h Mystery.h
	$(CC) $(CFLAGS) -c Board.cpp

Bishop.o: Bishop.cpp Bishop.h Piece.h
	$(CC) $(CFLAGS) -c Bishop.cpp

Rook.o: Rook.cpp Rook.h Piece.h
	$(CC) $(CFLAGS) -c Rook.cpp

King.o: King.cpp King.h Piece.h
	$(CC) $(CFLAGS) -c King.cpp

Queen.o: Queen.cpp Queen.h Piece.h
	$(CC) $(CFLAGS) -c Queen.cpp

Pawn.o: Pawn.cpp Pawn.h Piece.h
	$(CC) $(CFLAGS) -c Pawn.cpp

Knight.o: Knight.cpp Knight.h Piece.h
	$(CC) $(CFLAGS) -c Knight.cpp

CreatePiece.o: CreatePiece.cpp CreatePiece.h Bishop.h Rook.h King.h Queen.h Pawn.h Knight.h Mystery.h
	$(CC) $(CFLAGS) -c CreatePiece.cpp

main.o: main.cpp Chess.h
	$(CC) $(CFLAGS) -c main.cpp

.PHONY: clean
clean:
	rm -f *.o chess
