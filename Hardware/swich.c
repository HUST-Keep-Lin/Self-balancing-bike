#include "stm32f10x.h"
#include "swich.h"

//通过PB4输出5V电压，实现整体的控制
void Swich_Init(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE ); //配置复用时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE); //全功能SW+JTAG，禁用JTRST，PB4可用
	
	//默认输出低电平，灯亮，低电平输出
	//按下key按键输出高电平，灯灭，电机转动
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);


}
