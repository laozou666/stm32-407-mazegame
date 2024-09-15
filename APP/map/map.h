#ifndef _map_H
#define _map_H

//#define N 2
#define M 2*30
#define BLOCK_SIZE 14 // 墙宽度
#define size_ghost 10 // 怪物步骤大小

void move_left(void);
void move_right(void);
void move_up(void);
void move_down(void);

void Initialize(int b);
int IsHaveNeighbor(int x, int y);
void Creat(int x, int y);
int Succeed(void);
void SetNewMap(int);
void print(void);
void move(void);

#endif
