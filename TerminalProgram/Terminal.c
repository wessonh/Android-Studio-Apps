// Henry Wesson
// SD 247
// 3/4/2024
// Project 4: C Terminal

// This program acts as a termial that allows the user to run common system commands. These ionclude navagating directories and managing files.
// This program can run (If one Windows, system commands such as dir, copy, move, del, type nul > filename, md, rd, findstr, type, etc...)
// This program can run (If on Unix/Linux/macOS, system commands such as ls, cp, mv, rm, touch, mkdir, rmdir, pwd, cd, etc...)
// It also has options to use custom commands list_dirctory, create_directory, change-directory, delete_directory to run
// have the program only rely on system() and to get around any cross-platform compatability issues with system commands.
// This program has a custom command "history" that can be used to view the last 10 commands.
// History commands can be rerun via their indexes by using !n where n is the index of the command in the history.
// This program also has a command "secure_file" that allows the user to encrypt or decrypt files using a caesar cipher.
// This program features error handling for invalid inputs and cross compatability for Windows or Unix/Linux/macOS operating systems. 
// The user will be prompted with a > symbol when entering their input. 
// To encrypt a file use "secure_file -e -f filename". To decrypt use "secure_file -d -f filename".
// The commands "quit" or "exit" end the program.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef _WIN32 // if windows
#include <direct.h> // including this for _mkdir on Windows OS
#endif

#define MAX_COMMAND_LENGTH 1024 // maximum characters for command length                                           
#define MAX_HISTORY 10 // maximum length of history commands before it starts overwriting
#define SHIFT 3 // shift amount for caesar cipher

char* commandHistory[MAX_HISTORY];
int historyCount = 0;

// the function prototypes
void addToHistory(const char* command);
void printHistory();
void runCommand(char* input);
int parseEncrypt(char* input, char* fileName);
void encryptDecrypt(char* args, int encrypt);
void caesar(FILE* inputFile, FILE* outputFile, int encrypt);
void changeDirectory(const char* path);
void listDirectory(const char* path);
void createDirectory(const char* path);
void deleteDirectory(const char* path);


// the main function that calls the other functions to run the terminal
int main() {  
    char input[MAX_COMMAND_LENGTH]; // buffer that stores the command input

    while (1) { // loop for running the different commands 
        printf("> "); // the prompt for the user to enter a command
        fflush(stdout); // flush stdout so that prompt is properly displayed

        if (!fgets(input, MAX_COMMAND_LENGTH, stdin)) { // if user input command is invalid
            fprintf(stderr, "Error, invalid input! Please try again.\n"); // error message, try again
            continue;
        }

        input[strcspn(input, "\n")] = 0; // this gets rid of any accidental newlines

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) { // if quit or exit commands are entered
            break; // breaks loop
        }

        if (strcmp(input, "history") == 0) { // if history command is entered
            printHistory(); // displays the history
            continue;
        }

        addToHistory(input); // adds input to command history
        runCommand(input); // runs the command
    }

    for (int i = 0; i < historyCount; ++i) { // for loop interates through history of commands
        free(commandHistory[i]); // this releases the allocated memory to prevent memory leakage
    }

    return 0; // end program
}

// this function adds the entered commands to the history list. @param command is the command that gets added to the history
void addToHistory(const char* command) { 
    if (historyCount < MAX_HISTORY) { // if history isn't full, add the command to the list
        commandHistory[historyCount++] = strdup(command);
    } else { // otherwise, remove the oldest command is the history is full
        free(commandHistory[0]);
        memmove(commandHistory, commandHistory + 1, sizeof(char*) * (MAX_HISTORY - 1)); // shift commands one to the left
        commandHistory[MAX_HISTORY - 1] = strdup(command); // this adds the new command at the end of the list
    }
}

// this function prints the command history for the user
void printHistory() { 
    for (int i = 0; i < historyCount; ++i) { // iterates through history
        printf("%d: %s\n", i + 1, commandHistory[i]); // prints it
    }
}

// this function parses the input and runs the commands. @param input is the user input string that contains the command
void runCommand(char* input) { 
    char command[MAX_COMMAND_LENGTH];
    char args[MAX_COMMAND_LENGTH] = {0}; // buffer

    sscanf(input, "%s %[^\n]", command, args); // extracts command and its arguements from the user input

    if (input[0] == '!' && isdigit(input[1])) { // if command is from the command history
        int historyIndex = atoi(&input[1]) - 1; // this converts the string to an integer
        if (historyIndex >= 0 && historyIndex < historyCount) { // if the index is valid, run the command
            printf("Executing: %s\n", commandHistory[historyIndex]);
            runCommand(commandHistory[historyIndex]); // recursive call
        } else { // otherwise, error message if number is invalid
            fprintf(stderr, "Error! History number '%d' is invalid.\n", historyIndex + 1);
        }
        return;
    }   

    // Here is let the user use change_directory, list_directory, create_directory, and delete_directory rather than
    // relying on system() just to see if I could. This works as intended after doing some bug testing. Kind of redundant but
    // I did this for the learning aspect. This is also cross-platform compatable. 
    if (strcmp(command, "change_directory") == 0) { // if change_directory
        changeDirectory(args);
    } else if (strcmp(command, "list_directory") == 0) { // else if list_directory
        listDirectory(args[0] ? args : ".");
    } else if (strcmp(command, "create_directory") == 0) { // else if create_directory
        createDirectory(args);
    } else if (strcmp(command, "delete_directory") == 0) { // else if delete_directory
        deleteDirectory(args);
    } else if (strncmp(command, "secure_file", 11) == 0) {
        char fileName[MAX_COMMAND_LENGTH]; // else if secure file (encrypt/decrypt)
        int operation = parseEncrypt(input, fileName); // parses the input
        if (operation != -1) {
            encryptDecrypt(fileName, operation);
        }
    } else { // otherwise, pass any other command to system shell
        system(input);
    }
}

// the function to parse the input for the secure_file command to determine encrypt or decrypt and what file
// @param input is the user input string with the command, fileName is the buffer for the fileName gotten from the input
int parseEncrypt(char* input, char* fileName) { 
    char mode[3], flag[3];
    if (sscanf(input, "secure_file %2s %2s %s", mode, flag, fileName) == 3) { // extracts the mode, flag, or file name from user input
        if (strcmp(mode, "-e") == 0 && strcmp(flag, "-f") == 0) return 1; // returns 1 for encrypt
        if (strcmp(mode, "-d") == 0 && strcmp(flag, "-f") == 0) return 0; // returns 0 for decrypt
    }
    fprintf(stderr, "Error! Invalid format! Please use format as 'secure_file -[e|d] -f <filename>'.\n");
    return -1; // invalid command
}

// the function to encrypt or decrypt a file using caesar cipher. @param fileName is the name of file being secured, encrypt is 1 to encrypt 0 to decrypt
void encryptDecrypt(char* fileName, int encrypt) {
    char outputFileName[MAX_COMMAND_LENGTH];
    snprintf(outputFileName, sizeof(outputFileName), "%s.%s", fileName, encrypt ? "enc" : "dec"); // add .enc or .dec to the file name after doing encrypting/decrypting

    if (access(outputFileName, F_OK) == 0) { // if file already exists, cancels operation
        fprintf(stderr, "Error! File '%s' already exists. Operation cancelled!\n", outputFileName); // error message
        return;
    }

    FILE* inputFile = fopen(fileName, "r"); // opens and reads the input file 
    if (!inputFile) { // if invalid
        fprintf(stderr, "Error! Failed to open file '%s'.\n", fileName); // error message
        return;
    }

    FILE* outputFile = fopen(outputFileName, "w"); // opens and writes the output file
    if (!outputFile) { // if invalid
        fprintf(stderr, "Error! Failed to open file '%s'.\n", outputFileName); // error message
        fclose(inputFile); // closes the input file
        return;
    }

    caesar(inputFile, outputFile, encrypt); // calls the caesar fucntion
    fclose(inputFile); // cloese input file
    fclose(outputFile); // closes output file
    printf("File '%s' has been %s and saved as '%s'.\n", fileName, encrypt ? "encrypted" : "decrypted", outputFileName); // success message
}

// this function uses the casesar cipher on the file contents to encrypt or decrypt
// @param inputFile is pointer to file for enc/dec, outputFile is pointer to output file, ecrypt is 1 to enc/ 0 to dec
void caesar(FILE* inputFile, FILE* outputFile, int encrypt) { 
    int ch; // char read from input file
    while ((ch = fgetc(inputFile)) != EOF) { // while loop to read characters till the end of the file
        if (isalpha(ch)) { // if character is alphabetical 
            char base = isupper(ch) ? 'A' : 'a'; // handles cases
            ch = base + (((ch - base) + (encrypt ? SHIFT : (26 - SHIFT))) % 26); // shifts character for caesar
        }
        fputc(ch, outputFile); // writes new characters to the output file 
    }
}

// the function to change the directory. @param path is the path to the directory to change to
void changeDirectory(const char* path) { 
    if (chdir(path) != 0) {
        perror("Error! Change directory failed! Please use format as 'change_directory <path>'"); // error message
    }
}

// the function to list the current directory contents. @param path is the path to the directory to list the contents of.
void listDirectory(const char* path) { 
    DIR *d;
    struct dirent *dir;
    d = opendir(path); // opens the entered directory and sets to d 
    if (d) { // if d is valid
        while ((dir = readdir(d)) != NULL) { // iterates through the directory contents
            printf("%s\n", dir->d_name); // and prints them
        }
        closedir(d); // closes the directory
    } else { // otherwise
        perror("Error! List directory failed! Please use format as 'list_directory <path>'"); // error message
    }
}

// function to create a new directory. allows cross-compatabiltiy for Windows or Unix/Linux/macOS.
// @param path is to where the new directory is added
void createDirectory(const char* path) { 
#ifdef _WIN32 // if Windows OS
    if (_mkdir(path) != 0) {
#else // if UNIX/Linux/macOS
    if (mkdir(path, 0755) != 0) {
#endif
        perror("Error! Create directory failed! Please use format as 'create_directory <path>'"); // error message
    }
}

// function to delete an empty directory. @param path is the path to the directory being deleted. 
void deleteDirectory(const char* path) { 
    if (rmdir(path) != 0) {
        perror("Error! Delete directory failed!  Please use format as 'delete_directory <path>'"); //error message
    }
}

// end of program