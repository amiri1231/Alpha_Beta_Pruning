#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

const int BOARD_SIZE = 3;
const char EMPTY = '-';
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

void displayBoard(const vector<vector<char>>& board);
bool checkWin(const vector<vector<char>>& board, char player);
bool isBoardFull(const vector<vector<char>>& board);
bool placeMark(vector<vector<char>>& board, int row, int col, char player);
int minimax(vector<vector<char>>& board, int depth, bool isMaximizing, int alpha, int beta, int difficulty);
void aiMove(vector<vector<char>>& board, int difficulty);
void playGame();

int main() {
    char choice;
    do {
        playGame();
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "Thanks for playing!\n";
    return 0;
}

// Display the board
void displayBoard(const vector<vector<char>>& board) {
    cout << "\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << board[i][j];
            if (j < BOARD_SIZE - 1) cout << " | ";
        }
        cout << "\n";
        if (i < BOARD_SIZE - 1) cout << "---------\n";
    }
    cout << "\n";
}

// Check for a win
bool checkWin(const vector<vector<char>>& board, char player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) return true;
    return false;
}

// Check if the board is full
bool isBoardFull(const vector<vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) return false;
        }
    }
    return true;
}



// Place mark on the board
bool placeMark(vector<vector<char>>& board, int row, int col, char player) {
    if (board[row][col] == EMPTY) {
        board[row][col] = player;
        return true;
    } else {
        cout << "Position already taken. Try again.\n";
        return false;
    }
}

// Minimax algorithm with Alpha-Beta Pruning
int minimax(vector<vector<char>>& board, int depth, bool isMaximizing, int alpha, int beta, int difficulty) {
    // Base cases: check for terminal conditions (win, loss, or draw)
    if (checkWin(board, PLAYER_O)) return 10 - depth; // AI wins
    if (checkWin(board, PLAYER_X)) return depth - 10; // Player wins
    if (isBoardFull(board)) return 0; // Draw (board is full)

    // Difficulty adjustments: shallow search for easier difficulty
    if (difficulty == 1 && depth >= 1) return 0; // Easy difficulty: shallow search
    if (difficulty == 2 && depth >= 2) return 0; // Medium difficulty: slightly deeper search

    if (isMaximizing) {
         // AI's turn (maximize score)
         // Start with the worst possible score *numeric_limits min == -INFINITI
        int maxEval = numeric_limits<int>::min();
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) { // Check if the space is empty
                    board[i][j] = PLAYER_O; // Try the move for AI
                    int eval = minimax(board, depth + 1, false, alpha, beta, difficulty); // Recursively evaluate the move
                    board[i][j] = EMPTY; // Undo the move
                    // Maximize the score (Maybe recurssive?)
                    maxEval = max(maxEval, eval); 
                     // Update alpha (best score for the maximizing player)
                    alpha = max(alpha, eval);
                    if (beta <= alpha) break; // Beta cut-off (prune the search tree)
                }
            }
        }
        return maxEval; // Return the best score for AI
    } 
     // Update alpha (best score for the maximizing player)
    else { 
        // Player's turn (minimize score)
        // Start with the best possible score (numeric limits max ==  Positive Inifinit)
        int minEval = numeric_limits<int>::max(); 
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) { // Check if the space is empty
                    board[i][j] = PLAYER_X; // Try the move for the player
                    int eval = minimax(board, depth + 1, true, alpha, beta, difficulty); // Recursively evaluate the move
                    board[i][j] = EMPTY; // Undo the move
                    minEval = min(minEval, eval); // Minimize the score
                    beta = min(beta, eval); // Update beta (best score for the minimizing player)
                    if (beta <= alpha) break; // Alpha cut-off (prune the search tree)
                }
            }
        }
        return minEval; // Return the worst score for the player
    }
}

// AI's move based on the chosen difficulty level
void aiMove(vector<vector<char>>& board, int difficulty) {
    if (difficulty == 1) { // Easy difficulty: AI makes random moves
        vector<pair<int, int>> emptyPositions; // List of empty positions
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) emptyPositions.push_back({i, j}); // Add empty positions to the list
            }
        }
        if (!emptyPositions.empty()) { // If there are empty positions
            srand(time(0)); // Seed random number generator
            auto move = emptyPositions[rand() % emptyPositions.size()]; // Choose a random position
            board[move.first][move.second] = PLAYER_O; // Place the AI's mark on the chosen position
        }
        return;
    }

    // For medium and hard difficulties, use the minimax algorithm to make optimal moves
    int bestScore = numeric_limits<int>::min(); // Start with the worst possible score
    int bestRow = -1, bestCol = -1;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) { // Check if the space is empty
                board[i][j] = PLAYER_O; // Try the move for AI
                int moveScore = minimax(board, 0, false, numeric_limits<int>::min(), numeric_limits<int>::max(), difficulty); // Evaluate the move
                board[i][j] = EMPTY; // Undo the move
                if (moveScore > bestScore) { // If this move has the best score
                    bestScore = moveScore;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }
    board[bestRow][bestCol] = PLAYER_O; // Place the AI's mark on the best move
}


// Main game function
void playGame() {
    vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, EMPTY));
    char currentPlayer = PLAYER_X;
    bool playerTurn = true;
    int difficulty;

    cout << "Welcome to Tic Tac Toe! Player is X and AI is O.\n";
    cout << "Select difficulty level (1: Easy, 2: Medium, 3: Hard): ";
    cin >> difficulty;
    while (difficulty < 1 || difficulty > 3) {
        cout << "Invalid choice. Please select difficulty (1, 2, or 3): ";
        cin >> difficulty;
    }

    displayBoard(board);

    while (true) {
        if (playerTurn) {
            int row, col;
            cout << "Player " << currentPlayer << ", enter your move (row and column: 1 2): ";
            cin >> row >> col;

            if (row < 1 || row > BOARD_SIZE || col < 1 || col > BOARD_SIZE) {
                cout << "Invalid position. Try again.\n";
                continue;
            }
            if (!placeMark(board, row - 1, col - 1, currentPlayer)) continue;
        } else {
            cout << "AI's move:\n";
            aiMove(board, difficulty);
        }

        displayBoard(board);

        if (checkWin(board, currentPlayer)) {
            cout << (playerTurn ? "Player" : "AI") << " (" << currentPlayer << ") wins!\n";
            break;
        } else if (isBoardFull(board)) {
            cout << "It's a draw!\n";
            break;
        }

        playerTurn = !playerTurn;
        currentPlayer = playerTurn ? PLAYER_X : PLAYER_O;
    }
}




// Alpha-Beta Pruning in the Code
// Purpose of Alpha-Beta Pruning:
// Alpha-Beta pruning is used to optimize the Minimax algorithm by pruning unnecessary branches of the game tree, making it more efficient.
// The idea is to eliminate moves that will not influence the final decision, thereby reducing the number of nodes that need to be explored.

// How Alpha-Beta Pruning Works in This Code:
// Minimax Algorithm: The core of the decision-making process is the Minimax algorithm. Minimax simulates every possible move in the game,
// recursively evaluates each board configuration, and assigns a score. The algorithm alternates between maximizing the score (AI's turn) and minimizing it (Player's turn).

// Alpha and Beta Parameters:

// alpha: The best score that the maximizing player can guarantee so far. Initially, it's set to negative infinity (numeric_limits<int>::min()).
// beta: The best score that the minimizing player can guarantee so far. Initially, it's set to positive infinity (numeric_limits<int>::max()).
// Pruning Process:

// During the recursion, when a move is being evaluated, the algorithm checks if it can be pruned:
// If the current move leads to a value greater than or equal to beta, the current branch can be pruned (since the minimizing player will not allow it to be chosen).
// If the current move leads to a value less than or equal to alpha, the current branch can be pruned (since the maximizing player will not allow it to be chosen).
// Effectiveness of Pruning: By pruning branches where further exploration wouldn't alter the outcome, Alpha-Beta pruning significantly reduces the number of game states evaluated.
// This makes the Minimax algorithm run faster, especially as the depth of the search tree increases.

// Example of Alpha-Beta in Action:
// The AI's turn (isMaximizing is true) tries to maximize the score. When evaluating a possible move, if that move leads to a score better than or equal to the current beta value,
// we know that the minimizing player will avoid this branch, so we can stop searching further down that path.
// Similarly, when it's the player's turn (isMaximizing is false), if the move leads to a score worse than or equal to the current alpha value, 
// we stop searching that branch because the maximizing player (AI) will avoid it.
// Impact of Difficulty on Alpha-Beta:
// The difficulty parameter adjusts the search depth:
// For easy difficulty (depth = 1), the AI only looks at a shallow number of moves, so pruning isn't as necessary.
// For medium difficulty (depth = 2), the AI performs a slightly deeper search.
// For hard difficulty (depth > 2), the AI explores deeper, where Alpha-Beta pruning has a more significant impact by skipping unpromising branches of the game tree, thus speeding up decision-making.

// Summary of Alpha-Beta Pruning Mechanism:

// In this code, Alpha-Beta pruning is integrated into the Minimax algorithm to optimize the decision-making process for the AI. 
// As the AI recursively evaluates each possible move, it uses alpha and beta values to cut off branches of the game tree that cannot influence the outcome, based
// on the principle that the minimizing or maximizing player will avoid moves that are detrimental to them. By pruning unnecessary branches,
// the algorithm becomes more efficient, particularly for harder difficulty levels where deeper searches are necessary. This allows the AI to make optimal moves faster while conserving computational resources.