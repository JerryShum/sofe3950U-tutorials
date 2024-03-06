#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// Create a program that does the following.
// - Forks and executes process using the fork and exec
// - Sleeps for 5 seconds
// - Sends the signal SIGINT to the process using kill
// - You should see the process running in the terminal printing to the screen
// it’s PID and other information with colour for five seconds and then the
// output will stop once it’s terminated.

#define SLEEP_TIME 5

int main(void){
    pid_t child_PID;

    //fork
    child_PID = fork();
    if(child_PID == -1){
        printf(stderr, "Fork Failed\n");
    }

    //child process
    if(child_PID == 0){
        int count = 0;

        //loop to print out seconds
        while(1) {
            printf("Child process PID: %d, Seconds passed: %d\n", getpid(), count);
            sleep(1);
            count++;
        }
        
    }else{
        //Parent Process
        sleep(SLEEP_TIME); //sleep 5

        //kill()
        kill(child_PID, SIGINT);
    }

    return 0;


}
