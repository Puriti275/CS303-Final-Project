#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include "chess.h"

using namespace std;

ChessBoard::ChessBoard() {
    //for simplicity, the player will always play as black

    // initialize pieces
    // 11/10/2026 updated to remove the value (pmyers)
    Piece RookB = {1, 'R'};
    Piece KnightB = {1, 'N'};
    Piece BishopB = {1, 'B'};
    Piece QueenB = {1, 'Q'};
    Piece KingB = {1, 'K'};
    Piece PawnB = {1, 'P'};

    Piece RookW = {0, 'R'};
    Piece KnightW = {0, 'N'};
    Piece BishopW = {0, 'B'};
    Piece QueenW = {0, 'Q'};
    Piece KingW = {0, 'K'};
    Piece PawnW = {0, 'P'};

    // One cell of the board. no piece pointer on init (pmyers)
    Cell cell = {0, 0, nullptr};
    
    vector<Cell> boardRow; // a row of cells is a row of the board (pmyers)

    for(int i = 0; i < 8; i++) { // 8 x 8 rows and columns for chess board (pmyers)
        for(int j = 0; j < 8; j++) {
            cell.col = j;
            cell.row = i;
            boardRow.push_back(cell); // cell's values are overwritten and board is created on contructor call (pmyers)
        }    

        board.push_back(boardRow);
        boardRow.clear(); boardRow.shrink_to_fit(); // housekeeping for memory alloc (pmyers)
    }

    // add pieces to piece vector, fixed the 500 pushbacks (pmyers)
    pieces = {QueenB, KingB, RookB, RookB, KnightB, KnightB, BishopB, BishopB, 
	      QueenW, KingW, RookW, RookW, KnightW, KnightW, BishopW, BishopW};
    
    pieces.insert(pieces.end(), 8, PawnB);   // 16-23 black pawns (pmyers)
    pieces.insert(pieces.end(), 8, PawnW);   // 24-31 white pawns (pmyers)

    board[0][0].pieceOccupied = &pieces[2]; //RookB
    board[0][1].pieceOccupied = &pieces[4]; //KnightB
    board[0][2].pieceOccupied = &pieces[6]; //BishopB
    board[0][3].pieceOccupied = &pieces[0]; //QueenB
    board[0][4].pieceOccupied = &pieces[1]; //KingB
    board[0][5].pieceOccupied = &pieces[7]; //BishopB
    board[0][6].pieceOccupied = &pieces[5]; //KnightB
    board[0][7].pieceOccupied = &pieces[3]; //RookB

    for(int i = 0; i < 8; i++) board[1][i].pieceOccupied = &pieces[i + 16]; //8 times PawnB

    board[7][0].pieceOccupied = &pieces[10];
    board[7][1].pieceOccupied = &pieces[12];
    board[7][2].pieceOccupied = &pieces[14];
    board[7][3].pieceOccupied = &pieces[8];
    board[7][4].pieceOccupied = &pieces[9];
    board[7][5].pieceOccupied = &pieces[15];
    board[7][6].pieceOccupied = &pieces[13];
    board[7][7].pieceOccupied = &pieces[11];

    for(int i = 0; i < 8; i++) board[6][i].pieceOccupied= &pieces[i + 24];
}

void ChessBoard::display() {
    for(int row = 0; row < 8; row++){
        cout << "\033[31m" << abs(row - 8) << "\033[0m";
        for(int col = 0; col < 8; col++) {
            Cell square = board[row][col];
            if(square.pieceOccupied == nullptr) cout << "|.|";
            else if(square.pieceOccupied != nullptr && square.pieceOccupied->color == 1) cout << "|\033[30m" << square.pieceOccupied->type << "\033[0m|";
            else if(square.pieceOccupied != nullptr && square.pieceOccupied->color == 0) cout << "|" << square.pieceOccupied->type << "|";
        }
        cout << endl;
    }
    
    cout << "\033[31m  A  B  C  D  E  F  G  H  \033[0m" << endl;
    //cout << "\033[31mThis text is red.\033[0m";
}

int ChessBoard::stringToMove(string &move) {
    //examples: e4, Nf3, Rce3, R6c4
    int output = 0;

    if(move.length() == 2) {
        // pawn move
        if(move[0] == 'a' || move[0] == 'A') output += 10;
        else if(move[0] == 'b' || move[0] == 'B') output += 20;
        else if(move[0] == 'c' || move[0] == 'C') output += 30;
        else if(move[0] == 'd' || move[0] == 'D') output += 40;
        else if(move[0] == 'e' || move[0] == 'E') output += 50;
        else if(move[0] == 'f' || move[0] == 'F') output += 60;
        else if(move[0] == 'g' || move[0] == 'G') output += 70;
        else if(move[0] == 'h' || move[0] == 'H') output += 80;
        
        output += (move[1] - '0');

        //cout << ", done: output = " << output << endl;

        return output;
    }
    else if(move.length() == 3) {
        // regular move
        if(move[1] == 'a' || move[1] == 'A') output += 10;
        else if(move[1] == 'b' || move[1] == 'B') output += 20;
        else if(move[1] == 'c' || move[1] == 'C') output += 30;
        else if(move[1] == 'd' || move[1] == 'D') output += 40;
        else if(move[1] == 'e' || move[1] == 'E') output += 50;
        else if(move[1] == 'f' || move[1] == 'F') output += 60;
        else if(move[1] == 'g' || move[1] == 'G') output += 70;
        else if(move[1] == 'h' || move[1] == 'H') output += 80;
        
        output += (move[2] - '0');
        
        return output;

    }
    else if(move.length() == 4) {
        //regular move with extra stuff

        if(move[2] == 'a' || move[2] == 'A') output += 10;
        else if(move[2] == 'b' || move[2] == 'B') output += 20;
        else if(move[2] == 'c' || move[2] == 'C') output += 30;
        else if(move[2] == 'd' || move[2] == 'D') output += 40;
        else if(move[2] == 'e' || move[2] == 'E') output += 50;
        else if(move[2] == 'f' || move[2] == 'F') output += 60;
        else if(move[2] == 'g' || move[2] == 'G') output += 70;
        else if(move[2] == 'h' || move[2] == 'H') output += 80;
        
        output += (move[3] - '0');
        
        return output;
    }

    return output;
}

void ChessBoard::move(Cell &currentCell, int row, int col) {
    // row & col is where you want to move to

    //board[row][col].pieceOccupied = currentCell.pieceOccupied;
    //currentCell.pieceOccupied = nullptr;

    int dRow = abs(currentCell.row - row);
    int dCol = abs(currentCell.col - col);
    
    //conditional to see if the cell is occupied with a piece
    if(board[row][col].pieceOccupied == nullptr) {
        goto start;
        //board[row][col].pieceOccupied = currentCell.pieceOccupied;
        //currentCell.pieceOccupied = nullptr;
    }
    //cell is occupied, not the king and black
    else if(board[row][col].pieceOccupied->type != 'K') {
        // Queen movement
        start:
        if(currentCell.pieceOccupied->type == 'Q') {
            // moving along rows
            if(dRow == 0) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
            // moving along files
            else if(dCol == 0) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
            // diagonal movement
            else if(dRow == dCol) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
        }
        // Rook movement
        else if(currentCell.pieceOccupied->type == 'R') {
            // moving along rows
            if(dRow == 0) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
            // moving along files
            else if(dCol == 0) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
        }
        // Bishop movement
        else if(currentCell.pieceOccupied->type == 'B') {
            // diagonal movement
            if(dRow == dCol) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
        }
        // Knight movement
        else if(currentCell.pieceOccupied->type == 'N') {
            if((dRow == 1 && dCol == 2) || (dRow == 2 && dCol == 1)) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
        }
        // Pawn movement
        else if(currentCell.pieceOccupied->type == 'P') {
            if(currentCell.row - row <= 2) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
        }   
        // King movement
        else if(currentCell.pieceOccupied->type == 'K') {
            if(dRow <= 1 && dCol <= 1) {
                board[row][col].pieceOccupied = currentCell.pieceOccupied;
                currentCell.pieceOccupied = nullptr;
            }
        }
    }
}

void ChessBoard::play() {
    (*this).welcome();
    (*this).display();
    
    char input;
    string moveInput, turn;
    int moveInt, toRank, toFile, fromRank, fromFile;
    int turnCounter = 0;
    bool gameStatus = true;
    while(gameStatus) {

        if(turnCounter % 2 == 0) turn = "Black";
        else turn = "White";

        cout << turn << ", what would you like to do? ";
        cin >> input;

        switch(input) {
            case 'q':
                cout << "Thank you for playing!";
                exit(EXIT_SUCCESS);
            case 'c': (*this).welcome();
            case 'm':
                //ask which piece to move
                cout << "Enter the cell of the piece you'd like to move: ";
                cin >> moveInput;

                moveInt = stringToMove(moveInput);
                fromRank = 8 - (moveInt % 10);
                fromFile = (moveInt / 10) - 1;

                cout << "Enter your move in chess notation: ";
                cin >> moveInput;
                
                //code to check if the string is valid

                //convert string to move
                moveInt = stringToMove(moveInput);
                toRank = 8 - (moveInt % 10);
                toFile = (moveInt / 10) - 1;
                move(board[fromRank][fromFile], toRank, toFile);
                (*this).display();
                break;
        }
        turnCounter++;
    }

}

void ChessBoard::checkBoard() {
    // unable to implement due to time restrictions
}

void ChessBoard::welcome() {
    cout << "Welcome to Chess! You will play with the white pieces. White's pieces are lowercase, and black pieces are uppercase." << endl;
    cout << "Here are the commands: " << endl;
    cout << "- Press 'q' to quit!" << endl;
    cout << "- Press 'm' to move a piece, and you will be prompted for where you'd like to move your piece on the board." << endl;
    cout << "- Enter 'c' to print this list of commands. We hope you enjoy this chess experience!" << endl;
}
