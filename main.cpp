//matrix test
//use PROCENTEC style guide
#define debug TRUE

#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
#include "HHS_examples/Matrix.h"

#include <cassert>
//#define NDEBUG
enum Side {EMPTY, HUMAN, COMPUTER};

class SmallBoard
{
public:
    SmallBoard();
    void PrintSmall(const int row, const int col) const;
    Side FieldStatus() const;
private:
    matrix<Side, 3, 3> board;
    Side side;
    const char humanSymbol = 'o';
    const char computerSymbol = 'x';
    const char emptySymbol = ' ';
    //void PrintWon() const;
};

class Game
{
public:
    Game();
    void PrintBoard() const;
    void Move();
    Side BoardStatus() const;
private:
    matrix<SmallBoard, 3, 3> bigBoard;
    int nextMove;
};

SmallBoard::SmallBoard()
{
    fill(board.begin(), board.end(), EMPTY);
    side = EMPTY;
}

void SmallBoard::PrintSmall(const int row, const int col) const
{
    if (side == EMPTY) {
        if (board(row,col) == HUMAN) {
            cout << humanSymbol;
        }
        else if (board(row,col) == COMPUTER) {
            cout << computerSymbol;
        }
        else {
            cout << emptySymbol;
        }
    }
    else{
        if (side == HUMAN) cout << humanSymbol;
        else if (side == COMPUTER) cout << computerSymbol;
    }
}

Side SmallBoard::FieldStatus() const
{
    return side;
}

Game::Game()
{
    nextMove = 0;
}

void Game::PrintBoard() const
{
    cout << " |012|345|678\n"
         << "-A---B---C---\n";
    for (int row = 0; row < 9; ++row) {
        if (row == 3){
            cout << "-D---E---F---\n";
        }
        if (row == 6){
            cout << "-G---H---I---\n";
        }
        for (int col = 0; col < 9; ++ col) {
            if (col == 0){
                cout << row << '|';
            }
            if ((col == 3) || (col==6)) {
                cout << '|';
            }
            bigBoard(row/3,col/3).PrintSmall(row%3,col%3);
        }
        cout << endl;
    }
    cout << endl;
}

Side Game::BoardStatus() const
{
    switch (nextMove) {
        case 1: return bigBoard(0,0).FieldStatus();
        case 2: return bigBoard(0,1).FieldStatus();
        case 3: return bigBoard(0,2).FieldStatus();
        case 4: return bigBoard(1,0).FieldStatus();
        case 5: return bigBoard(1,1).FieldStatus();
        case 6: return bigBoard(1,2).FieldStatus();
        case 7: return bigBoard(2,0).FieldStatus();
        case 8: return bigBoard(2,1).FieldStatus();
        case 9: return bigBoard(2,2).FieldStatus();
        default:
            assert(false);
    }
}

void Game::Move()
{
    assert ((nextMove >= 0) && (nextMove < 10));
    if ((nextMove == 0)) {
        cout << "Play in any field! Enter the row and column:\n";
    }
    else if ((BoardStatus() == HUMAN) ||
             (BoardStatus() == COMPUTER)) {
        cout << "Play in any field! Enter the row and column:\n";
    }
    else {
        char fieldName = 'A'+nextMove-1;
        cout << "Play in field " << fieldName << ". Enter the row and column:\n";
    }
}

int main(){
    cout << "Hello!\n\n";
    Game game;
    game.PrintBoard();
    game.Move();
    cout << "\nBye!\n";
    return 0;
}
