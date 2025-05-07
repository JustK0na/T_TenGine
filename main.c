#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include "T_TenGine.h"


#define FRAMERATE 30
#define MICROSECONDS 1000000


typedef struct ball_t{
  float posx, posy, speedx, speedy;
  char sprite;
}ball_t;


void createBall(ball_t *ball, float x, float y, char sprite){
  ball->posx = x;
  ball->posy = y;
  ball->speedx=0.12;
  ball->speedy=0.06;
  ball->sprite = sprite;
}
void updateBall(ball_t *ball, map_t *map,int deltaTmacro){
  ball->posx = ball->posx + ball->speedx;
  ball->posy = ball->posy + ball->speedy;
  if(ball->posx >= map->col)
    ball->posx = map->col-1;

  if(map->tiles[rounding((ball->posy + ball->speedy))][rounding((ball->posx + ball->speedx))] == '#')
    ball->speedx*= -1;
  if(map->tiles[rounding(ball->posy + ball->speedy)][rounding(ball->posx + ball->speedx)] == '&')
    ball->speedy *= -1;



}
void drawBall(ball_t *ball, map_t *map){
  printf("\033[%d;%dH\033[%d;%df", (int)ball->posy+MARGIN, (int)ball->posx, (int)ball->posy+MARGIN, (int)ball->posx);
  printf("\033[1;38;5;99m%c\033[0m",ball->sprite);
}

void update(map_t *map, ball_t *ball, int deltaTmacro){
  updateBall(ball, map, deltaTmacro);
}

void draw(map_t *map, ball_t *ball){
  drawMap(map);
  drawBall(ball, map);
  fflush(stdout);
}

int main() {
  
  struct termios oldConf = enableRAWmode();
  clock_t nowClock, pastClock, deltaTmacro=0;
  char key;
  char gameName[32]="Pong";
  map_t map;
  ball_t ball;
  int tick=0;

  loadMap("map.T_Tmap", &map);
  createBall(&ball, 7, 1, '@');
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
      update(&map, &ball, deltaTmacro);
      draw(&map, &ball);

      tick=0;
    }

  }
  
  escapeRAWmode(oldConf);

  return 0;
}
//Compile with:
//gcc -Wall -pedantic -o demo main.c T_TenGine.c