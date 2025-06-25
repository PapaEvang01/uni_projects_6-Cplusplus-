/*
============================================
    Tic-Tac-Toe with AI - C++ Console Game
============================================

Description:
------------
This is a terminal-based Tic-Tac-Toe game written in C++.
You can play against a computer opponent with 3 difficulty levels:

 1 - Easy:     AI picks random moves
 2 - Medium:   AI blocks your winning moves
 3 - Hard:     AI uses Minimax algorithm (unbeatable)

Features:
---------
- Text-based 3x3 board display with row/column labels (A-C, 1-3)
- User input in formats like A1, b3, 2C, etc.
- Option to go first or second
- Clear end-game messages (win, lose, draw)
- Structured and modular code

Controls:
---------
- Input your move using cell coordinates (e.g., A1, B3, 3C)
- The game automatically alternates turns
- Restart by rerunning the program

Author: Vaggelis Papaioannou

*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

const int SIZE = 3;
const char PLAYER = 'X';
const char AI = 'O';

// ===================================================
// Function: printBoard
// Purpose : Displays the current state of the 3x3 game board
// Input   : A 2D character array 'board' representing the grid
// Output  : Prints the board with row labels (A–C) and column numbers (1–3)
// ===================================================
void printBoard(char board[SIZE][SIZE]) {
    cout << "\n    1   2   3\n";  // Column headers
    for (int r = 0; r < SIZE; r++) {
        cout << char('A' + r) << " | ";  // Row label (A, B, C)
        for (int c = 0; c < SIZE; c++) {
            cout << board[r][c];        // Cell content: ' ', 'X', or 'O'
            if (c < SIZE - 1) cout << " | ";
        }
        cout << "\n";
        if (r < SIZE - 1) cout << "  |---|---|---\n";  // Row separator
    }
    cout << "\n";
}

// ===================================================
// Function: checkWinner
// Purpose : Checks if there is a winner on the board
// Input   : A 2D character array 'board' representing the grid
// Returns :
//   - 'X' if the player has won
//   - 'O' if the AI has won
//   - ' ' (space) if there is no winner yet
// Logic   : Checks all rows, columns, and both diagonals for matching symbols
// ===================================================
char checkWinner(char board[SIZE][SIZE]) {
    // Check rows
    for (int r = 0; r < SIZE; r++)
        if (board[r][0] != ' ' && board[r][0] == board[r][1] && board[r][1] == board[r][2])
            return board[r][0];

    // Check columns
    for (int c = 0; c < SIZE; c++)
        if (board[0][c] != ' ' && board[0][c] == board[1][c] && board[1][c] == board[2][c])
            return board[0][c];

    // Check main diagonal
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];

    // Check anti-diagonal
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    return ' ';  // No winner found
}

// ===================================================
// Function: isDraw
// Purpose : Determines if the game has ended in a draw
// Input   : A 2D character array 'board' representing the grid
// Returns :
//   - true  → if all cells are filled and there is no winner
//   - false → if at least one cell is empty
// Logic   : Scans the entire board for any empty cell (' ')
// ===================================================
bool isDraw(char board[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            if (board[r][c] == ' ')
                return false;
    return true;
}

// ===================================================
// Function: tryWinningMove
// Purpose : Checks if the given player (symbol) can win in one move
// Input   :
//   - board : 3x3 game board
//   - symbol: the player symbol to check ('X' or 'O')
// Output   :
//   - r, c  : the row and column of the winning move (via reference)
// Returns  :
//   - true  → if a winning move is found and (r, c) are set
//   - false → if no winning move is available
// Logic    : Tries placing 'symbol' in every empty cell temporarily,
//            checks if it results in a win using checkWinner(),
//            and undoes the move afterward.
// ===================================================
bool tryWinningMove(char board[SIZE][SIZE], int &r, int &c, char symbol) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = symbol;  // Try move
                if (checkWinner(board) == symbol) {
                    r = i;
                    c = j;
                    board[i][j] = ' '; // Undo move
                    return true;
                }
                board[i][j] = ' '; // Undo move
            }
        }
    }
    return false;
}


// ===================================================
// Function: isPlayerFirst
// Purpose : Asks the user whether they want to make the first move
// Input   : User types 'y' or 'n'
// Returns :
//   - true  → if user types 'y' or 'Y'
//   - false → otherwise
// ===================================================
bool isPlayerFirst() {
    char choice;
    cout << "Do you want to play first? (y/n): ";
    cin >> choice;
    return (choice == 'y' || choice == 'Y');
}


// ===================================================
// Function: chooseDifficulty
// Purpose : Prompts the user to select the AI difficulty level
// Returns :
//   - 1 for Easy   → Random moves
//   - 2 for Medium → Random + blocks your wins
//   - 3 for Hard   → Unbeatable AI using Minimax
// Input validation: Loops until the user enters a valid number (1–3)
// ===================================================
int chooseDifficulty() {
    cout << "Choose AI difficulty:\n";
    cout << "1 - Easy (random)\n";
    cout << "2 - Medium (block human wins)\n";
    cout << "3 - Hard (unbeatable)\n";
    int level;
    while (true) {
        cin >> level;
        if (level >= 1 && level <= 3) break;
        cout << "Invalid choice. Choose 1, 2, or 3: ";
    }
    return level;
}


// ===================================================
// Function: makeEasyAIMove
// Purpose : Executes an "easy" AI move by randomly selecting an empty cell
// Input   : The current state of the game board (3x3)
// Behavior: 
//   - Picks a random cell (row, column)
//   - Repeats until it finds an empty cell (' ')
//   - Places the AI symbol ('O') in that cell
// Notes   : This strategy is purely random and not strategic
// ===================================================
void makeEasyAIMove(char board[SIZE][SIZE]) {
    int r, c;
    do {
        r = rand() % SIZE;
        c = rand() % SIZE;
    } while (board[r][c] != ' ');
    board[r][c] = AI;
}


// ===================================================
// Function: makeMediumAIMove
// Purpose : Executes a "medium" difficulty AI move
// Input   : The current state of the game board (3x3)
// Behavior:
//   1. If AI can win in one move, it makes that move
//   2. Else if the player can win next move, AI blocks it
//   3. Else it falls back to a random move (easy mode)
// Notes   : Introduces basic defensive strategy to the AI
// ===================================================
void makeMediumAIMove(char board[SIZE][SIZE]) {
    int r, c;
    if (tryWinningMove(board, r, c, AI)) {
        board[r][c] = AI;  // Take winning move
    } else if (tryWinningMove(board, r, c, PLAYER)) {
        board[r][c] = AI;  // Block player's winning move
    } else {
        makeEasyAIMove(board);  // Random move
    }
}


// ===================================================
// Function: evaluate
// Purpose : Assigns a numeric score to the current board state
// Input   : The current game board
// Returns :
//   +10 if AI ('O') has won
//   -10 if Player ('X') has won
//     0 if the game is still ongoing or a draw
// Used by: minimax() to evaluate terminal states
// ===================================================
int evaluate(char board[SIZE][SIZE]) {
    char winner = checkWinner(board);
    if (winner == AI) return +10;
    if (winner == PLAYER) return -10;
    return 0;
}


// ===================================================
// Function: minimax
// Purpose : Implements the Minimax algorithm recursively
// Input   :
//   - board: current game board
//   - isMaximizing: true if it's AI's turn, false for Player
// Returns :
//   - Best score possible for the current player
// Logic   :
//   - If game is over (win/draw), return score from evaluate()
//   - If AI's turn: maximize the score
//   - If Player's turn: minimize the score
// ===================================================
int minimax(char board[SIZE][SIZE], bool isMaximizing) {
    int score = evaluate(board);
    if (score == 10 || score == -10 || isDraw(board))
        return score;

    if (isMaximizing) {
        int best = -1000;
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (board[r][c] == ' ') {
                    board[r][c] = AI;
                    best = max(best, minimax(board, false));
                    board[r][c] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (board[r][c] == ' ') {
                    board[r][c] = PLAYER;
                    best = min(best, minimax(board, true));
                    board[r][c] = ' ';
                }
            }
        }
        return best;
    }
}


// ===================================================
// Function: makeHardAIMove
// Purpose : Executes the best possible move using Minimax algorithm
// Input   : The current game board
// Behavior:
//   - Tries all possible moves for AI
//   - Uses minimax() to score each move
//   - Picks the move with the highest score
// Result  : The AI makes an unbeatable move
// ===================================================
void makeHardAIMove(char board[SIZE][SIZE]) {
    int bestVal = -1000;
    int bestRow = -1, bestCol = -1;

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] == ' ') {
                board[r][c] = AI;
                int moveVal = minimax(board, false);
                board[r][c] = ' ';

                if (moveVal > bestVal) {
                    bestRow = r;
                    bestCol = c;
                    bestVal = moveVal;
                }
            }
        }
    }

    board[bestRow][bestCol] = AI;
}

// ===================================================
// Function: makeAIMove
// Purpose : Executes an AI move based on selected difficulty
// Input   : 
//   - board     : current game board
//   - difficulty: AI difficulty level (1 = Easy, 2 = Medium, 3 = Hard)
// Behavior:
//   - Calls the appropriate AI strategy function:
//       1 → Random (easy)
//       2 → Block + win logic (medium)
//       3 → Minimax (hard, unbeatable)
// ===================================================
void makeAIMove(char board[SIZE][SIZE], int difficulty) {
    switch (difficulty) {
        case 1: makeEasyAIMove(board); break;
        case 2: makeMediumAIMove(board); break;
        case 3: makeHardAIMove(board); break;
    }
}


// ===================================================
// Function: playerMove
// Purpose : Handles input and move logic for the human player
// Input   : 
//   - board: the current game board
// Behavior:
//   - Prompts the player to enter a move (e.g., A1, 2B, b3, etc.)
//   - Accepts both formats: letter+number or number+letter (case-insensitive)
//   - Validates:
//       - Move format (length 2)
//       - Valid board coordinates
//       - Cell availability (not already taken)
//   - Places player's symbol ('X') on the board if valid
// ===================================================
void playerMove(char board[SIZE][SIZE]) {
    string move;
    int row, col;
    cout << "Your move (e.g., A1, B3): ";
    while (true) {
        cin >> move;

        if (move.length() != 2) {
            cout << "Invalid format. Use A1–C3: ";
            continue;
        }

        // Normalize input (e.g., a1 → A1, 2C → C2)
        char rChar = toupper(move[0]);
        char cChar = move[1];
        if (isdigit(rChar)) swap(rChar, cChar); // Allow 1A as well as A1

        row = rChar - 'A';  // Convert 'A', 'B', 'C' → 0, 1, 2
        col = cChar - '1';  // Convert '1', '2', '3' → 0, 1, 2

        // Validate bounds
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            cout << "Invalid cell. Use A1–C3: ";
        } else if (board[row][col] != ' ') {
            cout << "Cell taken. Try again: ";
        } else {
            board[row][col] = PLAYER;
            break;
        }
    }
}


// ===================================================
// Function: playGame
// Purpose : Runs the main game loop of Tic-Tac-Toe
// Behavior:
//   - Initializes an empty board
//   - Prompts user for:
//       - Turn order (first or second)
//       - AI difficulty level
//   - Alternates turns between the player and AI
//   - Prints the board after each move
//   - Ends when there's a winner or a draw
//   - Announces the game result
// ===================================================
void playGame() {
    char board[SIZE][SIZE] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
    char winner = ' ';
    srand(time(0)); // Random seed for AI moves

    bool playerFirst = isPlayerFirst();
    int difficulty = chooseDifficulty();

    printBoard(board);

    while (true) {
        if (playerFirst) {
            playerMove(board);
        } else {
            cout << "AI is thinking...\n";
            makeAIMove(board, difficulty);
        }

        printBoard(board);
        winner = checkWinner(board);
        if (winner != ' ' || isDraw(board)) break;

        playerFirst = !playerFirst; // Alternate turns
    }

    // Announce result
    if (winner == PLAYER)
        cout << "🎉 You win!\n";
    else if (winner == AI)
        cout << "💻 AI wins!\n";
    else
        cout << "It's a draw!\n";
}

// ===================================================
// Function: main
// Purpose : Entry point of the program
// Behavior: Calls playGame() to start the match
// ===================================================
int main() {
    playGame();
    return 0;
}
