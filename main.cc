#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;
using namespace std::chrono_literals;

// TIL: % is a remainder operator in C++ and not 
// modulo
// https://stackoverflow.com/questions/12276675/modulus-with-negative-numbers-in-c
long mod(long a, long b)
{ return (a%b+b)%b; }

class Board {
    public:
        Board(int size); 
        vector<vector<bool>> getBoard();
        int getSize();
        int numberOfNeighboursForCell(int x, int y);
        void updateCell(int x, int y, bool is_alive);
        void updateBoard();
    private:
        vector<vector<bool>> board;
        vector<vector<bool>> new_board;
        int boardSize;
        tuple<int, int> normalizeCoordinates(int x, int y);
};

Board::Board(int size) {
    board = {};
    // Init Random Number Generator for Board Initialisation.
    srand((unsigned) time(NULL));

    boardSize = size;
    for (auto i=0;i<size;i+=1) {
        vector<bool> row = {};
        for (auto j=0;j<size;j+=1) {
            row.push_back(rand() % 2);
        }

        board.push_back(row);
    }
}


// Because our board is limited in size, we want to wrap any 
// coordinate that is outside of the board coordinate space.
tuple<int, int> Board::normalizeCoordinates(int x, int y) {
    return { mod(x, boardSize),  mod(y, boardSize) };
}

int Board::numberOfNeighboursForCell(int x, int y) {
    int numNeighbours = 0;
    for (int i = -1; i <= 1; i+=1) {
        for (int j = -1; j <= 1; j+=1) {
           auto [cx, cy] = normalizeCoordinates(i+x, j+y);
           numNeighbours += board[cy][cx]; 
        }

    }

    return numNeighbours;
}

vector<vector<bool>> Board::getBoard() {
    return board;
}

int Board::getSize() {
    return boardSize;
}

void Board::updateCell(int x, int y, bool is_alive) {
    board[y][x] = is_alive;
}

void Board::updateBoard() {
    new_board = {};
    for (auto y=0;y<boardSize;y+=1) {
        vector<bool> row = {};
        for (int x=0;x<boardSize;x+=1) {
            int numNeighbors = numberOfNeighboursForCell(x, y);

            if (numNeighbors < 2 || numNeighbors > 3) {
                row.push_back(0);
            } else if (numNeighbors == 3) {
                row.push_back(1);
            } else {
                row.push_back(board[y][x]);
            }
        }

        new_board.push_back(row);
    }

    // Swap new board for old board.
    board = new_board;
}

void renderBoard(Board board) {
    for (vector<bool> row: board.getBoard()) {
        for (bool cell: row) {
            if (cell) {
                cout << ".";
            } else {
                cout << " ";
            }
        }

        cout << '\n';
    }
}



void clearScreen() {
    // https://github.com/MarioTalevski/game-of-life/blob/master/GameOfLife.cpp#L171
    cout << "\033[2J;" << "\033[1;1H";
}

int main() {

    Board* board = new Board(50);
    while (true) {
        clearScreen();
        renderBoard(*board);
        board->updateBoard();
        this_thread::sleep_for(0.1s);
    }
}

