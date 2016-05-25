/**
* Tactical Tic-Tac-Toe (4T)
*
* Rules:
* TODO
*
* Coding conventions:
* TODO
*
* Naming conventions:
* TTT = TicTacToe
* bigBoard = 3x3 games of TTT
* field = normal TTT game
* spot = place in normal TTT game
* There is 1 bigBoard, with 9 TTT games, with 729 spots in total
*
* Possible improvements:
* GUI
* colored TUI
* AB pruning
* analysis
**/
#warning write rules in preamble
#warning write rules in game
#warning write coding conventions
#warning expand naming conventions
#warning add debug IFDEFs
#warning change field to row/col, -1 is free move
#warning TODO: add time to analysis and expand total moves calculation

#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
#include "HHS_examples/Matrix.h"

#include <cassert>
//#define NDEBUG
enum Side {HUMAN, DRAW, COMPUTER, EMPTY};
typedef matrix<Side, 3, 3> Board;

// print number of moves considered and time used
#define ANALYSE
#ifdef ANALYSE
#ifdef WIN32
    #include <windows.h>
#else
    #include <ctime>
#endif // WIN32
//print AI move calculation progress
#define SHOWMOVESPROGRESS
#endif // ANALYSE

#define DEBUG
//#define TEST_AI_1

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
    Board board;
    Side fieldWonBy;
    const char humanSymbol = 'x';
    const char computerSymbol = 'o';
    const char emptySymbol = ' ';
};

class Game
{
public:
    typedef matrix<SmallBoard, 3, 3> BigBoard;
    Game(const bool playerFirst);
    void PrintBoard() const;
    bool Play();
    void GetPlayerMove(const string s);
#ifdef TEST_AI_1
    Side ChooseComputerMoveTop();
    Side ChooseHumanMove();
#else
    void GetComputerMove();
    Side ChooseComputerMoveTop(int& bestRow, int& bestCol);
    Side ChooseComputerMove(BigBoard& board, const int nextRow, const int nextCol);
    Side ChooseHumanMove(BigBoard& board, const int nextRow, const int nextCol);
#endif // TEST_AI_1
    Side CheckGameEnd() const;
    Side CheckGameEnd(BigBoard& board) const;
#ifdef DEBUG
    void PrintFieldStatus() const;
#endif // DEBUG
private:
    bool playerFirst;
    BigBoard bigBoard;
    int nextFieldRow;
    int nextFieldCol;
    bool playerTurn;
    int playRow;
    int playCol;
#ifdef ANALYSE
    int movesConsidered;
#ifdef SHOWMOVESPROGRESS
    void ShowMovesConsideredProgress();
#endif // SHOWMOVESPROGRESS
#endif // ANALYSE

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

bool SmallBoard::IsFieldWon(const Side side) const
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
    nextFieldRow = -1;
    nextFieldCol = -1;
    playerTurn = playerFirst;
    playRow = 0;
    playCol = 0;
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
        for (int col = 0; col < 9; ++col) {
            if (col == 0){
                cout << row << '|';
            }
            if ((col == 3) || (col == 6)) {
                cout << '|';
            }
            bigBoard(row/3, col/3).PrintSmall(row%3, col%3);
        }
        cout << endl;
    }
    cout << endl;
}

bool Game::Play()
{
    Side side;
#ifdef TEST_AI_1
    if (playerTurn) {
        side = HUMAN;
        ChooseHumanMove();
        cout << "P move: <" << playRow << "," << playCol << ">\n";
    }
    else {
        side = COMPUTER;
        ChooseComputerMoveTop();
        cout << "C move: <" << playRow << "," << playCol << ">\n";
    }
#else
    if (playerTurn) {
        side = HUMAN;
        string s = "Player, ";
        if (nextFieldRow == -1) {
            s += "play in any field. Enter the row and column:\n";
        }
        else {
            char fieldName = 'A' + nextFieldRow*3 + nextFieldCol;
            s += "play in field ";
            s += fieldName;
            s += ". Enter the row and column:\n";
        }
        GetPlayerMove(s);
    }
    else {
        side = COMPUTER;
        ///GetComputerMove();
        #warning GetComputerMove() crashes, replaced by second PlayerMove below
        string s = "Computer, ";
        if (nextFieldRow == -1) {
            s += "play in any field. Enter the row and column:\n";
        }
        else {
            char fieldName = 'A'  + nextFieldRow*3 + nextFieldCol;
            s += "play in field ";
            s += fieldName;
            s += ". Enter the row and column:\n";
        }
        GetPlayerMove(s);
    }
#endif // TEST_AI_1
    //Play move, check fieldStatus
    if (bigBoard(playRow/3, playCol/3).DoMove(playRow%3, playCol%3, side) != EMPTY) {
        //check for win if field is full, stop if board is won or full
        if (CheckGameEnd() != EMPTY) return 0;
    }
#ifdef DEBUG
    PrintFieldStatus();
#endif // DEBUG
    //change active player
    playerTurn = !playerTurn;
    //Restrict next move
    nextFieldRow = playRow%3;
    nextFieldCol = playCol%3;
    //Restriction invalid, free move
    if (bigBoard(nextFieldRow, nextFieldCol).FieldStatus() != EMPTY) {
        nextFieldRow = -1;
        nextFieldCol = -1;
    }
    PrintBoard();
#ifdef DEBUG
    cout << "nextField: " << nextFieldRow << "," << nextFieldCol << ". Press enter" << endl;
    cin.get();
#endif // DEBUG
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
        else if ((nextFieldRow > -1) && ((tempRow/3 != nextFieldRow) || (tempCol/3 != nextFieldCol))){
            char playField = 'A' + nextFieldRow*3 + nextFieldCol;
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
    playRow = tempRow;
    playCol = tempCol;
}

#ifndef TEST_AI_1
void Game::GetComputerMove()
{
    int bestRow, bestCol;
    //calculate best move
    ChooseComputerMoveTop(bestRow, bestCol);
    //Check move validity
    //Move out of bounds
    assert((bestCol > 8) || (bestCol < 0) || (bestRow > 8) || (bestRow < 0));
    //Forced move, wrong field
    assert((nextFieldRow != -1) && ((bestRow/3 != nextFieldRow) || (bestCol/3 != nextFieldCol)));
    //Field already won or full
    assert(bigBoard(bestRow/3, bestCol/3).FieldStatus() != EMPTY);
    //Move already taken
    assert(bigBoard(bestRow/3, bestCol/3).SpotStatus(bestRow%3, bestCol%3) != EMPTY);
    //insert move in game data
    playRow = bestRow;
    playCol = bestCol;
}
#endif // TEST_AI_1

#ifdef TEST_AI_1
Side Game::ChooseComputerMoveTop()
{
#ifdef ANALYSE
    movesConsidered = 1;
#ifdef SHOWMOVESPROGRESS
    ShowMovesConsideredProgress();
#endif // SHOWMOVESPROGRESS
#endif // ANALYSE
    //Field restriction, just check one field
    if (nextFieldRow > -1){
        //only check rows and cols of active field
        for (int row = nextFieldRow*3; row < nextFieldRow*3 + 3; ++row) {
            for (int col = nextFieldCol*3; col < nextFieldCol*3 + 3; ++col) {
                //only check empty spots
                if (bigBoard(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY){
                    //do move, ignore return value of field
                    bigBoard(row/3, col/3).DoMove(row%3, col%3, COMPUTER);
                    playRow = row;
                    playCol = col;
                    return COMPUTER;
                }
            }
        }
    }
    //No field restriction, check entire board
    else {
        //only check rows and cols of active field
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                //only check empty spots
                if ((bigBoard(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY) &&
                    (bigBoard(row/3, col/3).FieldStatus() == EMPTY)){
                    playRow = row;
                    playCol = col;
                    return COMPUTER;
                }
            }
        }
    }
    return EMPTY;
}
#else
Side Game::ChooseComputerMoveTop(int& bestRow, int& bestCol)
{
#ifdef ANALYSE
    movesConsidered = 1;
#ifdef SHOWMOVESPROGRESS
    ShowMovesConsideredProgress();
#endif // SHOWMOVESPROGRESS
#endif // ANALYSE
    Side bestOutcome = CheckGameEnd(bigBoard);
    //Game not finished?
    if (bestOutcome == EMPTY){
        //Field restriction, just check one field
        if (nextFieldRow > -1){
            //only check rows and cols of active field
            for (int row = nextFieldRow*3; row < nextFieldRow*3 + 3; ++row) {
                for (int col = nextFieldCol*3; col < nextFieldCol*3 + 3; ++col) {
                    //only check empty spots
                    if (bigBoard(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY){
                        //copy board for every new move
                        BigBoard tempBoard = bigBoard;
                        //do move, ignore return value of field
                        tempBoard(row/3, col/3).DoMove(row%3, col%3, COMPUTER);
                        //prepare nextRow, nextCol
                        int tempNextRow = row%3;
                        int tempNextCol = col%3;
                        if (tempBoard(tempNextRow, tempNextCol).FieldStatus() != EMPTY) {
                            tempNextRow = -1;
                            tempNextCol = -1;
                        }
                        //call other player
                        Side outcome = ChooseHumanMove(tempBoard, tempNextRow, tempNextCol);
                        //check if move is better or equal
                        if (outcome >= bestOutcome) {
                            bestOutcome = outcome;
                        }
                    }
                }
            }
        }
        //No field restriction, check entire board
        else {
            //only check rows and cols of active field
            for (int row = 0; row < 9; ++row) {
                for (int col = 0; col < 9; ++col) {
                    //only check valid fields and empty spots
                    if ((bigBoard(row/3, col/3).FieldStatus() == EMPTY) &&
                        (bigBoard(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY)){
                        //copy board for every new move
                        BigBoard tempBoard = bigBoard;
                        //do move, ignore return value of field
                        tempBoard(row/3, col/3).DoMove(row%3, col%3, COMPUTER);
                        //prepare nextRow, nextCol
                        int tempNextRow = row%3;
                        int tempNextCol = col%3;
                        if (tempBoard(tempNextRow, tempNextCol).FieldStatus() != EMPTY) {
                            tempNextRow = -1;
                            tempNextCol = -1;
                        }
                        //call other player
                        Side outcome = ChooseHumanMove(tempBoard, tempNextRow, tempNextCol);
                        //check if move is better or equal
                        if (outcome >= bestOutcome) {
                            bestOutcome = outcome;
                        }
                    }
                }
            }
        }
    }
    return bestOutcome;
}
#endif // TEST_AI_1

#ifndef TEST_AI_1
Side Game::ChooseComputerMove(BigBoard& board, const int nextRow, const int nextCol)
{
#ifdef ANALYSE
    ++movesConsidered;
#ifdef SHOWMOVESPROGRESS
    ShowMovesConsideredProgress();
#endif // SHOWMOVESPROGRESS
#endif // ANALYSE
    Side bestOutcome = CheckGameEnd(board);
    //Game not finished?
    if (bestOutcome == EMPTY){
        //Field restriction, just check one field
        if (nextRow > -1){
            //only check rows and cols of active field
            for (int row = nextRow*3; row < nextRow+3; ++row) {
                for (int col = nextCol*3; col < nextCol+3; ++col) {
                    //only check empty spots
                    if (board(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY){
                        //copy board for every new move
                        BigBoard tempBoard = board;
                        //do move, ignore return value of field
                        tempBoard(row/3, col/3).DoMove(row%3, col%3, COMPUTER);
                        //prepare nextRow, nextCol
                        int tempNextRow = row%3*3;
                        int tempNextCol = col%3*3;
                        if (tempBoard((tempNextRow/3), (tempNextCol/3)).FieldStatus() != EMPTY) {
                            tempNextRow = -1;
                            tempNextCol = -1;
                        }
                        //call other player
                        Side outcome = ChooseHumanMove(tempBoard, tempNextRow, tempNextCol);
                        //check if move is better or equal
                        if (outcome >= bestOutcome) {
                            bestOutcome = outcome;
                        }
                    }
                }
            }
        }
        //No field restriction, check entire board
        else {
            //only check rows and cols of active field
            for (int row = 0; row < 9; ++row) {
                for (int col = 0; col < 9; ++col) {
                    //only check valid fields and empty spots
                    if ((board(row/3, col/3).FieldStatus() == EMPTY) &&
                        (board(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY)){
                        //copy board for every new move
                        BigBoard tempBoard = board;
                        //do move, ignore return value of field
                        tempBoard(row/3, col/3).DoMove(row%3, col%3, COMPUTER);
                        //prepare nextRow, nextCol
                        int tempNextRow = row%3*3;
                        int tempNextCol = col%3*3;
                        if (tempBoard((tempNextRow/3), (tempNextCol/3)).FieldStatus() != EMPTY) {
                            tempNextRow = -1;
                            tempNextCol = -1;
                        }
                        //call other player
                        Side outcome = ChooseHumanMove(tempBoard, tempNextRow, tempNextCol);
                        //check if move is better or equal
                        if (outcome >= bestOutcome) {
                            bestOutcome = outcome;
                        }
                    }
                }
            }
        }
    }
    return bestOutcome;
}
#endif // TEST_AI_1

#ifdef TEST_AI_1
Side Game::ChooseHumanMove()
{
#ifdef ANALYSE
    ++movesConsidered;
#ifdef SHOWMOVESPROGRESS
    ShowMovesConsideredProgress();
#endif // SHOWMOVESPROGRESS
#endif // ANALYSE
    //Field restriction, just check one field
    if (nextFieldRow > -1){
        //only check rows and cols of active field
        for (int row = nextFieldRow*3; row < nextFieldRow*3 + 3; ++row) {
            for (int col = nextFieldCol*3; col < nextFieldCol*3 + 3; ++col) {
                //only check empty spots
                if (bigBoard(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY){
                    playRow = row;
                    playCol = col;
                    return HUMAN;
                }
            }
        }
    }
    //No field restriction, check entire board
    else {
        //only check rows and cols of active field
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                //only check empty spots
                if ((bigBoard(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY) &&
                    (bigBoard(row/3, col/3).FieldStatus() == EMPTY)){
                    playRow = row;
                    playCol = col;
                    return HUMAN;
                }
            }
        }
    }
    return DRAW;
}
#else
Side Game::ChooseHumanMove(BigBoard& board, const int nextRow, const int nextCol)
{
#ifdef ANALYSE
    ++movesConsidered;
#ifdef SHOWMOVESPROGRESS
    ShowMovesConsideredProgress();
#endif // SHOWMOVESPROGRESS
#endif // ANALYSE
    Side bestOutcome = CheckGameEnd(board);
    //Game not finished?
    if (bestOutcome == EMPTY){
        //Field restriction, just check one field
        if (nextRow > -1){
            //only check rows and cols of active field
            for (int row = nextRow*3; row < nextRow+3; ++row) {
                for (int col = nextCol*3; col < nextCol+3; ++col) {
                    //only check empty spots
                    if (board(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY){
                        //copy board for every new move
                        BigBoard tempBoard = board;
                        //do move, ignore return value of field
                        tempBoard(row/3, col/3).DoMove(row%3, col%3, HUMAN);
                        //prepare nextRow, nextCol
                        int tempNextRow = row%3*3;
                        int tempNextCol = col%3*3;
                        if (tempBoard((tempNextRow/3), (tempNextCol/3)).FieldStatus() != EMPTY) {
                            tempNextRow = -1;
                            tempNextCol = -1;
                        }
                        //call other player
                        Side outcome = ChooseComputerMove(tempBoard, tempNextRow, tempNextCol);
                        //check if move is better or equal
                        if (outcome <= bestOutcome) {
                            bestOutcome = outcome;
                        }
                    }
                }
            }
        }
        //No field restriction, check entire board
        else {
            //only check rows and cols of active field
            for (int row = 0; row < 9; ++row) {
                for (int col = 0; col < 9; ++col) {
                    //only check valid fields and empty spots
                    if ((board(row/3, col/3).FieldStatus() == EMPTY) &&
                        (board(row/3, col/3).SpotStatus(row%3, col%3) == EMPTY)){
                        //copy board for every new move
                        BigBoard tempBoard = board;
                        //do move, ignore return value of field
                        tempBoard(row/3, col/3).DoMove(row%3, col%3, HUMAN);
                        //prepare nextRow, nextCol
                        int tempNextRow = row%3*3;
                        int tempNextCol = col%3*3;
                        if (tempBoard((tempNextRow/3), (tempNextCol/3)).FieldStatus() != EMPTY) {
                            tempNextRow = -1;
                            tempNextCol = -1;
                        }
                        //call other player
                        Side outcome = ChooseComputerMove(tempBoard, tempNextRow, tempNextCol);
                        //check if move is better or equal
                        if (outcome <= bestOutcome) {
                            bestOutcome = outcome;
                        }
                    }
                }
            }
        }
    }
    return bestOutcome;
}
#endif // TEST_AI_1

Side Game::CheckGameEnd() const
{
    //No current winner
    Side winner = EMPTY;
    //Did HUMAN win?
    if ((bigBoard(0, 0).FieldStatus() == HUMAN && bigBoard(1, 1).FieldStatus() == HUMAN && bigBoard(2, 2).FieldStatus() == HUMAN) ||
        (bigBoard(0, 2).FieldStatus() == HUMAN && bigBoard(1, 1).FieldStatus() == HUMAN && bigBoard(2, 0).FieldStatus() == HUMAN)){
        winner = HUMAN;
    }
    else {
        for (int i = 0; i < 3; ++i) {
            if ((bigBoard(i, 0).FieldStatus() == HUMAN && bigBoard(i, 1).FieldStatus() == HUMAN && bigBoard(i, 2).FieldStatus() == HUMAN) ||
                (bigBoard(0, i).FieldStatus() == HUMAN && bigBoard(1, i).FieldStatus() == HUMAN && bigBoard(2, i).FieldStatus() == HUMAN)) {
                winner = HUMAN;
            }
        }
    }
    //HUMAN did not win, did COMPUTER win?
    if (winner == EMPTY) {
		if ((bigBoard(0, 0).FieldStatus() == COMPUTER && bigBoard(1, 1).FieldStatus() == COMPUTER && bigBoard(2, 2).FieldStatus() == COMPUTER) ||
			(bigBoard(0, 2).FieldStatus() == COMPUTER && bigBoard(1, 1).FieldStatus() == COMPUTER && bigBoard(2, 0).FieldStatus() == COMPUTER)){
			winner = COMPUTER;
		}
		else {
			for (int i = 0; i < 3; ++i) {
				if ((bigBoard(i, 0).FieldStatus() == COMPUTER && bigBoard(i, 1).FieldStatus() == COMPUTER && bigBoard(i, 2).FieldStatus() == COMPUTER) ||
					(bigBoard(0, i).FieldStatus() == COMPUTER && bigBoard(1, i).FieldStatus() == COMPUTER && bigBoard(2, i).FieldStatus() == COMPUTER)) {
					winner = COMPUTER;
				}
			}
		}
    }
    //Both did not win, draw?
    if (winner == EMPTY) {
        if (none_of(bigBoard.cbegin(), bigBoard.cend(), [](SmallBoard s){return s.FieldStatus() == EMPTY;})) {
            winner = DRAW;

        }
    }
    //Cout game status if ended
    //Return game status
    switch (winner) {
        case HUMAN:
            PrintBoard();
            cout << "Player won! Game over.\n";
            return winner;
        case COMPUTER:
            PrintBoard();
            cout << "Computer won! Game over.\n";
            return winner;
        case DRAW:
            PrintBoard();
            cout << "Draw! Game over.\n";
            return winner;
        //at least one field EMPTY
        default:
            return winner;
    }
}

Side Game::CheckGameEnd(BigBoard& board) const
{
    //Did HUMAN win?
    if ((board(0, 0).FieldStatus() == HUMAN && board(1, 1).FieldStatus() == HUMAN && board(2, 2).FieldStatus() == HUMAN) ||
        (board(0, 2).FieldStatus() == HUMAN && board(1, 1).FieldStatus() == HUMAN && board(2, 0).FieldStatus() == HUMAN)){
        return HUMAN;
    }
    for (int i = 0; i < 3; ++i) {
        if ((board(i, 0).FieldStatus() == HUMAN && board(i, 1).FieldStatus() == HUMAN && board(i, 2).FieldStatus() == HUMAN) ||
            (board(0, i).FieldStatus() == HUMAN && board(1, i).FieldStatus() == HUMAN && board(2, i).FieldStatus() == HUMAN)) {
            return HUMAN;
        }
    }
    //HUMAN did not win, did COMPUTER win?
    if ((board(0, 0).FieldStatus() == COMPUTER && board(1, 1).FieldStatus() == COMPUTER && board(2, 2).FieldStatus() == COMPUTER) ||
        (board(0, 2).FieldStatus() == COMPUTER && board(1, 1).FieldStatus() == COMPUTER && board(2, 0).FieldStatus() == COMPUTER)){
        return COMPUTER;
    }
    for (int i = 0; i < 3; ++i) {
        if ((board(i, 0).FieldStatus() == COMPUTER && board(i, 1).FieldStatus() == COMPUTER && board(i, 2).FieldStatus() == COMPUTER) ||
            (board(0, i).FieldStatus() == COMPUTER && board(1, i).FieldStatus() == COMPUTER && board(2, i).FieldStatus() == COMPUTER)) {
            return COMPUTER;
        }
    }
    //Both did not win, draw?
    if (none_of(board.cbegin(), board.cend(), [](SmallBoard s){return s.FieldStatus() == EMPTY;})) {
        return DRAW;
    }
    //No win or draw -> still playing
    return EMPTY;
}

#ifdef SHOWMOVESPROGRESS
void Game::ShowMovesConsideredProgress()
{
    if ((movesConsidered % 100000) == 0){
        if (movesConsidered >= 1000000) {
            if ((movesConsidered % 1000000) == 0) {
                cout << "DEBUG, moves considered: ";
                cout << movesConsidered / pow(10,6) << "M" << endl;
            }
        }
        else {
            cout << "DEBUG, moves considered: ";
            cout << "0."<< movesConsidered / 100000 << "K" << endl;
        }
    }
}
#endif // SHOWMOVESPROGRESS

#ifdef DEBUG
void Game::PrintFieldStatus() const
{
    Side status;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            status = bigBoard(row, col).FieldStatus();
            switch (status) {
                case HUMAN: cout << 'H'; break;
                case COMPUTER: cout << 'C'; break;
                case DRAW: cout << 'D'; break;
                case EMPTY: cout << '_'; break;
                default: assert(false);
            }
        }
        cout << endl;
    }
}
#endif // DEBUG

int main(){
    cout << "Hello!\n";
    bool playerFirst = true;
    bool playing = true;
    Game game(playerFirst);
    do {
        playing = game.Play();
    } while (playing);
    cout << "\nBye!\n";
    cin.get();
    return 0;
}


