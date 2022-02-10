#include stdio.h
#include stdlib.h
#include sys/types.h
#include signals.h
#include unistd.h

int pitch_signal = 0;
int pitch_type = 0;

void pitch_recieved(int signum){
  pitch_signal = 1;
  if(signum == SIGUSR1){
    pitch_type = 1;
  if(signum == SIGUSR2){
    pitch_type = 2;}}
}

int main( int argc, char * argv[]){
  int fielder;
  int status;
  int hand = 1; // 1 = right 0 = left

  int batter_thing = 0;
  int opt;
  while((opt = getopt(argc, argv, "rl")) != -1){
    switch (opt){
      case 'r':
        hand = 1;
        break;
      case 'l':
      hand = 0;
        batter_thing = 1;
        break;
      default :
      break;
    }}
  
  printf( "Batter attribute is: %i\n", batter_thing);

  fielder = atoi( argv[optind]);

  printf("Batter PID is: %i\n fielder is: %i \n", getpid(), fielder);

  struct sigaction SA = {0};
  SA.sa_handler = & pitch_recieved;
  status = sigaction( SIGUSR1, &SA, 0);
  if( status == -1){
    perror("Proble registering SIGUSR1"); exit(3);}
  status = sigaction(SIGUSR2, &SA, 0);
  

   while(1){
     pause();
     if(pitch_signal == 1){
       printf(" Pitch received [%i] batter does something \n", pi);
       if((hand == 1) && (pitch_type == 1)){
         printf(" Fly ball to fielder");
         kill( SIGUSR1, fielder);
       }
       if((hand == 0) && (pitch_type == 1)){
         printf("Fly ball to fielder");
         kill(SIGUSR2, fielder);

       }
       if((hand == 1) && (pitch_type == 2)){
         printf(" Ground Ball to PID");
         kill(SIGUSR1, fielder);

       }
      if((hand == 0) && (pitch_type == 2)){
        printf("Ground ball to fielder");
        kill(SIGUSR2, fielder);

      }  
       kill(fielder, SIGUSR2);
       pitch_signal = 0;
       pitch_type = 0;
     }

   }
}
