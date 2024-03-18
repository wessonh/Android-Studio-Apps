Henry Wesson
// 2/17/2024
// SD 247
// Project 3: Implementing Battleship in C

Compile the program and run battleship.exe on windows or ./battleship on Mac or Linux

This program simulates playing the game Battle Ship with a computer opponent. 
The program displays two game boards as a 10x10 grid.
Then player and the computer take turns takes turns with the computer entering coordinates.
Once all of one of the players ships have been sunk the game ends and the user is prompted to play again or quit. 

Each ship occupies a set number of cells on game board.
Each player has 5 ships, sizes 2, 3, 4 ,and 5 squares long.
The 10x10 grid is a 2D array.  
Ships can be placed either vertically or horizontally and in random positions on the board.
The program also has functions check if the placements are valid and ship fits on the board and is not
overlapping other ships. 
The game checks if the user and computer guesses are in valid locations (within the board and not the same place twice. 
The user inputs their own guess in the format"row col" with row and col being integers.
The computer uses an algorithm to randomly generate its guess coordinates. 
Empty water ismarked as ".", and ships are marked as "S". 
A hit is marked with a "H" and a miss is marked with a "O"
The game provides feedback for each turn.
The game also has error handling to make sure coordinates are entered in the proper format and y and n to
play again or quit are entered correctly. 



In the future, I am planning on making the computer strategy to not be randomized and
base its next moves off it's previous moves for more challenging and realistic gameplay.
I would also like to add the ability for the user to place their own ships on the grid.