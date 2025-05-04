#include "T_TenGine.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>

struct termios enableRAWmode(){
  struct termios oldConf;
  struct termios rawConf;
  
  tcgetattr(STDIN_FILENO, &oldConf);
  tcgetattr(STDIN_FILENO, &rawConf);

  rawConf.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(STDIN_FILENO,TCSANOW ,&rawConf);
  printf("\033[?25l");
  printf("\033[2J\033[H");
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  return oldConf;
}

struct termios escapeRAWmode(struct termios oldConf){
  struct termios rawConf;
  tcgetattr(STDIN_FILENO, &rawConf);

  tcsetattr(STDIN_FILENO,TCSANOW ,&oldConf);
  printf("\033[2J\033[H\033[?25h");
  
  return rawConf;
}

void clearScreen(){
  //printf("\033[2J\033[H");
  printf("\033[H");
  fflush(stdout);
}

void infoPrint(char *gameName, char key, int a, clock_t deltaTmicro){
  printf("somethinf button: %c\t%s\ttps %f\tfps: %.2f\n",key, gameName,
	 (float)a*deltaTmicro/1000000.0, 1000000.0/deltaTmicro);
  fflush(stdout);  
}


char *loadMap(char *filename){
  FILE *fp;
  char *map;
  long lSize;
  
  fp = fopen(filename,"rb");

  if(fp==0){
    perror("Couldn't open map file :(\n");
    exit(-1);
  }
  fseek(fp, 0, SEEK_END);
  lSize = ftell(fp);
  rewind(fp);

  map = calloc(1, lSize+1);

  if(fread(map, lSize, 1, fp)!=1){
    perror("reading map failed!");
    exit(1);
  }
  fclose(fp);

  return map;
}
