#include "map.h"
#include<stdio.h>
//#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include "rng.h"
#include "SysTick.h"

int border;//地图大小
int random_x,random_y;//ghost初始位置
int current_y = 1, current_x = 1;
int line[size_ghost] = {0};// 怪物路线
int flag_ghost = 1;//判断怪物正走和反走的标志
int i_ghost = 0; //怪物专用，不要重定义了 
extern int map[M + 1][M + 1];
int bullet_direction;//  子弹方向0：上  1：下 2：右 3：左
int bullet_x , bullet_y; //  子弹位置
int flag_bullet = 1 ; //  是否能发射子弹
int temp_dir;
int shoot = 0; //1发射子弹 0不发射

void move_up(void)
{
	if (map[current_x][current_y - 1] == 5 || map[current_x][current_y - 1] == 3)
	{
		map[current_x][current_y] = 5;
		current_y -= 1;
		map[current_x][current_y] = 6;
	}
}

void move_down(void)
{
	if (map[current_x][current_y + 1] == 5 || map[current_x][current_y + 1] == 3)
	{
		map[current_x][current_y] = 5;
		current_y += 1;
		map[current_x][current_y] = 6;
	}
}

void move_left(void)
{
	if (map[current_x - 1][current_y] == 5 || map[current_x - 1][current_y] == 3)
	{
		map[current_x][current_y] = 5;
		current_x -= 1;
		map[current_x][current_y] = 6;
	}
}

void move_right(void)
{
	if (map[current_x + 1][current_y] == 5 || map[current_x + 1][current_y] == 3)
	{
		map[current_x][current_y] = 5;
		current_x += 1;
		map[current_x][current_y] = 6;
	}
}

/*
void menu()
{
	char select[10];
	printf("输入地图大小（从4到58的偶数)：");
	scanf("%d", &border);
}
*/

void Initialize(int b)
{
	int i = 0, j = 0;
	//重新初始化map为0数组
	int i_ =0,j_ = 0;
	border = b;
	for (i_ = 0;  i_< M+1; i_++) {
		for (j_ = 0; j_ < M+1; j_++) {
			map[i_][j_] = 0;
		}
	}
	//map[M + 1][M + 1] = { 0 };
	
	for (i = 0; i < border; i++) {
		for (j = 0; j < border; j++) {
			if ((i % 2) && (j % 2)) {
				map[i][j] = 1;
			}
		}
	}
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			if (i > border || j > border) {
				map[i][j] = -1;
			}
		}
	}

}
int IsHaveNeighbor(int x_index, int y_index)
{
	if ((x_index >= 3 && map[x_index - 2][y_index] == 1) || (x_index <= border - 2 && map[x_index + 2][y_index] == 1) || (y_index >= 3 && map[x_index][y_index - 2] == 1) || (y_index <= border - 2 && map[x_index][y_index + 2] == 1)) {
		return 1;
	}
	else return 0;
}
void Creat(int x_index, int y_index) {
	u32 random;
	int rand_position, x, y, flag = 0;
	x = x_index;
	y = y_index;
	while (1)
	{
		flag = 0;
		flag = IsHaveNeighbor(x_index, y_index);
		if (flag == 0) {
			return;
		}
		else {
			map[x_index][y_index] = 5;
			x = x_index;
			y = y_index;
			while (1) {
				//rand_position = rand() % 4;//随机得到一个方向0：上  1：下 2：右 3：左
				random=RNG_Get_RandomRange(0,3);
				rand_position = random;
				if (rand_position == 0 && x_index >= 3 && map[x_index - 2][y_index] == 1) {
					x_index = x_index - 2;
				}
				else if (rand_position == 1 && x_index <= border && map[x_index + 2][y_index] == 1) {
					x_index = x_index + 2;
				}
				else if (rand_position == 2 && y_index <= border - 2 && map[x_index][y_index + 2] == 1) {
					y_index = y_index + 2;
				}
				else if (rand_position == 3 && y_index >= 3 && map[x_index][y_index - 2] == 1) {
					y_index -= 2;
				}
				else {
					continue;
				}
				map[(x + x_index) / 2][(y + y_index) / 2] = 5;//5代表路
				map[x_index][y_index] = 5;
				Creat(x_index, y_index);
				break;
			}
		}
	}
}
void Move_ghost(){
	int ghost_rand_position;

		//rand_position = rand() % 4;//随机得到一个方向0：上  1：下 2：右 3：左
		ghost_rand_position=RNG_Get_RandomRange(0,3);
		//rand_position = random;
	if (ghost_rand_position == 0 &&(map[random_x][random_y - 1] == 5 || map[random_x][random_y - 1] == 6))
		{
			map[random_x][random_y] = 5;
			random_y -= 1;
			map[random_x][random_y] = 7;
			
		}
		else if (ghost_rand_position == 1 &&(map[random_x][random_y + 1] == 5 || map[random_x][random_y + 1] == 6)) {
			map[random_x][random_y] = 5;
			random_y += 1;
			map[random_x][random_y] = 7;
			
		}
		else if (ghost_rand_position == 2 &&(map[random_x+1][random_y ] == 5 || map[random_x+1][random_y ] == 6)) {
			map[random_x][random_y] = 5;
			random_x += 1;
			map[random_x][random_y] = 7;
			
		}
		else if (ghost_rand_position == 3 &&(map[random_x-1][random_y ] == 5 || map[random_x-1][random_y ] == 6)) {
			map[random_x][random_y] = 5;
			random_x -= 1;
			map[random_x][random_y] = 7;
		}

}
//按照固定路线移动
void Automove_ghost(){
	if (i_ghost <  0){
		i_ghost = 0;
		flag_ghost = 1;
	}
	if (i_ghost > 9){
		i_ghost = 9;
		flag_ghost = 2;
	}
    if (flag_ghost == 1 && i_ghost < 10 && i_ghost>=0){
       // 0：上  1：下 2：左 3：右
		if (line[i_ghost] == 0 &&(map[random_x][random_y - 1] == 5 || map[random_x][random_y - 1] == 6))
		{
			map[random_x][random_y] = 5;
			random_y -= 1;
			map[random_x][random_y] = 7;
			
		}
		else if (line[i_ghost] == 1 &&(map[random_x][random_y + 1] == 5 || map[random_x][random_y + 1] == 6)) {
			map[random_x][random_y] = 5;
			random_y += 1;
			map[random_x][random_y] = 7;
			
		}
		else if (line[i_ghost] == 2 &&(map[random_x-1][random_y ] == 5 || map[random_x-1][random_y ] == 6)) {
			map[random_x][random_y] = 5;
			random_x -= 1;
			map[random_x][random_y] = 7;
			
		}
		else if (line[i_ghost] == 3 &&(map[random_x+1][random_y ] == 5 || map[random_x+1][random_y ] == 6)) {
			map[random_x][random_y] = 5;
			random_x += 1;
			map[random_x][random_y] = 7;
		}
		i_ghost++;
	}
	//反方向走  0：下  1：上 2：右 3：左
	else if(flag_ghost == 2 && i_ghost < 10 && i_ghost >= 0){
		if (line[i_ghost] == 0 &&(map[random_x][random_y + 1] == 5 || map[random_x][random_y + 1] == 6))
		{
			map[random_x][random_y] = 5;
			random_y += 1;
			map[random_x][random_y] = 7;
			
		}
		else if (line[i_ghost] == 1 &&(map[random_x][random_y - 1] == 5 || map[random_x][random_y - 1] == 6)) {
			map[random_x][random_y] = 5;
			random_y -= 1;
			map[random_x][random_y] = 7;
			
		}
		else if (line[i_ghost] == 2 &&(map[random_x+1][random_y ] == 5 || map[random_x+1][random_y ] == 6)) {
			map[random_x][random_y] = 5;
			random_x += 1;
			map[random_x][random_y] = 7;
			
		}
		else if (line[i_ghost] == 3 &&(map[random_x-1][random_y ] == 5 || map[random_x-1][random_y ] == 6)) {
			map[random_x][random_y] = 5;
			random_x -= 1;
			map[random_x][random_y] = 7;
		}
		i_ghost--;
	}
}
//固定怪物路线
void Ghost_line(){
	int temp_x = random_x; 
	int temp_y = random_y;
	int i = 0;
	int ghost_rand_position;
	int ii , jj;
	int temp_map[40][40] = {0};
	//int line[i];
	for (ii = 0; ii < 40; ii++) {
		for (jj = 0; jj < 40; jj++) {
			temp_map[ii][jj] = map[ii][jj];
		}
	}
	 while(1){
		//rand_position = rand() % 4;//随机得到一个方向0：上  1：下 2：左 3：右
			ghost_rand_position=RNG_Get_RandomRange(0,3);
				//rand_position = random;
			if (ghost_rand_position == 0 &&temp_map[temp_x][temp_y - 1] != 7&&temp_map[temp_x][temp_y - 1] == 5)
		{
			temp_map[temp_x][temp_y] = 7;
			temp_y -= 1;
			temp_map[temp_x][temp_y] = 7;
			line[i] = ghost_rand_position;
			i++;

		}
		else if (ghost_rand_position == 1&&temp_map[temp_x][temp_y + 1] != 7 &&temp_map[temp_x][temp_y + 1] == 5) {
			temp_map[temp_x][temp_y] = 7;
			temp_y += 1;
			temp_map[temp_x][temp_y] = 7;
			line[i] = ghost_rand_position;
			i++;
		}
		else if (ghost_rand_position == 2&&temp_map[temp_x-1][temp_y] != 7 &&temp_map[temp_x-1][temp_y ] == 5) {
			temp_map[temp_x][temp_y] = 7;
			temp_x -= 1;
			temp_map[temp_x][temp_y] = 7;
			line[i] = ghost_rand_position;
			i++;
		}
		else if (ghost_rand_position == 3 &&temp_map[temp_x+1][temp_y] != 7&&temp_map[temp_x+1][temp_y ] == 5) {
			temp_map[temp_x][temp_y] = 7;
			temp_x += 1;
			temp_map[temp_x][temp_y] = 7;
			line[i] = ghost_rand_position;
			i++;
		}

		else continue;
		if(i>=10){
			break;
		}
    }
	// for (ii = 0; ii < M+1; ii++) {
	// 	for (jj = 0; jj < M+1; jj++) {
	// 		if(temp_map[ii][jj] == 7)
	// 		{
    //              temp_map[ii][jj] = 5;
	// 		}
			
	// 	}
	// }
}
void Bullet(){
	if (shoot == 0){
		return ;
	}
	if (flag_bullet == 1){   //1能发射子弹 
	bullet_x = current_x;
	bullet_y = current_y;
	temp_dir = bullet_direction;
	}
	else if(flag_bullet == 0){
		map[bullet_x][bullet-y] == 5 ;
	}
	//随机得到一个方向0：上  1：下 2：左 3：右
    switch (temp_dir)
		{
		case 0:
			// LCD_Clear(BACK_COLOR);
            if (map[bullet_x][bullet_y-1] == 5 || map[bullet_x][bullet_y-1] == 3){
				map[bullet_x][bullet_y-- ] = 8;  
				flag_bullet = 0;               
			}
			else 
			{
				flag_bullet = 1;
				shoot = 0;
			}
			break;
		case 1:
			// LCD_Clear(BACK_COLOR);
            if (map[bullet_x][bullet_y+1] == 5 && map[bullet_x][bullet_y+1] == 3){
				map[bullet_x][bullet_y++  ] = 8;  
				flag_bullet = 0;               
			}
						else 
			{
				flag_bullet = 1;
				shoot = 0;
			}
			break;
		case 2:
			// LCD_Clear(BACK_COLOR);
            if (map[bullet_x - 1][bullet_y] == 5 && map[bullet_x - 1][bullet_y] == 3){
				map[bullet_x --][bullet_y] = 8;  
				flag_bullet = 0;               
			}
						else 
			{
				flag_bullet = 1;
				shoot = 0;
			}
			break;
		case 3:
			// LCD_Clear(BACK_COLOR);
            if (map[bullet_x + 1][bullet_y] == 5 && map[bullet_x + 1][bullet_y] == 3){
				map[bullet_x ++][bullet_y] = 8;  
				flag_bullet = 0;               
			}
						else 
			{
				flag_bullet = 1;
				shoot = 0;
			}
			break;
	    default:
		    break;
    }
}
int Succeed()
{
	if (map[current_x][current_y] == 3)
	{
		return 1;
	}
	return 0;
}

void SetNewMap(int b)
{
	Initialize(b);
	IsHaveNeighbor(1, 1);
	Creat(1, 1);
	current_x = 1;
	current_y = 1;
	//ghost生成的初始位置
		while(1){
		random_x=RNG_Get_RandomRange(4, border);
	    random_y=RNG_Get_RandomRange(4, border);
		if (map[random_x][random_y] == 5){
			break;
		}
	}
    map[random_x][random_y] = 7; 
}

/*
void print() {
	system("cls");
	map[1][1] = 2;
	map[border - 1][border - 1] = 3;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {

			if (map[i][j] == 0) {
				printf("国");
			}
			else if (map[i][j] == -1) {
				break;
			}
			else if (map[i][j] == 5) {
				printf("  ");
			}
			else if (map[i][j] == 2) {
				printf("▼ ");

			}
			else if (map[i][j] == 6) {
				printf("b ");

			}
			else if (map[i][j] == 3) {
				printf("▲ ");
			}
		}if (map[i][0] == -1) {
			break;
		}
		printf("\n");

	}
}
*/

/*
void move() {
	while (1)
	{
		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 0xE0) // 检测到特殊键
			{
				ch = _getch(); // 获取实际的键值
				if (ch == 75) {
					move_left();
				}
				else if (ch == 77) {
					move_right();
				}
				else if (ch == 72) {
					move_up();
				}
				else if (ch == 80) {
					move_down();
				}
				print();
				if (map[current_y][current_x] == 3)
				{
					break;
				}
			}
		}
	}
}

int main0()
{
	menu();
	Initialize(border);
	IsHaveNeighbor(1, 1);
	Creat(1, 1);
	//for (int i = 0; i < M; i++) {
	//	for (int j = 0; j < M; j++) {
	//		printf("%2d", map[i][j]);
	//	}printf("\n");
	//}
	print();
	move();
	return 0;
}

*/
