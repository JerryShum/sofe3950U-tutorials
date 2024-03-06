
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// - Forks and executes process using the fork and exec
// - Sleeps for 5 seconds
// - Sends the signal SIGTSTP to the process to suspend it
// - Sleeps for 10 seconds
// - Sends the signal SIGCONT to the process resuming it
// - Uses the waitpid function to wait until the process has terminated (about 5 seconds) before the main process exits.
// - You should see the process running in the terminal printing to the screen
// it’s PID and other information with colour for five seconds and then the
// output will stop for 10 seconds once it’s suspended and resume again until
// the program terminates

int main(void){
    pid_t PID;

    PID = fork();

    //child
    if(PID == 0 ){
        int count = 1;

        //loop to print out seconds and PID
        while(1) {
            printf(ANSI_COLOR_BLUE "Child process PID: %d, Seconds passed: %d\n" ANSI_COLOR_RESET , getpid(), count);
            sleep(1);
            count++;
        }

    }else{ //parent
        sleep(5); //sleep 5 seconds
        kill(PID,SIGTSTP);//SIGSTP suspend child
        sleep(10); //sleep 10 seconds
        kill(PID, SIGCONT); //SIGCONT to continue child
        sleep(5); //sleep 5
        kill(PID, SIGKILL); //kill child
        waitpid(PID, NULL, 0);
    }

    return 0;


}
