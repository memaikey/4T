//matrix test
//use PROCENTEC style guide

#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
#include "HHS_examples/Matrix.h"



class SmallBoard
{
public:
    SmallBoard();
    void PrintSmall(const int row, const int col) const;
private:
    enum Side {EMPTY, HUMAN, COMPUTER};
    matrix<Side, 3, 3> board;
    Side side;
    const char humanSymbol = 'o';
    const char computerSymbol = 'x';
    const char emptySymbol = ' ';
    void PrintNormal(const int row, const int col) const;
    void PrintWon() const;
};

class Console
{
public:
    void PrintBoard() const;
private:
    matrix<SmallBoard, 3, 3> bigBoard;
};

SmallBoard::SmallBoard()
{
    fill(board.begin(), board.end(), EMPTY);
    side = EMPTY;
}

void SmallBoard::PrintSmall(const int row, const int col) const
{
    if (side == EMPTY){
        PrintNormal(row, col);
    }
    else{
        #error continue here
        PrintWon(row, col, );
    }
}

void SmallBoard::PrintNormal(const int row, const int col) const
{
    if (board(row,col) == HUMAN){
        cout << humanSymbol;
    }
    else if (board(row,col) == COMPUTER){
        cout << computerSymbol;
    }
    else{
        cout << emptySymbol;
    }
}

void SmallBoard::PrintWon() const
{

}

void Console::PrintBoard() const
//TODO:
#warning TODO: check for smallBoard side
{
    cout << " |012|345|678\n"
         << "-+---+---+---\n";
    for (int row = 0; row < 9; ++row){
        if ((row == 3) || (row==6)){
            cout << "-+---+---+---\n";
        }
        for (int col = 0; col < 9; ++ col){
            if (col == 0){
                cout << row << '|';
            }
            if ((col == 3) || (col==6)){
                cout << '|';
            }
            bigBoard(row/3,col/3).PrintSmall(row%3,col%3);
        }
        cout << endl;
    }
}

int main(){
    cout << "Hello!\n\n";
    Console console;
    console.PrintBoard();
    cout << "\nBye!\n";
    return 0;
}
