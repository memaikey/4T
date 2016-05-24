//matrix test
//use PROCENTEC style guide

#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
#include "HHS_examples/Matrix.h"

#include <cassert>
//#define NDEBUG
enum Side {EMPTY, HUMAN, COMPUTER, DRAW};

#warning add debug IFDEFs
#warning add documentation

class SmallBoard
{
public:
    SmallBoard();
    void PrintSmall(const int row, const int col) const;
    Side FieldStatus() const;
    Side SpotStatus(const int row, const int col) const;
    Side DoMove(const int row, const int col, const Side side);
    bool IsFieldWon(const Side side) const;
private:
    matrix<Side, 3, 3> board;
    Side fieldWonBy;
    const char humanSymbol = 'x';
    const char computerSymbol = 'o';
    const char emptySymbol = ' ';
};

class Game
{
public:
    Game(const bool playerFirst);
    void PrintBoard() const;
    Side BoardStatus() const;
    bool Play();
    void GetPlayerMove(const string s);
    void GetComputerMove();
    Side ChooseComputerMove(int& bestRow, int& bestCol);
    Side ChooseHumanMove(int& bestRow, int& bestCol);
    bool CheckGameEnd(const Side side) const;
private:
    bool playerFirst;
    matrix<SmallBoard, 3, 3> bigBoard;
    int nextMove;
    bool playerTurn;
    int row;
    int col;
};

SmallBoard::SmallBoard()
{
    fill(board.begin(), board.end(), EMPTY);
    fieldWonBy = EMPTY;
}

void SmallBoard::PrintSmall(const int row, const int col) const
{
    if ((fieldWonBy == EMPTY) || (fieldWonBy == DRAW)) {
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
        if (fieldWonBy == HUMAN) cout << humanSymbol;
        else if (fieldWonBy == COMPUTER) cout << computerSymbol;
    }
}

Side SmallBoard::FieldStatus() const
{
    return fieldWonBy;
}

Side SmallBoard::SpotStatus(const int row, const int col) const
{
    return board(row, col);
}

Side SmallBoard::DoMove(const int row, const int col, const Side side)
{
    board(row, col) = side;
    if (IsFieldWon(side)) {
        fieldWonBy = side;
    }
    else if (none_of(board.cbegin(), board.cend(), [](Side s){return s == EMPTY;})) {
        fieldWonBy = DRAW;
    }
    return fieldWonBy;
}

bool IsFieldWon(const Side side) const
{
    for (int i = 0; i < 3; ++i) {
        if ((board(i, 0) == side && board(i, 1) == side && board(i, 2) == side) ||
            (board(0, i) == side && board(1, i) == side && board(2, i) == side)) {
            return true;
        }
    }
    return ((board(0, 0) == side && board(1, 1) == side && board(2, 2) == side) ||
            (board(0, 2) == side && board(1, 1) == side && board(2, 0) == side));
}

Game::Game(const bool playerFirst)
{
    nextMove = 0;
    playerTurn = playerFirst;
    row = 0;
    col = 0;
}

void Game::PrintBoard() const
{
    cout << endl
         << " |012|345|678\n"
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

bool Game::Play()
{
    Side side;
    if (playerTurn) {
        string s;
        side = HUMAN;
        s = "Player, ";
        if (nextMove == 0) {
            s += "play in any field! Enter the row and column:\n";
        }
        else {
            char fieldName = 'A' - 1 + nextMove;
            s += "play in field ";
            s += fieldName;
            s += ". Enter the row and column:\n";
        }
        GetPlayerMove(s);
    }
    else {
        side = COMPUTER;
        GetComputerMove();
    }

    //Play move
    bigBoard(row/3, col/3).DoMove(row%3, col%3, side);
    playerTurn = !playerTurn;

    //Restrict next move
    nextMove = (row%3*3 + col%3 + 1);
    //Restriction invalid, free move
    if (BoardStatus() != EMPTY) {
        nextMove = 0;
    }
    return 1;
}

void Game::GetPlayerMove(const string s)
{
    int tempRow, tempCol;
    bool inputCorrect = false;
    do {
        PrintBoard();
        cout << s;
        cin >> tempRow >> tempCol;
        //Move out of bounds
        if ((tempCol > 8) || (tempCol < 0) || (tempRow > 8) || (tempRow < 0)) {
            cout << "\nMove out of bounds, choose a value from 0 to 8 for row and column.\n\n";
        }
        //Forced move, wrong field
        else if ((nextMove != 0) && ((tempRow/3*3 + tempCol/3 + 1) != nextMove)) {
            char playField = 'A' - 1 + nextMove;
            cout << "\nWrong field, play in field " << playField << ".\n\n";
        }
        //Field already won or full
        else if (bigBoard(tempRow/3, tempCol/3).FieldStatus() != EMPTY) {
            cout << "\nField already won, choose a different field.\n\n";
        }
        //Spot taken
        else if (bigBoard(tempRow/3, tempCol/3).SpotStatus(tempRow%3, tempCol%3) != EMPTY) {
            cout << "\nSpot already taken, choose a different spot.\n\n";
        }
        else {
            inputCorrect = true;
        }
    } while (!inputCorrect);
    //Save valid move
    row = tempRow;
    col = tempCol;
}

void Game::GetComputerMove()
{
    int bestRow, bestCol;
    chooseComputerMove(bestRow, bestCol);

    //Check move validity
    //Move out of bounds
    assert((bestCol > 8) || (bestCol < 0) || (bestRow > 8) || (bestRow < 0));
    //Forced move, wrong field
    assert((nextMove != 0) && ((bestRow/3*3 + bestCol/3 + 1) != nextMove));
    //Field already won or full
    assert(bigBoard(bestRow/3, bestCol/3).FieldStatus() != EMPTY);
    //Move already taken
    assert(bigBoard(bestRow/3, bestCol/3).SpotStatus(bestRow%3, bestCol%3) != EMPTY);

    row = bestRow;
    col = bestCol;
}

Side ChooseComputerMove(int& bestRow, int& bestCol);
{
    Side bestValue =
}

Side ChooseComputerMove(int& bestRow, int& bestCol);
{

}

bool Game::CheckGameEnd(const Side side) const
{
    Side winner = EMPTY;
    if ((bigBoard(0, 0).FieldStatus() == side && bigBoard(1, 1).FieldStatus() == side && bigBoard(2, 2).FieldStatus() == side) ||
        (bigBoard(0, 2).FieldStatus() == side && bigBoard(1, 1).FieldStatus() == side && bigBoard(2, 0).FieldStatus() == side)){
        winner = side;
    }
    else {
        for (int i = 0; i < 3; ++i) {
            if ((bigBoard(i, 0).FieldStatus() == side && bigBoard(i, 1).FieldStatus() == side && bigBoard(i, 2).FieldStatus() == side) ||
                (bigBoard(0, i).FieldStatus() == side && bigBoard(1, i).FieldStatus() == side && bigBoard(2, i).FieldStatus() == side)) {
                winner = side;
            }
        }
    }
    if (winner == side) {
        if (side == HUMAN) {
            cout << "Player won! Game over.\n";
        }
        else {
            cout << "Computer won! Game over.\n";
        }
        return 1;
    }
    if (none_of(bigBoard.cbegin(), bigBoard.cend(), [](SmallBoard s){return s.FieldStatus() == EMPTY;})) {
        cout << "Draw! Game over.\n";
        return 1;
    }
    return 0;
}

int main(){
    cout << "Hello!\n";
    bool playerFirst = true;
    bool playing = true;
    Game game(playerFirst);
    do {
        playing = game.Play();
    } while (playing);
    cout << "\nBye!\n";
    return 0;
}
