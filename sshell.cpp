#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE        80 /* 80 chars per line, per command */
int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* max of 40 arguments */       
    int should_run = 1; /* controls the running of this program */     
    int argc = 0;

    std::vector<std::string> user_input; /* sequence container to encapsulate user input */
    
    while(should_run == 1){

        int process_status = 1; /* status to run processes */
        
        std::string input;

        printf("osh>"); /* command line prompt */

        std::getline(std::cin, input); /* utilizes std getline() to get user input */
        std::istringstream iss(input); /* tokenizing - breaking up a sequence of strings into pieces */
        std::string input_after_space; /*user argument after input space */

        while (getline( iss, input_after_space, ' ' )) { /* space delimiter */

            if(input_after_space == "exit" || input_after_space == "quit"){ /* exits out of shell */

                printf("Program Exited.");  
                should_run = 0;
            }
            
            if(!input_after_space.empty()){

                if(input[0] == '!'){

                    continue;
                }

                else{

                    user_input.push_back(input_after_space);
                    argc++;
                }
            }
        }

        std::string arr[argc]; /* array for argument count */
        std::copy(user_input.begin(),  user_input.end(), arr); /* clone vector into string array */
        
        if(input != " ") {
            
            if(input[0] == '!' && input[1] != '!'){ /* child process for input commands */
                
                int num = 0; 
                std::string hold = &input[1]; /* for number after ! */
                num = std::stoi(hold.c_str()); /*convert string to int */
                num--;
                args[0] = const_cast<char*>(user_input.begin()[num].c_str());
            }

            else{ /* user input to run */
                
                args[0]= const_cast<char*>(user_input.back().c_str());
            }
            
            if(process_status == 1 && (should_run == 1)){ /* child and parent processes start */
                
                args[argc] = NULL; /*last argument is set to NULL */
                pid_t pid = fork(); /*create child process by using fork()*/
                
                if (pid < 0) { /* pid is negative sends error message*/

                    std::cout << "error: negative pid" << std::endl;
                    return 1;
                }

                else if (pid == 0) { /* child process */

                    execvp(args[0], args); /* execvp - replaces the current process image with a new process image created by the fork call*/
                    return 0;
                }

                else { /* parent process here waits for child process first*/

                    int child_status;
                    waitpid(pid, &child_status, 0);
                    should_run = 1;
                }
            }
        }
    }
    
    return 0;
}