#include <iostream>
#include <algorithm>
#include <map>
#include <string>
using namespace std;
#include "Matrix.h"

// print number of moves considered, time used and map size
#define ANALYSE
#ifdef ANALYSE
#ifdef WIN32
#include <windows.h>
#else
#include <ctime>
#endif    
#endif

// This solution only starts saves boards if DEPTH >= 3 and DEPTH <= MAX_DEPTH
// For DEPTH < 3 no transpositions can be found
// For DEPTH > MAX_DEPTH calculation is faster than lookup 

const int MAX_DEPTH = 9;

class TicTacToe {
public:
    enum Side { EMPTY, HUMAN, COMPUTER };
    enum Value { HUMAN_WINS = -1, DRAW, COMPUTER_WINS, UNDECIDED };
    TicTacToe()
#ifdef ANALYSE
        : movesConsidered(0)
#endif
    {
        fill(board.begin(), board.end(), EMPTY);
    }
    Value chooseComputerMove(int& bestRow, int& bestColumn, Value alpha = HUMAN_WINS, Value beta = COMPUTER_WINS, int depth = 1);
    Value chooseHumanMove(int& bestRow, int& bestColumn, Value alpha = HUMAN_WINS, Value beta = COMPUTER_WINS, int depth = 1);
    Side side(int row, int column) const;
    bool isUndecided() const;
    bool playMove(Side s, int row, int column);
    bool boardIsFull() const;
    bool isAWin(Side s) const;
#ifdef ANALYSE
    int getAndResetMovesConsidered() {
        int i = movesConsidered;
        movesConsidered = 0;
        return i;
    }
    int getMapSize() const {
        return boards.size();
    }
#endif
private:
    typedef matrix<Side, 3, 3> Board;
    Board board;
    Value value() const;
    class BoardWrapper {
    public:
        BoardWrapper(const Board& b): board(b) {
        }
        bool operator<(const BoardWrapper& rhs) const;
    private:
        Board board;
    };
    class ValueAndBestMove {
    public:
        ValueAndBestMove() = default;
        ValueAndBestMove(Value v, int r, int c) : value(v), bestRow(r), bestColumn(c) {
        }
        Value value;
        int bestRow;
        int bestColumn;
    };
    map<BoardWrapper, ValueAndBestMove> boards;
#ifdef ANALYSE
    int movesConsidered;
#endif
};

class ConsoleTTTGame {
public:
    explicit ConsoleTTTGame(bool computerGoesFirst);
    void play();
private:
    void printBoard() const;
    void doComputerMove();
    TicTacToe t;
    char computerSymbol;
    char humanSymbol;
};

#ifdef ANALYSE
class StopWatch {
public:
    StopWatch();
    void start();
    void stop();
    double time() const;
private:
    bool running;
    void printError() const;
#ifdef WIN32
    double f;
    long long start_time;
    long long total_time;
    long long getExecutionTime() const;
#else
    clock_t start_time;
    clock_t total_time;
    clock_t getExecutionTime() const;
#endif
};
ostream& operator<<(ostream& o, const StopWatch& sw);
#endif

TicTacToe::Value TicTacToe::value() const {
    return isAWin(COMPUTER) ? COMPUTER_WINS : isAWin(HUMAN) ? HUMAN_WINS : boardIsFull() ? DRAW : UNDECIDED;
}

TicTacToe::Value TicTacToe::chooseComputerMove(int& bestRow, int& bestColumn, Value alpha, Value beta, int depth) {
#ifdef ANALYSE
    ++movesConsidered;
#endif
    if (depth >= 3 && depth <= MAX_DEPTH) {
        auto itr = boards.find(BoardWrapper(board));
        if (itr != boards.end()) {
            bestRow = itr->second.bestRow;
            bestColumn = itr->second.bestColumn;
            return itr->second.value;
        }
    }
    Value bestValue = value();
    if (bestValue == UNDECIDED) {
        for (int row = 0; alpha < beta && row < 3; ++row) {
            for (int column = 0; alpha < beta && column < 3; ++column) {
                if (board(row, column) == EMPTY) {
                    board(row, column) = COMPUTER;
                    int dummyRow, dummyColumn;
                    Value value = chooseHumanMove(dummyRow, dummyColumn, alpha, beta, depth + 1);
                    board(row, column) = EMPTY;
                    if (value > alpha) {
                        alpha = value;
                        bestRow = row;
                        bestColumn = column;
                    }
                }
            }
        }
        bestValue = alpha;
    }
    if (depth >= 3 && depth <= MAX_DEPTH) {
        boards[BoardWrapper(board)] = ValueAndBestMove(bestValue, bestRow, bestColumn);
    }
    return bestValue;
}

TicTacToe::Value TicTacToe::chooseHumanMove(int& bestRow, int& bestColumn, Value alpha, Value beta, int depth) {
#ifdef ANALYSE
    ++movesConsidered;
#endif
    if (depth >= 3 && depth <= MAX_DEPTH) {
        auto itr = boards.find(BoardWrapper(board));
        if (itr != boards.end()) {
            bestRow = itr->second.bestRow;
            bestColumn = itr->second.bestColumn;
            return itr->second.value;
        }
    }
    Value bestValue = value();
    if (bestValue == UNDECIDED) {
        for (int row = 0; alpha < beta && row < 3; ++row) {
            for (int column = 0; alpha < beta && column < 3; ++column) {
                if (board(row, column) == EMPTY) {
                    board(row, column) = HUMAN;
                    int dummyRow, dummyColumn;
                    Value value = chooseComputerMove(dummyRow, dummyColumn, alpha, beta, depth + 1);
                    board(row, column) = EMPTY;
                    if (value < beta) {
                        beta = value;
                        bestRow = row;
                        bestColumn = column;
                    }
                }
            }
        }
        bestValue = beta;
    }
    if (depth >= 3 && depth <= MAX_DEPTH) {
        boards[BoardWrapper(board)] = ValueAndBestMove(bestValue, bestRow, bestColumn);
    }
    return bestValue;
}

TicTacToe::Side TicTacToe::side(int row, int column) const {
    return board(row, column);
}

bool TicTacToe::isUndecided() const {
    return value() == UNDECIDED;
}

bool TicTacToe::playMove(Side s, int row, int column) {
    if (row < 0 || row >= 3 || column < 0 || column >= 3 || board(row, column) != EMPTY)
        return false;
    board(row, column) = s;
    return true;
}

bool TicTacToe::boardIsFull() const {
    return none_of(board.cbegin(), board.cend(), [](Side s) {
        return s == EMPTY;
    });
}

bool TicTacToe::isAWin(Side s) const {
    for (int i = 0; i < 3; ++i) {
        if ((board(i, 0) == s && board(i, 1) == s && board(i, 2) == s) ||
            (board(0, i) == s && board(1, i) == s && board(2, i) == s)) {
            return true;
        }
    }
    return (board(0, 0) == s && board(1, 1) == s && board(2, 2) == s) ||
        (board(0, 2) == s && board(1, 1) == s && board(2, 0) == s);
}

bool TicTacToe::BoardWrapper::operator<(const BoardWrapper& rhs) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board(i, j) != rhs.board(i, j)) {
                return board(i, j) < rhs.board(i, j);
            }
        }
    }
    return false;
}

ConsoleTTTGame::ConsoleTTTGame(bool computerGoesFirst) :
computerSymbol(computerGoesFirst ? 'x' : 'o'), humanSymbol(computerGoesFirst ? 'o' : 'x') {
    if (computerGoesFirst) {
        doComputerMove();
        cout << endl;
    }
}

void ConsoleTTTGame::printBoard() const {
    string streep(3, '-');
    cout << streep << endl;
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column)
        if (t.side(row, column) == TicTacToe::COMPUTER)
            cout << computerSymbol;
        else if (t.side(row, column) == TicTacToe::HUMAN)
            cout << humanSymbol;
        else
            cout << ' ';
        cout << endl;
    }
    cout << streep << endl;
}

void ConsoleTTTGame::doComputerMove() {
    int bestRow, bestColumn;
#ifdef ANALYSE
    StopWatch sw;
    sw.start();
#endif
    t.chooseComputerMove(bestRow, bestColumn);
#ifdef ANALYSE
    sw.stop();
    cout << "Calculation time: " << sw << endl;
    cout << "Map size is: " << t.getMapSize() << endl;
    cout << "Moves considered: " << t.getAndResetMovesConsidered() << endl;
#endif
    cout << "Computer plays: ROW = " << bestRow << " COLUMN = " << bestColumn << endl;
    t.playMove(TicTacToe::COMPUTER, bestRow, bestColumn);
}

void ConsoleTTTGame::play() {
    do {
        int row, column;
        do {
            printBoard();
            cout << endl << "Enter row and column (starts at 0): ";
            cin >> row >> column;
        } while (!t.playMove(TicTacToe::HUMAN, row, column));
        cout << endl;
        if (t.isUndecided()) {
            printBoard();
            cout << endl;
            doComputerMove();
            cout << endl;
        }
    } while (t.isUndecided());
    printBoard();
    if (t.isAWin(TicTacToe::COMPUTER)) {
        cout << "Computer wins!!" << endl;
    }
    else if (t.isAWin(TicTacToe::HUMAN)) {
        cout << "Human wins!!" << endl;
    }
    else {
        cout << "Draw!!" << endl;
    }
}

#ifdef ANALYSE
StopWatch::StopWatch() : running(false), total_time(0) {
#ifdef WIN32
    LARGE_INTEGER performanceFrequency;
    if (QueryPerformanceFrequency(&performanceFrequency) == 0) {
        printError();
    }
    f = double(performanceFrequency.QuadPart);
#endif
}

void StopWatch::start() {
    if (!running) {
        running = true;
        start_time = getExecutionTime();
    }
}

void StopWatch::stop() {
    if (running) {
        running = false;
        total_time = getExecutionTime() - start_time;
    }
}

#ifdef WIN32
long long StopWatch::getExecutionTime() const {
    LARGE_INTEGER performanceCount;
    if (QueryPerformanceCounter(&performanceCount) == 0) {
        printError();
        return 0;
    }
    return performanceCount.QuadPart;
}
#else
clock_t StopWatch::getExecutionTime() const {
    clock_t t = clock();
    if (t == -1) {
        printError();
        return 0;
    }
    return clock();
}
#endif

void StopWatch::printError() const {
#ifdef WIN32
    LPTSTR lpMsgBuf(0);
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        lpMsgBuf,
        0,
        NULL
        );
    cerr << lpMsgBuf << endl;
    LocalFree(lpMsgBuf);
#else
    cerr << "No timer available\n" << endl;
#endif
}

double StopWatch::time() const {
#ifdef WIN32
    long long res = running ? getExecutionTime() - start_time : total_time;
    return res / f;
#else
    clock_t res = running ? getExecutionTime() - start_time : total_time;
    return static_cast<double>(res) / CLK_TCK;
#endif
}

ostream& operator<<(ostream& o, const StopWatch& sw) {
    return o << sw.time() << " sec";
}
#endif

int main() {
    cout << "Welcome to TIC-TAC-TOE" << endl;
    bool computerGoesFirst = true;
    char again;
    do {
        ConsoleTTTGame game(computerGoesFirst);
        game.play();
        do {
            cout << "Play again (y/n)? ";
            cin >> again;
        } while (again != 'y' && again != 'Y' && again != 'n' && again != 'N');
        computerGoesFirst = !computerGoesFirst;
        cout << endl;
    } while (again != 'n' && again != 'N');
    return 0;
}
