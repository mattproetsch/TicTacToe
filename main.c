// Welcome back to "Introduction to C"


//
// This is C99, so compile with -std=c99 or equivalent
// gcc -std=c99 -o tictactoe tictactoe.c


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


// color #defines
#ifndef _WIN32
#define USE_COLOR
#endif

#ifdef USE_COLOR
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#endif


//
// typedefs
typedef enum SPACE_TYPE
{
    Blank,
    X,
    O
} SpaceType;

//
// global constants
const char *SPACE_TYPE_REPR[] = {" ", "X", "O"};
const int32_t NUM_ROWS = 3;
const int32_t NUM_COLS = 3;


//
// prototypes
const char *space_repr(SpaceType*, int32_t, int32_t);
void print_board(SpaceType*);
int32_t request_move();
bool move_valid(SpaceType*, int32_t);
bool make_player_move(SpaceType*, int32_t);
bool make_computer_move(SpaceType*);
bool is_winning_move(SpaceType*, int32_t);
bool board_full(SpaceType*);


//
// main method
int32_t main()
{
    // PRNG used for making computer moves sometimes
    srand(0);
    SpaceType board[9] = { Blank };

    bool game_over = false;
    SpaceType winner;

    // Main game loop
    while (true)
    {
        print_board(board);

        int32_t move = request_move();
        if (!move_valid(board, move))
        {
            printf("Invalid move\n");
            continue;
        }

        game_over = make_player_move(board, move);
        if (game_over)
        {
            winner = X;
            break;
        }

        if (board_full(board))
        {
            winner = Blank;
            break;
        }

        game_over = make_computer_move(board);
        if (game_over)
        {
            winner = O;
            break;
        }

        
    }

    printf("Game over!\n");
    
    printf("Final game board:\n");
    print_board(board);

    // Test game result, print appropriate message
    switch (winner) 
    {
        case X:
            printf("YOU WIN!\n");
            break;
        case O:
            printf("You lost.\n");
            break;
        case Blank:
            printf("It was a tie.\n");
            break;
    }


    return 0;
}

//
// Given a board, row, and col, returns the string representation
// ("X", "O", or an empty space) of the occupant
const char *space_repr(SpaceType *board, int32_t row, int32_t col)
{
    if (row >= NUM_ROWS)
    {
        printf("space_repr: Row index invalid\n");
        return NULL;
    }
    if (col >= NUM_ROWS)
    {
        printf("space_repr: Column index invalid\n");
        return NULL;
    }

    SpaceType board_entry = board[row * NUM_ROWS + col];
    return SPACE_TYPE_REPR[board_entry];
}

//
// Prints a colorized representation of the tic-tac-toe board
// On Windows, just prints the board, no ANSI colors
void print_board(SpaceType *board)
{
    static const char TOP_ROW[] = " \u250C1\u252C2\u252C3\u2510";
    static const char BTM_ROW[] = " \u2514 \u2534 \u2534 \u2518";

    printf("%s\n", TOP_ROW);

    for (int32_t i = 0; i < NUM_ROWS; i++)
    {
        printf("%d ", i + 1);
        for (int32_t j = 0; j < NUM_COLS; j++)
        {
#ifdef USE_COLOR
            if (board[i * NUM_ROWS + j] == X)
            {
                printf(ANSI_COLOR_CYAN);
            }
            else if (board[i * NUM_ROWS + j] == O)
            {
                printf(ANSI_COLOR_RED);
            }
#endif

            if (j < NUM_COLS - 1)
            {
#ifdef USE_COLOR
                printf("%s" ANSI_COLOR_RESET "\u2502", space_repr(board, i, j));
#else
                printf("%s\u2502", space_repr(board, i, j));
#endif
            }
        }

        printf("%s" ANSI_COLOR_RESET, space_repr(board, i, NUM_COLS - 1));
        printf("\n");

        if (i < NUM_ROWS - 1)
        {
            printf(" \u251C\u2500\u253C\u2500\u253C\u2500\u2524\n");
        }
    }

    printf("%s\n", BTM_ROW);
}

//
// Prompts the player to make a move and returns the move index on the game board
int32_t request_move()
{
    int32_t playerMoveRow, playerMoveCol;
    printf("Enter a row and col separated by a space (i.e. 1 1): ");
    scanf("%d%d", &playerMoveRow, &playerMoveCol);
    return (playerMoveRow - 1) * NUM_ROWS + playerMoveCol - 1;
}

//
// Returns whether a given move is valid
bool move_valid(SpaceType *board, int32_t move)
{
    if (move >= NUM_ROWS * NUM_COLS)
    {
        return false;
    }
    else if (move < 0)
    {
        return false;
    }

    if (board[move] != Blank)
    {
        return false;
    }

    return true;
}

//
// Updates the state of the game board with player's selected move.
// Returns true if the move causes the player to win.
bool make_player_move(SpaceType *board, int32_t move)
{
    board[move] = X;
    if (is_winning_move(board, move))
    {
        return true;
    }

    return false;
}

//
// Selects a reasonable move for the computer and updates the game board.
// Returns true if the move causes the computer to win.
bool make_computer_move(SpaceType *board)
{
    bool moveMade = false;
    int32_t move;


    // Loop through each more on the board,
    // testing if it would cause the player to win
    // If it would, then block it

    // Alternatively, check if taking it would cause Computer to win
    // If it would, then take it
    for (int32_t i = 0; i < NUM_ROWS; i++)
    {
        for (int32_t j = 0; j < NUM_COLS; j++)
        {
            move = i * NUM_ROWS + j;
            if (board[move] == Blank)
            {
                // Test if it's a winning move for Human
                board[move] = X;

                if (is_winning_move(board, move))
                {
                    board[move] = O;
                    moveMade = true;
                    break;
                }
                
                // Test if it's a winning move for Computer
                board[move] = O;
                if (is_winning_move(board, move))
                {
                    moveMade = true;
                    break;
                }

                // Reset
                board[move] = Blank;
            }
        }

        if (moveMade)
        {
            break;
        }
    }

    // Take center if not already taken
    if (!moveMade && board[4] == Blank)
    {
        board[4] = O;
        moveMade = true;
    }

    // Make random move
    if (!moveMade)
    {
        do {
            move = rand() % 9;
        } while (board[move] != Blank);
        board[move] = O;
    }

    return is_winning_move(board, move);
}

//
// Tests whether the given move causes a three-in-a-row.
bool is_winning_move(SpaceType *board, int32_t move)
{
    SpaceType playerType = board[move];
    if (playerType == Blank)
    {
        printf("is_winning_move: Tried to check if blank was winning move\n");
        return false;
    }

    int32_t row = move / 3;
    int32_t col = move % 3;

    bool checkSeDiagonal = (move % 4 == 0);
    bool checkSwDiagonal = ((move - 2) % 2 == 0 && (move - 2) <= 4);
    
    // check same row
    if (board[row * NUM_ROWS] == playerType 
        && board[row * NUM_ROWS + 1] == playerType
        && board[row * NUM_ROWS + 2] == playerType)
    {
        return true;
    }

    // check same col
    if (board[col] == playerType
        && board[col + NUM_ROWS] == playerType
        && board[col + 2 * NUM_ROWS] == playerType)
    {
        return true;
    }

    // check SE diagnal
    if (checkSeDiagonal)
    {
        if (board[0] == playerType
            && board[4] == playerType
            && board[8] == playerType)
        {
            return true;
        }
    }

    // check SW diagonal
    if (checkSwDiagonal)
    {
        if (board[2] == playerType
            && board[4] == playerType
            && board[6] == playerType)
        {
            return true;
        }
    }

    return false;
}

//
// Returns whether or not all spaces have been filled.
bool board_full(SpaceType *board)
{
    for (int32_t i = 0; i < NUM_ROWS; i++)
    {
        for (int32_t j = 0; j < NUM_COLS; j++)
        {
            if (board[i * NUM_ROWS + j] == Blank)
            {
                return false;
            }
        }
    }

    return true;
}

