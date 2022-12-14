//Received help from Demetria Mack
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[256] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;



int main() {
    int buf = 256;
    int echoint = 1;
    int envint = 1;
    bool in_background = false;
    char *text;
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    size_t n = sizeof(arguments) / sizeof(arguments[0]);
    while (true) {
      
        do{ 
            // Print the shell prompt.
            getcwd(prompt, buf);
            printf("%s", prompt);
            fflush(stdout);

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
			  
			
        // 1. Tokenize the command line input (split it on whitespace)
        int i = 0;
        arguments[i] = strtok(command_line, delimiters);
        while(arguments[i] != NULL){
          printf("%s ", arguments[i]);
          i += 1;
          arguments[i] = strtok(NULL, delimiters);
        }

        if(strcmp(arguments[i-1], "&") == 0){
          in_background = true;
          arguments[i-1] = '\0';
        }

      
        // 2. Implement Built-In Commands
        if(strcmp(arguments[0], "cd") == 0){
          if(chdir(arguments[1]) != 0){
            printf("Directory Change to %s failed", arguments[1]);
          }
          else{
            if(arguments[1] == NULL){
              chdir(getenv("HOME"));
            }
            else{
              chdir(arguments[1]);
            }
          }
        }
        else if(strcmp(arguments[0], "pwd") == 0){
          printf("%s\n", prompt);
        }
        else if(strcmp(arguments[0], "echo") == 0){
          echoint = 1;
          while(arguments[echoint] != NULL){
            text = arguments[echoint];
            if(strstr(text, "$") != NULL){
              text += 1;
              text = getenv(text);
              printf("%s \n", text);
            }
            else{
              printf("%s ", arguments[echoint]);
            }
            echoint += 1;
          }
        }
        else if(strcmp(arguments[0], "exit") == 0){
          exit(1);
        }
        else if(strcmp(arguments[0], "env") == 0){
          envint = 1;
          printf("PATH : %s\n", getenv("PATH"));
          printf("HOME : %s\n", getenv("HOME"));
          printf("ROOT : %s\n", getenv("ROOT"));
          while(arguments[envint] != NULL){
            text = arguments[envint];
            if(strstr(text, "$") != NULL){
              text += 1;
            }
            printf("%s : ", getenv(text));
            envint += 1;
          }
        }
        else if(strcmp(arguments[0], "setenv") == 0){
          if(arguments[1] != NULL && arguments[2] != NULL){
            setenv(arguments[1], arguments[2], 1);
          }
          else{
            printf("SETENV Error: Specified Parameters Missing\n");
          }
        }
        else{
        // 3. Create a child process which will execute the command line input
        pid_t pid;
        pid_t child_pid;
        int status;
        pid = fork();
        if(pid < 0){
          printf("Error in forking\n");
        }
        else if(pid == 0){
          if(execvp(arguments[0], arguments) == -1){
            perror("execvp() fail:\n");
            printf("Error\n");
          }
          exit(0);
        }
        else{
          child_pid = pid;
          alarm(10);
          if(!in_background){
            wait(NULL);
          }
        }
  
        // 4. The parent process should wait for the child to complete unless its a background process
      
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
    }
}