#include <bits/stdc++.h>
using namespace std;

class Player
{
    const string name;
    const char value;

public:
    Player() : name{"unnamed"}, value{'Z'} {}
    Player(string name, char character) : name{name}, value{character} {}
    ~Player() {}

    char GetpVal() { return value; }
    string GetName() { return name; }
};

class GameBoard
{
    /*
        char board name = DisplayBoard
        int board name = 
    */
    vector<vector<char>> board;
    vector<int> columnMap;
    int rowMax;
    int colMax;

public:
    GameBoard(int row, int col) : board(row, vector<char>(col, ' ')), columnMap(col, row - 1), rowMax(row), colMax(col) {}
    ~GameBoard() {}

    const int getColMax() { return colMax; }

    int inputIntoMatrix(int colVal, const char &val)
    {
        int rowVal = FillBoard(colVal - 1, val);
        if (rowVal != -1)
        {
            return rowVal;
        }
        else
        {
            cout << "column is full.\n";
            return -1;
        }
    }

    void printDisplayMatrix()
    {
        cout << "\n\n====================================================\n";
        cout << "Game Board";
        cout << "\n====================================================\n";
        PrintBoard();
    }

    int winCheck(int row, int col)
    {

        // horizontal check
        int answer = HorizontalCheck(row, col);

        // vertical check
        if (answer == 1)
        {
            return answer;
        }
        else
        {
            answer = VerticalCheck(row, col);
        }

        // diagonal check 1
        if (answer == 1)
        {
            return answer;
        }
        else
        {
            answer = DiagonalCheckLeftToRight(row, col);
        }

        // diagonal check 2
        if (answer == 0)
        {
            answer = DiagonalCheckRightToLeft(row, col);
        }
        return answer;
    }

    void tieCheck(bool &gameCheck)
    {
        if (CheckTopRow() == colMax)
        {
            printDisplayMatrix();
            cout << "\n============================\n";
            cout << "It is a tie!" << '\n';
            cout << "\n============================\n";
            gameCheck = false;
        }
    }

private:
    void PrintBoard()
    {
        //  prints matrix of size row * col
        for (int i = 0; i < rowMax; i++)
        {
            for (int j = 0; j < colMax; j++)
            {

                std::cout << "[ " << board[i][j] << " ] ";
            }
            std::cout << '\n';
        }

        // prints column numbers from 1 - 7
        for (int j = 0; j < colMax; j++)
        {
            cout << "  " << j + 1 << "   ";
        }
    }

    int CheckTopRow()
    {
        int count = 0;
        for (int i = 0; i < colMax; i++)
        {

            if (board[0][i] != ' ')
            {
                count++;
            }
        }
        return count;
    }

    int FillBoard(int colVal, const char &charVal)
    {
        // columnMap stores the count of rowMax - 1
        // every turn it reduces by one which makes it to start filling upwards
        int rowVal = columnMap[colVal];

        if (rowVal != -1)
        {
            board[rowVal][colVal] = charVal;
            columnMap[colVal]--;
        }
        return rowVal;
    }

    int HorizontalCheck(int row, int col)
    {
        int count = 1;

        int j = col;
        // check towards left
        while (j > 0 && board[row][j] == board[row][j - 1])
        {
            count++;
            j--;

            if (count == 4)
            {
                return 1;
            }
        }
        // check towards right
        j = col;
        while (j < colMax - 1 && board[row][j] == board[row][j + 1])
        {
            count++;
            j++;
            if (count >= 4)
            {
                return 1;
            }
        }

        return 0;
    }

    int VerticalCheck(int row, int col)
    {
        int count = 1;

        // initialize incrementer
        int i = row;

        // check downwards
        while (i < rowMax - 1 && board[i][col] == board[i + 1][col])
        {
            i++;
            count++;
            if (count >= 4)
            {
                return 1;
            }
        }

        // reinitialize
        i = row;

        // check upwards
        while (i > 0 && board[i][col] == board[i - 1][col])
        {
            count++;
            i--;
            if (count >= 4)
            {
                return 1;
            }
        }

        return 0;
    }

    int DiagonalCheckLeftToRight(int row, int col)
    {
        int count = 1;
        int i = row, j = col;
        // check downwards- towards max - 1
        while (i < rowMax - 1 && j < colMax - 1 &&
               board[i][j] == board[i + 1][j + 1])
        {
            i++;
            j++;
            count++;
            if (count == 4)
            {
                return 1;
            }
        }

        i = row, j = col;

        // check upwards - towards 0
        while (i > 0 && j > 0 && board[i][j] == board[i - 1][j - 1])
        {
            i--;
            j--;
            count++;
            if (count >= 4)
            {
                return 1;
            }
        }

        return 0;
    }
    int DiagonalCheckRightToLeft(int row, int col)
    {
        int i = row, j = col;

        int count = 1;

        // check diagonal downwards
        while (i < rowMax - 1 && j > 0 && board[i][j] == board[i + 1][j - 1])
        {
            i++;
            j--;
            count++;
            if (count == 4)
            {
                return 1;
            }
        }

        // check upwards
        i = row, j = col;
        while (j < colMax - 1 && i > 0 && board[i][j] == board[i - 1][j + 1])
        {
            i--;
            j++;
            count++;
            if (count >= 4)
            {
                return 1;
            }
        }

        return 0;
    }
};

class GameManager
{
    unique_ptr<GameBoard> board;
    unique_ptr<Player> red;
    unique_ptr<Player> blue;
    bool startGame;

public:
    void gameMenu()
    {
        DisplayStartMessage();

        MenuOptions();
    }

    void runGame(int rowValueMax, int colValueMax)
    {
        startGame = true;

        // initialize empty char board (2D vector) to play on
        board = unique_ptr<GameBoard>(new GameBoard(rowValueMax, colValueMax));

        // initialize players red and blue
        red = unique_ptr<Player>(new Player("Red", 'R'));
        blue = unique_ptr<Player>(new Player("Blue", 'B'));

        int turnCounter = 0;

        while (startGame)
        {
            board->printDisplayMatrix();

            if (turnCounter % 2 == 0)
            {
                PlayTurn(red, startGame);
            }
            else
            {
                PlayTurn(blue, startGame);
            }

            turnCounter++;

            if (turnCounter >= rowValueMax * colValueMax && startGame)
            {
                board->tieCheck(startGame);
            }
        }
    }

private:
    void MenuOptions()
    {
        int int_val, rowMax = 6, colMax = 7;
        bool menuLoop = true;
        while (menuLoop)
        {
            cout << "\n=============================================================="
                    "=====\n";
            cout << "Menu Options: \n";
            cout << "================================================================"
                    "===\n";
            cout << "Row size: " << rowMax << "\nColumn Size: " << colMax << '\n';
            cout << "Press 1 to start a game.\n";
            cout << "Press 2 to change row and column size.\n";
            cout << "Press 3 to exit.\n";
            cout << "Enter Choice: ";
            cin >> int_val;

            switch (int_val)
            {
            case 1:
                runGame(rowMax, colMax);
                break;
            case 2:
                ChangeRowColValues(rowMax, colMax);
                break;
            case 3:
                menuLoop = false;
                break;
            }
        }
    }

    void ChangeRowColValues(int &rowMax, int &colMax)
    {
        cout << "Enter the no. of rows: ";
        cin >> rowMax;
        cout << endl;
        rowMax = rowMax < 4 ? 4 : rowMax;
        cout << "Enter the no. of columns: ";
        cin >> colMax;
        cout << endl;
        colMax = colMax < 4 ? 4 : colMax;
    }

    void PlayTurn(unique_ptr<Player> &plyr, bool &gameCheck)
    {
        // Take input from player
        cout << "\n\n=============================================================="
                "=====\n";
        cout << "How To Play:\n";
        cout << "=================================================================="
                "=\n";
        cout << "- Populate any column from 1 to 7 per turn.\n";
        cout << "- Numbers below 1 and above 7 will get rounded to 1 or 7.\n";
        // Whos turn
        cout << "\n------------------------------------------\n";
        cout << " Player's Turn: " << plyr->GetName() << '\n';
        cout << "------------------------------------------";
        cout << "\nEnter the column number to fill:\n";
        int colno;
        cin >> colno;

        if (colno < 1)
        {
            colno = 1;
        }
        else if (colno > board->getColMax())
        {
            colno = board->getColMax();
        }

        // input into the column
        // T - O(m)
        // S - O(m*n)
        int rowVal = board->inputIntoMatrix(colno, plyr->GetpVal());

        // check for win if legal move
        if (rowVal != -1)
        {
            int winChk = board->winCheck(rowVal, colno - 1);

            if (winChk == 1)
            {
                board->printDisplayMatrix();
                cout << "\n========================================\n";
                cout << "========================================\n";
                cout << plyr->GetName() << " has Won!\n";
                cout << "\n========================================\n";
                cout << "========================================\n";
                gameCheck = false;
            }
        }
    }

    void DisplayStartMessage()
    {
        cout << "\n\n=============================================================="
                "======\n";
        cout << "|                                                                 "
                " |\n";
        cout << "|                              Connect-4                          "
                " |\n";
        cout << "|                                                                 "
                " |\n";
        cout << "=================================================================="
                "==\n";

        cout << "\n\n=============================================================="
                "=====\n";
        cout << "Rules:\n";
        cout << "=================================================================="
                "=\n";
        cout << "1. Two-Player (Red and Blue) Game.\n";
        cout << "2. Board size is 6 rows and 7 columns.\n";
        cout << "3. The player who can connect 4 dots first wins.\n";
        cout
            << "4. Dots can be connected horizontally, vertically or diagonally.\n";
        cout << "5. Game is tie if board gets full without any 4 dots connecting\n";

        cout << "\n\n=============================================================="
                "=====\n";
        cout << "How To Play: \n";
        cout << "=================================================================="
                "=\n";
        cout << "-Each Step choose a column number in which to drop the dot.\n";
        cout << "-Rows are filled bottom to up in the column.\n";
        cout << "-If column is full turn is skipped.\n";
        cout << "-Red goes first.\n";
        cout << "=================================================================="
                "=\n";
    }
};

int main()
{
    GameManager *game = new GameManager();
    game->gameMenu();
    delete game;
}