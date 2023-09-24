#include "stm32f10x.h"                  // Device header
#include "delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//下拉输入 按下高电平，松手低电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	//0代表低电平，1代表高电平，等于1说明按键按下
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
	{
		//delay消除抖动
		Delay_ms(20);
		//while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1);
		Delay_ms(20);
		KeyNum = 1;
	}
	
	return KeyNum;
}
