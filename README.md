# OS
Tasks for course "Unix SVR4 System Calls and Libraries"
Solaris operating system is used.


1.     Output various process attributes according to specified options.
Write a program that will process command line options specified below.  Options must be processed according to their order from right to left.  The same option can occur several times.  Use getopt(3C) to parse command line. 
-i  Print real and effective ids of user and group.
-s  Process becomes a group leader. Hint: see setpgid(2).
-p  Print process id, parent process id and process group id.
-u  Print value of ulimit(2)
-Unew_ulimit  Change value of ulimit. Hint: see atol(3C) on man page strtol(3C)
-c  Print the size of a core file that can be created, in bytes.
-Csize  Change size of a core file.
-d  Print current working directory
-v  Print all environment variables and their values
-Vname=value  Create a new environment variable or change the name of an existing variable.

3.     Setting user id for file access
Create data file that is accessible for read and write only to its owner (it can be done by shell command chmod 600 file) and write a program which prints real and effective user ids.
Opens a file using fopen(3). If fopen() is successful, close a file with fclose(3). Print error message using  perror(3C) if the file cannot be opened.
Ensure that real and effective user ids are the same. Hint: setuid(2)
Repeat two first steps.
Test your program.
Run the program and observe the output
Make the program available for your group members and let some of your group members run the program.
By shell command chmod u+s prog set the setuid bit and let your group members run the program again.

9.     Process creation
Write a program that creates a child process. This process must execute cat(1) of a long file. Parent process must call printf(3) and print some text. Afrer finishing the first part, modify your program such that a last string. Printed by the parent was printed after child process finishes.  Use wait(2), waitid(2) or waitpid(3).

10.     Process exit code
Write a program that launches a command passed as the first argument as a child process. All other arguments of your program must be passed as arguments to the command.. Then program must wait for child process and print its exit code. 

18.     Directory listing
Write a program that works like ls -ld. For every its argument, this program must print file state bits in human-readable form:
d if the file is a directory
– if the file is a regular file
? in other cases
Three character groups corresponding to owner, group and other access rights:
r if the file is available for reading, otherwise -
w if the file is available for writing, otherwise –
x if the file is available for execution, otherwise -
Number of file links
Names of the owner and group of the file (advice - use getpwuid and getgrgid).
If the file is  regular file, its size. Otherwise leave this field empty.
File modification time (use ctime(3C)).
File name (if the name was given with the path, print only the name).

19.     File name templates
 Write a program that invites the user to enter a file name pattern similar to the one used in shell. The template syntax is:
* matches a sequence of any characters except / of any length; possibly an empty sequence.
? matches any single character.
/ cannot meet.
any other character matches itself.
Symbols * and ? the pattern can occur in any number and in any order.
The program should then find and print the names of all files in the current directory that match the pattern. If there are no such files, the program should print the template itself.

20.     File name templates (2)
Change the previous program so that the /characters can appear in the template. In this case, the program must print all files whose path names match the pattern. Thus, the pattern * / * matches all files in all subdirectories of the current directory.
