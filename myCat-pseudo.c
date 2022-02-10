//pseudo code for my cat
//Paragraph 3b
//reads file name in command line
//outputs the contents of file in command line

//Paragraph 3c
//Descriptor IO
//needs to read a single file and outputs
//the content to standard output
#include "fcntl.h"
#include "stdio.h"
#include "unistd.h"

#define BUFLEN 2
int read_and_write(int in, int out){
  char buff[BUFLEN];
  int c;
  c = read(in, buff, BUFLEN);
  while(c){
    write(out, buff, c);
    c = read(in, buff, BUFLEN);
  }
}
int main(int argc, char * argv){
  int fin, fout;
  fin = open("test33", O_RDONLY, S_IRUSR);
  fout = open("testout.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if((fin < 0) || (fout < 0)){
    printf("ERROR with a file\n" );
    return -1;
  }
  read_and_write( fin, fout);
  close(fin);
  close(fout);
}
