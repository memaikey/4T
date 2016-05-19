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
    enum Side {EMPTY, HUMAN, COMPUTER};
    Side side(const int row, const int col) const;
private:
    matrix<Side, 3, 3> board;
};

class Console
{
public:
    void Play();
    void PrintSmall(const int bigRow,const int bigCol) const;
private:
    SmallBoard smallboard;
    char humanSymbol = 'o';
    char computerSymbol = 'x';
};

SmallBoard::SmallBoard()
{
    fill(board.begin(), board.end(), EMPTY);
}

SmallBoard::Side SmallBoard::side(const int row, const int col) const
{
    return board(row,col);
}

void Console::PrintSmall(const int bigRow,const int bigCol) const
{
for (int r = 0; r < 3; ++r){
        for (int c = 0; c < 3; ++c){
            cout << smallboard.side(r,c);
        }
        cout << "\n";
    }
}

int main(){
    cout << "Hello!\n";
    Console console;
    console.PrintSmall(0,0);
    return 0;
}
