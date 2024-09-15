/*******************************************************************************
*
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : 外部中断实验
* 实验说明       :
* 连接方式       :
* 注    意		 : 外部中断驱动程序在exti.c内
*******************************************************************************/

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "map.h"
#include "tftlcd.h"
#include "usart.h"
#include "time.h"
#include "stdlib.h"

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

int map[M + 1][M + 1];
int SHOW_SIZE = 23; // 显示比迷宫大小大1

//游戏界面显示
extern void game_show(u8 size)
{
	u8 i = 0, j = 0;
	map[1][0] = 2;//2是入口
	map[size - 2][size - 1] = 3;// 出口
	FRONT_COLOR = BLACK;
	LED2 = !LED2;
	//LCD_ShowFontHZ_Ex((tftlcd_data.width-4*32)/2,10,"迷宫游戏",22);
	//LCD_ShowString(100,40,100,100,24,"Level:");
	//LCD_ShowNum(100+6*12,40,game_level,1,24);
 
	//绘制界面，墙、路、玩家
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (map[i][j] == 0)//绘制墙
			{
				//LCD_DrawRectangle(i*BLOCK_SIZE,j*BLOCK_SIZE,i*BLOCK_SIZE+BLOCK_SIZE,j*BLOCK_SIZE+BLOCK_SIZE);
				LCD_Fill(i * BLOCK_SIZE, j * BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, BLACK);
			}
			else if (map[i][j] == 2) {
				LCD_Fill(i * BLOCK_SIZE, j * BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, WHITE);
			}
			else if (map[i][j] == 3) {
				LCD_Fill(i * BLOCK_SIZE, j * BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, WHITE);
			}
			// else if(map[game_level-1][i][k]==PLAYER)//绘制玩家
			// {
			// 	LCD_ShowPicture(START_X+k*X_SIZE,START_Y+(i)*Y_SIZE,X_SIZE,Y_SIZE,(u8 *)gImage_wanjia);
			// }
			else if (map[i][j] == 5) {
				LCD_Fill(i * BLOCK_SIZE, j * BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, WHITE);
			}
			else if (map[i][j] == 6) {
				LCD_Fill(i * BLOCK_SIZE, j * BLOCK_SIZE, i * BLOCK_SIZE + 16, j * BLOCK_SIZE + 16, RED);
			}
			else if (map[i][j] == 7) {
				LCD_Fill(i * BLOCK_SIZE, j * BLOCK_SIZE, i * BLOCK_SIZE + 16, j * BLOCK_SIZE + 16, YELLOW);
			}
			else if (map[i][j] == 8) {
				LCD_Draw_Circle(i * BLOCK_SIZE+BLOCK_SIZE/2, j * BLOCK_SIZE +BLOCK_SIZE/2 ,4);
			}
		}
	}
	LCD_Fill(1*BLOCK_SIZE, 1*BLOCK_SIZE, 1*BLOCK_SIZE+BLOCK_SIZE, 1*BLOCK_SIZE+BLOCK_SIZE, RED);
}

void sys_open_show(void)
{
	FRONT_COLOR = BLACK;
	// LCD_ShowFontHZ_Ex((tftlcd_data.width-4*32)/2,10,"迷宫游戏",22);
	// LCD_ShowPicture((tftlcd_data.width-240)/2,(tftlcd_data.height-240)/2,240,240,(u8 *)gImage_picture);
	LCD_ShowString(80, 80, tftlcd_data.width, tftlcd_data.height, 24, "PRESS KEY_UP TO START");


	while (1)
	{
		if (KEY_Scan(0) == KEY_UP_PRESS)
		{
			LCD_Clear(BACK_COLOR);
			game_show(SHOW_SIZE);
			break;
		}
	}
}

int main()
{
	u8 key;
	// srand(time(NULL));
	SysTick_Init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	USART1_Init(115200);
	LED_Init();
	TFTLCD_Init();
	RNG_Init();
	KEY_Init();
	BEEP_Init();
	// My_EXTI_Init();  //外部中断初始化
	// Initialize(22);
	// IsHaveNeighbor(1, 1);
	// Creat(1, 1);
	SetNewMap(SHOW_SIZE - 1);
	
	sys_open_show();
	Ghost_line();
	//LCD_ShowString(80,80,tftlcd_data.width,tftlcd_data.height,24,"begin");
	// LCD_DrawRectangle(1, 1, 20, 20);
	// LCD_DrawRectangle(460, 1, 480, 20);
	game_show(SHOW_SIZE);
	while (1)
	{
		delay_ms(150);
		LED1 = 0;
		key = KEY_Scan(0);   //扫描按键
		//Move_ghost();
		Automove_ghost();
		game_show(SHOW_SIZE);
		switch (key)
		{
		case KEY_UP_PRESS:
			LED1 = 1;
			// LCD_Clear(BACK_COLOR);
			move_up();
			game_show(SHOW_SIZE);
			break;
		case KEY0_PRESS:
			LED1 = 1;
			// LCD_Clear(BACK_COLOR);
			move_right();
			game_show(SHOW_SIZE);
			break;
		case KEY1_PRESS:
			LED1 = 1;
			// LCD_Clear(BACK_COLOR);
			move_down();
			game_show(SHOW_SIZE);
			break;
		case KEY2_PRESS:
			LED1 = 1;
			// LCD_Clear(BACK_COLOR);
			move_left();
			game_show(SHOW_SIZE);
			break;
		case 0:
			LED1 = 1;
			break;
		}
		if (Succeed())
		{
			LCD_ShowString(80, 180, tftlcd_data.width, tftlcd_data.height, 24, "You Succeeded!\nPress KEY_UP to continue.");
			SHOW_SIZE += 2 ;
			while (1)
			{
				if (KEY_Scan(0) == KEY_UP_PRESS)
				{
					SetNewMap(SHOW_SIZE - 1);
					LCD_Clear(BACK_COLOR);
					game_show(SHOW_SIZE);
					break;
				}
			}

		}
	}

}


