Henry Wesson
SD 247
3/15/2024
Project 4: Terminal in C

This program works as a Terminal coded in C. It accepts user input, runs system commands, and features several custom commands, including a secure_file command that allows the user to encrypt/decrypt files using a caesar cipher. This program is able to be used on both Windows and Unix/Linux/macOS operating systems. It keeps a history of the last 10 commands entered and the user can rerun commands from the list using the index number of the command. This program also features effective error handling so make sure it doesn't crash in certain situations and with improper user input. It also provides detailed feedback for the user when an error occurs.

Alogrithms that standout in particular are the caesar cipher using the secure_file command. 
The caesar cipher has a shift amount of 3 so the each letter is shifted foward by 3 (A would become D for example) during encryption.
During decryption, this process is reversed and the letters are shifted back by 3. 
The caesar cipher only effects letters and checks each character to make sure it is one. 
It also wraps around if at the beginning or end of the alphabet (Z would shift to C for example) during encrpytion.

  and the other custom comands of change_directory, list_directory, create_directory, and delete_ direcory that can work cross-platform.

To test this program I extensively tested different system and custom commands to insure they worked.
I also made sure that the files were properly  encrypted and decrypted by the caesar cipher with the secure_file command. 
I tested the command history to make sure it properly displayed the last 10 commands and updated correctly.
I tested the !n command to rerun commands from the history. 
I made sure this program worked on my windows and mac computers to ensure cross-compatability.
I debugged using print statements to confirm the commands were being parsed and running properly.
I also used the VScode debugger to check for memory leaks related to the command history. 

Overall, I enjoyed working on this program and it was a great learning experience for using C as a language.
I would like to improve the cipher for this in the future to make the program more secure, I also would like to move away from using system() and try do this with entirely custom commands because I know system() is considered bad practice in a real world setting. 
I also would like to add a "help" command in the future that doesn't rely on the system commands.  


Instructions: 

To complile this program,

1. First make sure GCC or a similar comliler is installed on your system.

2. Use the command,

 cd "C:<path to file>" 

to navigate to the =directory where Terminal.c is located in the VScode Terminal, powershull, or a similar program.

3. Now enter,

gcc Terminal.c -o Terminal -std=c99, to compile

4. Now enter,

./terminal, to run the program

to run the program. You will be prompted with a ">" to enter commands. 

5. Once in the terminal program you can run any windows system commands such as dir, copy, move, del, type nul > filename, md, rd, findstr, type, etc...
Or if you are using Unix/Linux/macOS then use commands such as ls, cp, mv, rm, mkdir, rmdir, pwd, touch, cd, etc...


You can also enter several custom commands such as, 

secure_file -e -f <filename>, to encrypt a file
secure_file -d -f <filename>, to decrypt a file

history, accesses the command history
!n, reruns a command from the command history where n is the command's index in the history list

change_dirctory C:<path to file>, changes the directory

list_directory C:<path to file>, changes the directory

create_directory C:<path ending in the name of the new directory>, creates a new directory

delete_directory C:<path to file>, deletes the directory

'quit' or 'exit' will end the program





 
