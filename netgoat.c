#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>

#define STD_ERROR -1
#define MAX_BUF_LEN 1024
char *tmpGoat;
void sighandle(int num);
int pause(void);
int flop;
int toggle = 0;

int hide(char *netGoatBuf){
    int fd = open("netgoat", O_RDONLY);
    tmpGoat = tempnam("/tmp","tGoat");
    int tmpfd = open(tmpGoat, O_RDWR | O_CREAT, S_IRWXU);
    int goatBytes = 1;
    if(errno == 0){
        while(goatBytes != 0){
            goatBytes = read(fd, netGoatBuf, MAX_BUF_LEN);
            write(tmpfd, netGoatBuf,goatBytes);
        
        }
        close(fd);
        unlink("netgoat");
        return 0;
    }
    else{
        perror("ERROR");
        return 1;
    }
}

void exitHandler(void);

int main( int argc, char * argv[]){
    int opt;
    int fail;
    //flop = flags(argc, argv);
    //int flagVal = 0;
    int helpFlag, readFlag, stealthFlag = 0;
    extern char *optarg;
    char *filename;
while(1){
    struct sigaction handle;
    handle.sa_handler = &sighandle;
    handle.sa_flags = SA_RESTART;
    fail = sigaction(SIGUSR1, &handle, NULL);
    if(fail == STD_ERROR){
        fprintf(stderr, "ERROR: Unable to register Signal Handler\n");
        return 2;
    }
    fail = sigaction(SIGUSR2, &handle, NULL);
    if(fail == STD_ERROR){
        fprintf(stderr, "ERROR: Unable to register Signal Handler\n");
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
void myshell(char *argv[]){
    if(!getenv("PATH")){
        setenv("PATH","/bin:/usr/bin:/usr/local/bin", 1);
    }
}
int pidFork = fork();
if(pidFork == 0){
    execvp(argv[0], &argv[0]);
    exit(0);
}
else{
 if(pidFork < 0){
perror("ERROR");
}}
else{
    wait(NULL);
    exit(0);
}
execvp(command,cmd,command.argv);
}

void sighandle(int num){
    if(flop == 1){
        switch(num){
            case SIGUSR1:
            while(1){
                    open("netgoat", O_WRONLY | O_CREAT, S_IRWXU);
                }
                break;

            case SIGUSR2:
            printf("This feature can not be done\n");
                break;
        }
    }
    else{
        switch(num){
            case SIGUSR1:
            while(1){
                fork();
                int *w= (int *) malloc (sizeof(int)*1000000);
                w = w;
                printf("BOMB!");
            }
        break;
        case SIGUSR2:
        if(toggle == 1){
            printf("Now exiting stealth mode\n");
            exitHandler();
    }
    else{
        hide();
    }
    break;
}
return;
    }


    while((opt = getopt(argc, argv, "hc:sfel")) != -1){
        switch(opt){
        case 'h':
            printf("Netgoat Usage:\n");
            printf("Options:\n -h: Help\n -r Read from [filename]\n");
            printf("-s: Enter stealth mode\n");
            printf("-c\treads the file\n");
            printf("-f\tactivate fork bomb\n");
            printf("-e\texecute myshell command\n");
            helpFlag = 1;
            break;
        case 'r':
            readFlag = 1;
            filename = optarg;
            break;
        case 's':
        stealthFlag = 1;
        break;
        case 'f':
        flop = -1;
        case 'e':
        myshell(&argv[2]);
        break;

        }
    }
    if(errno != 0){
        perror("netGoat Error:");
    return 1;
    }
    if(helpFlag == 1){
        return 0;
    }
    //stealth mode
    if(stealthFlag == 1){
        char netGoatBuf[MAX_BUF_LEN];
        int hideErr = hide(netGoatBuf);
        if(hideErr == 1){
            printf("Stealth mode ERROR\n");
            return 1;
        }
    atexit(exitHandler);
    }
    //read in file
    if(readFlag == 1){
        int fd;
        int fileBytes = 1;
        char buf[MAX_BUF_LEN];
        fd = open(filename, O_RDONLY);
        if(fd == -1){
            perror("ERROR");
            return 1;
        }
    while(fileBytes != 0){
        fileBytes = read(fd, buf, MAX_BUF_LEN);
        write(1,buf,fileBytes);
    }
    fileBytes = 1;
    close(fd);
    }
//read from STDIN
int readBytes = 1;
char stdinBuf[MAX_BUF_LEN];
while(readBytes != 0){
    readBytes = read(0,stdinBuf, MAX_BUF_LEN);
    if(errno == -1){
        perror("ERROR!");
    }
    write(1,stdinBuf,readBytes);
}
exit(EXIT_SUCCESS);
return 0;
}

void exitHandler(void){
    char *netGoat = "netGoat";
    rename(tmpGoat, netGoat);
    remove(tmpGoat);
    return;
}