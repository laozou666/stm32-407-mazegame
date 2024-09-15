#include "adc.h"
#include "SysTick.h"
#include "led.h"

/*******************************************************************************
* 函 数 名         : ADC_NVIC_Config
* 函数功能		  	 : ADC嵌套配置	
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
static void ADC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* 配置ADC1为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;//中断源的设置，不同的外设中断，中断源不一样，自然名字也不一样，所以名字不能写错，否则不会进入中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级设置，0为最高级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级设置
	
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函 数 名         : ADCx_Init
* 函数功能		  	 : ADC初始化	
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void ADCx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//端口时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC始终使能
	
	/*GPIO相关配置*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN; //模拟输入模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;//管脚设置
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;//浮空
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//速度为 100M
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化结构体
	
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//两个采样阶段之间的延迟20个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
	ADC_NVIC_Config();
		//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_56Cycles);	//ADC1,ADC通道,56个周期
	ADC_ITConfig(ADC1,ADC_IT_EOC, ENABLE);//开启转换结束中断
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器
	ADC_SoftwareStartConv(ADC1);//开始adc转换
}

