// Henry Wesson
// 2/17/2024
// SD 247
// Project 3: Implementing Battleship in C

// This program simulates playing the game Battle Ship with a computer opponent. The game boards grids
// are displayed and the user takes turns with the computer entering coordinates. Once all ships have been
// sunk the game ends and the user is prompted to play again or quit. The ships on the game board are placed
// randomly and the computer makes random guesses. I would like the improve this further by making the computer
// player take more intelligent turns based off of the previous moves it makes. I would also like to 
// add the feature of the user being able to place their own ships. 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define BOARD_SIZE 10
#define NUM_SHIPS 5
const int SHIP_LENGTHS[NUM_SHIPS] = {5, 4, 3, 3, 2};

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    Coordinate start;
    Coordinate end;
    int length;
    bool isSunk;
} Ship;

// this function intiatlizes the game board grid with nested for loops based on the BOARD_SIZE variable. spaces are represented by a period "."
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '.';
        }
    }
}

// this function uses nested for loops to display the board grid on the console. 
void displayBoard(char board[BOARD_SIZE][BOARD_SIZE], bool compBoard) {
    printf("  "); // this adds the spaces for the row numbers
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i); // this prints the columns for the grid with a space after
    }
    printf("\n"); // new line at the end of the numbers for the columns

    for (int i = 0; i < BOARD_SIZE; i++) { // for each row of the game board
        printf("%d ", i); // prints the rows number a tht he start of each row
        for (int j = 0; j < BOARD_SIZE; j++) { // for each column in the row
            if (compBoard && board[i][j] == 'S') { // if the cell has a ship print a period "." to hide it
                printf(". ");
            } else { // otherwise show a H for hit, an O for miss, or a . if no ship
                printf("%c ", board[i][j]);
            }
        }
        printf("\n"); // new line after all columns in the row
    }
}

// I added this function to check if a coordinate entered is within the game board dimensions
bool isInGrid(Coordinate c) { 
    return c.x >= 0 && c.x < BOARD_SIZE && c.y >= 0 && c.y < BOARD_SIZE;
}

// this function checks in a placed ship is in a valid location on the game board. 
bool isValidPlacement(char board[BOARD_SIZE][BOARD_SIZE], Coordinate start, Coordinate end) {
    if (!isInGrid(start) || !isInGrid(end)) { // checks if the start and end of the ship is within the grid 
        return false; // if not returns false 
    }

    if (start.x != end.x && start.y != end.y) { // if the ship's start (x and y) and end (x and y) don't align, invalid placement
        return false; // if not returns false
    }

    // this deteremines the ships placement direction by determining the 
    // differences between the x and y end and start coordinates
    int directionX = end.x - start.x, directionY = end.y - start.y;
    if (directionX != 0) directionX = directionX / abs(directionX); // if the difference isn't zero, normalize it to 0, 1, or -1
    if (directionY != 0) directionY = directionY / abs(directionY);

    // for loop to check cells in ships palcement for already existing ships
    for (int x = start.x, y = start.y; x != end.x + directionX || y != end.y + directionY; x += directionX, y += directionY) {
        if (board[x][y] != '.') { // if the cell is empty water "."
            return false; // returns false
        }
    }
    return true; // if the ship placement is in open water ".", the placment is valid.
}

// this function uses the ships start and end coordinates to place it on the 
// game board grid by deremining the differences bewtween the x nad y end and starts
void placeShip(char board[BOARD_SIZE][BOARD_SIZE], Ship ship) {
    int directionX = ship.end.x - ship.start.x, directionY = ship.end.y - ship.start.y;
    if (directionX != 0) directionX = directionX / abs(directionX); // if the difference isn't zero, normalize it to 0, 1, or -1
    if (directionY != 0) directionY = directionY / abs(directionY);

    // for loop to check if the ship is correctly placed when vertical, horizontal, or one cell length
    for (int x = ship.start.x, y = ship.start.y; x != ship.end.x + directionX || y != ship.end.y + directionY; x += directionX, y += directionY) {
        board[x][y] = 'S'; // marks ship location within cell with an "S" for ship
    }
}

// added this function to make sure ships are placed randomly, might add function for user to place their own
// ships in the future but didn't have time this time around. 
void placeShipsRandomly(char board[BOARD_SIZE][BOARD_SIZE], Ship ships[NUM_SHIPS]) {
    // for loop iterates through ships to be placed on the game board grid
    for (int i = 0; i < NUM_SHIPS; i++) {
        bool placed = false; // boolean to show if ship has been placed
        // while not placed
        while (!placed) {
            Coordinate start = {rand() % BOARD_SIZE, rand() % BOARD_SIZE}; // random starting coordinate
            bool horizontal = rand() % 2 == 0; // randomly decide if ship is horizontal or vertical
            Coordinate end = start; //sets end coordinate to equal start coordinate

            if (horizontal) { // if horizonatal, 
                end.y = start.y + SHIP_LENGTHS[i] - 1; // makes sure ship fits on board grid
            } else { // otherwise if vertical
                end.x = start.x + SHIP_LENGTHS[i] - 1; // makes sure ship fits on board grid
            }

            if (isValidPlacement(board, start, end)) { // checks if ship placement is valid and updates properties
                ships[i].start = start;
                ships[i].end = end;
                ships[i].length = SHIP_LENGTHS[i];
                ships[i].isSunk = false;
                placeShip(board, ships[i]);
                placed = true;
            }
        }
    }
}

// this functions determines if a player hits a ship with their guess
bool isHit(char board[BOARD_SIZE][BOARD_SIZE], Coordinate guess) {
    return board[guess.x][guess.y] == 'S'; // looks at the grid coordinates and if cell has an S it is a hit
}

// added this fucntion to mark the location of the players guess with H for hit or O for miss
void hitOrMiss(char board[BOARD_SIZE][BOARD_SIZE], Coordinate guess, bool hit) {
    board[guess.x][guess.y] = hit ? 'H' : 'O'; // if the player hits a ship, mark with H, if not O for miss
}

// this function determine if all cells of a ship have been sunk
bool isSunk(char board[BOARD_SIZE][BOARD_SIZE], Ship ship) {
    int directionX  = ship.end.x - ship.start.x, directionY = ship.end.y - ship.start.y;
    if (directionX  != 0) directionX  = directionX  / abs(directionX); // if the direction isn't zero, normalize to 0, 1, or -1
    if (directionY != 0) directionY = directionY / abs(directionY);    // whether vertical, horizontal, or one cell length

    // for loop to check if the cells in the ship have been marked with H for hit
    for (int x = ship.start.x, y = ship.start.y; x != ship.end.x + directionX || y != ship.end.y + directionY; x += directionX, y += directionY) {
        if (board[x][y] != 'H') { // if not the return false
            return false;
        }
    }
    return true; // otherwise return true because the ship has been sunk
}

// this function checks if the player made a valid guess, depending on if the cell has already been guessed.
bool isValidGuess(char board[BOARD_SIZE][BOARD_SIZE], Coordinate guess) {
    return isInGrid(guess) && (board[guess.x][guess.y] == '.' || board[guess.x][guess.y] == 'S'); // checks if guess is . or S to be valid
}

// this function checks if the ships on the board have been sunk or now qualify as being sunk and updates them
void updateShip(Ship ships[NUM_SHIPS], char board[BOARD_SIZE][BOARD_SIZE]) {
    // for loop interates through all the ships to check if been sunk or not
    for (int i = 0; i < NUM_SHIPS; i++) {
        if (!ships[i].isSunk && isSunk(board, ships[i])) { // if ship hasn't been marked sunk but has sunk
            ships[i].isSunk = true;                        // mark the ship as sunk by setting isSunk to true
            printf("Mayday! A ship has been sunk!\n");     // prints message for user
        }
    }
}

// this function handles user or computer player turns
void takeTurn(char board[BOARD_SIZE][BOARD_SIZE], Ship ships[NUM_SHIPS], bool isUserTurn) {
    Coordinate guess; 
    // if users turn
    if (isUserTurn) { 
        char input[100]; // buffer to store potential input
        printf("Make your guess? (row col), include a space between row and col: ");
        while (fgets(input, sizeof(input), stdin)) { // reads the input
            if (sscanf(input, "%d %d", &guess.x, &guess.y) == 2) { // if row and col ints are parsable
                if (isValidGuess(board, guess)) { // if guess if valid
                    break; 
                } else { // otherwise, guess is not valid, try again
                    printf("Invalid coordinates. Please try again: ");
                }
            } else { // else if user entered non integer row and col, error, try again
                printf("Error, invalid input. Please enter your guess as numbers in the format (row col):"); // invalid input
            }
        }
    // otherwise, if its the computer players turn
    } else { 
        do {                               // do while to 
            guess.x = rand() % BOARD_SIZE; // randomly generate coordinates until a vliad guess is made
            guess.y = rand() % BOARD_SIZE;
        } while (!isValidGuess(board, guess));
        printf("Computer's turn: (%d, %d)\n", guess.x, guess.y);
    }

    bool hit = isHit(board, guess); // checks if the guess hits a ship 
    hitOrMiss(board, guess, hit);
    if (hit) { // if hit
        printf("%s\n", isUserTurn ? "Direct Hit!" : "The computer has hit your ship!");
    } else { // otherwise miss
        printf("%s\n", isUserTurn ? "Oops, Miss." : "The computer missed.");
    }

    updateShip(ships, board); // calls updateShip
}

// this function determines if the game is over
bool isGameOver(Ship ships[NUM_SHIPS]) {
    // for loop iterates through all ships
    for (int i = 0; i < NUM_SHIPS; i++) { 
        if (!ships[i].isSunk) { // if all ships are not sunk 
            return false; // returns false
        }
    }
    return true; // otherwise true, gameover 
}

// this function prompts user to play again after gameover or to quit the game, has error handling, case-insensitive
bool playAgain() {
    char choice; // user choice variable
    while (true) {
        printf("Would you like to play again? (press y for yes, n for no): "); // prompt the user.
        choice = getchar(); // reads user input

        while (getchar() != '\n'); //consumes the newline or additional input in the buffer
        choice = tolower(choice); // converts input to lowercase in case entered uppercase
        
        if (choice == 'y' || choice == 'n') { // if input is y or n 
            break; // break loop
        } else { // otherwise invalid input
            printf("Error. Please enter y for yes, n for no.\n"); // error message if invalid input
        }
    }
    return choice == 'y'; // return true to play again if choice is y
}

// the main method function
int main() {

    printf("Weclome to my Battle Ship Game. The computer's board and the user's board will be randomly generated and displayed below. Take turns guessing the coordinates and see if you can sink the computers ships before they sink yours!");
    bool playAgainFlag = false; // sets playAgainFlag to false to start
    do { // do while loop to initialize the board for the user and comp player
        char userBoard[BOARD_SIZE][BOARD_SIZE], computerBoard[BOARD_SIZE][BOARD_SIZE];
        Ship userShips[NUM_SHIPS], computerShips[NUM_SHIPS];
        bool userTurn = true; // this keeps track of whose turn it currently is, starts set to true
        // sets up the game boards
        srand(time(NULL)); // seed random number to randomize each game 
        initializeBoard(userBoard);
        initializeBoard(computerBoard);
        placeShipsRandomly(userBoard, userShips);
        placeShipsRandomly(computerBoard, computerShips);
        // while loop for playing the game
        while (!isGameOver(userShips) && !isGameOver(computerShips)) {
            printf("\nYour Board:\n");
            displayBoard(userBoard, false);
            printf("\nComputer Board:\n");
            displayBoard(computerBoard, true);
            takeTurn(userTurn ? computerBoard : userBoard, userTurn ? computerShips : userShips, userTurn);
            userTurn = !userTurn;

            if (isGameOver(userShips)) { // if user ships are sunk, gameover
                printf("Game Over. You lose!\n");
                break;
            } else if (isGameOver(computerShips)) { // if computer ships are sunk, gameover
                printf("Game Over. You win!\n");
                break;
            }
        }

        playAgainFlag = playAgain(); // calls playAgain function
    } while (playAgain);

    printf("Goodbye!\n");
    return 0; // exits program
}