#include "exti.h"
#include "led.h"
#include "SysTick.h"
#include "key.h"
#include "beep.h"
#include "map.h"

/*******************************************************************************
* 函 数 名         : My_EXTI_Init
* 函数功能		   : 外部中断初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void game_show(u8);

void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);


	//EXTI0 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//EXTI2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//EXTI3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//EXTI4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器


	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}

/*******************************************************************************
* 函 数 名         : EXTI0_IRQHandler
* 函数功能		   : 外部中断0函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == 1)
	{
		delay_ms(10);
		if (KEY_UP == 1)
		{
			// LED2 = 0;
			move_up();
			game_show(SHOW_SIZE);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

/*******************************************************************************
* 函 数 名         : EXTI2_IRQHandler
* 函数功能		   : 外部中断2函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2) == 1)
	{
		delay_ms(10);
		if (KEY2 == 0)
		{
			//BEEP = 1;
			move_left();
			game_show(SHOW_SIZE);
		}

	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/*******************************************************************************
* 函 数 名         : EXTI3_IRQHandler
* 函数功能		   : 外部中断3函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) == 1)
	{
		delay_ms(10);
		if (KEY1 == 0)
		{
			// LED2 = 1;
			move_down();
			game_show(SHOW_SIZE);
		}

	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/*******************************************************************************
* 函 数 名         : EXTI4_IRQHandler
* 函数功能		   : 外部中断4函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4) == 1)
	{
		delay_ms(10);
		if (KEY0 == 0)
		{
			//BEEP = 0;
			move_right();
			game_show(SHOW_SIZE);
		}

	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}


