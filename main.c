#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include "T_TenGine.h"


#define FRAMERATE 12
#define MICROSECONDS 1000000


typedef struct ball{
  int pox;
  int posy;
}ball;

void update(char *map, float *position, int deltaTmacro){

  *position +=  10000.0f/deltaTmacro;

  printf("\n\n%f\t%f\n\n", *position, 10000.0f/deltaTmacro);

  if(map[(int)*position]!='\n')
    map[(int)*position]='!';
  
  /*
  if(map[tick%504]!='\n')
    map[tick%504]='!';*/
}
void draw(char *map){
    size_t mapSize = strlen(map);
  for(int i=0; i<=mapSize; i++){
    if(map[i]=='!')
      printf("\033[1;31m%c\033[0m",map[i]);
    else
    printf("%c", map[i]);
  }
  //printf("%s", map);
  

  fflush(stdout);
}

int main() {
  
  struct termios oldConf = enableRAWmode();
  clock_t nowClock, pastClock, deltaTmacro;
  char key;
  char gameName[32]="Game Name";
  char *map=NULL;
  int tick=0;
  float position;
    
  map=loadMap("map.txt");
  pastClock = clock();
  
  while(1){
    
    nowClock = clock();
    read(STDIN_FILENO, &key, 1);

    if(key=='q')
      break;

    deltaTmacro = MICROSECONDS*(nowClock - pastClock)/CLOCKS_PER_SEC;
    tick++;
    
    if(deltaTmacro>(float)MICROSECONDS/FRAMERATE){
      
      pastClock = clock();
      clearScreen();
      infoPrint(gameName, key, tick, deltaTmacro);
      update(map, &position, deltaTmacro);
      draw(map);

      tick=0;
    }

  }
  
  escapeRAWmode(oldConf);

  return 0;
}
//Compile with:
//gcc -Wall -pedantic -o demo main.c T_TenGine.c
