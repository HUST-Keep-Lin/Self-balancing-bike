#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	//RCC开启时钟，将我们需要的TIM外设和GPIO外设时钟打开，选择内部时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//初始化PA1引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用开漏推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//初始化时基单元
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR 周期
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC 预分频器
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	//配置输出比较（output compare）模块
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); //给结构体赋初始值
	//设置输出比较模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//设置输出比较极性  有效点平为高电平
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR的值 500-2500
	
	//使用结构体初始化输出比较单元  使用TIM2的OC2 CH2通道 PA1输出
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	//启动定时器
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);
}
