#define __USER_H__

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	1000
void drawingboard();
int drawingbloack(int nowboard[10][20]);
void makeblock(int nowboard[10][20]);
int dropblock(int nowboard[10][20]);
int checkdrop(int nowboard[10][20]);
void deleteline(int nowboard[10][20]);
void control(int nowboard[10][20]);
void gameover(int nowboard[10][20]);
void gamemain();