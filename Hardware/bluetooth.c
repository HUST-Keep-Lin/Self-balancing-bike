#include "stm32f10x.h"   
#include "string.h"

volatile uint8_t UartRxData;
uint8_t UartTxbuf[512]={1,2,3,4,5,6,7,8,9,10};//发送缓冲
uint8_t UartRxbuf[512],UartIntRxbuf[512];//接收缓冲
uint16_t UartRxIndex=0,UartRxFlag,UartRxLen=0,UartRxTimer,UartRxOKFlag,UartIntRxLen;

//串口清除
uint8_t UartRecv_Clear(void)
{
	UartRxOKFlag=0;
	UartRxLen=0;
	UartIntRxLen=0;
	UartRxIndex=0;
	return 1;
}

//接收标志函数，返回0说明没收据接收，返回1说明有数据收到
uint8_t Uart_RecvFlag(void)
{
		if(UartRxOKFlag==0x55)
		{
			UartRxOKFlag=0;
			UartRxLen=UartIntRxLen;
			memcpy(UartRxbuf,UartIntRxbuf,UartIntRxLen);//把缓冲区的数据，放入需要解析的数组
			UartIntRxLen=0;
			return 1;
		}
		return 0;
}
//串口1在1字节接收完成回调函数，中断服务程序USART1_IRQHandler
void HAL_UART_RxCpltCallback(void)
{
	

		UartRxFlag=0x55;//接收标志置位
		UartIntRxbuf[UartRxIndex]= USART_ReceiveData(USART1);//数据写入缓冲区
		UartRxIndex++;//记载数目加1
		if(UartRxIndex>=512)//缓冲区是512字节，如果存满，归零
		{
			UartRxIndex=0;
		}
		  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//接收中断打开


}

//1ms调用一次，用来判断是否收完一帧
void UART_RecvDealwith(void)
{
	if(UartRxFlag==0x55)
	{
		if(UartIntRxLen<UartRxIndex)//UartIntRxLen小于UartRxIndex，说明有收到新的数据，把接收长度增加
		{
		UartIntRxLen=UartRxIndex;
		}else
		{
			UartRxTimer++;
			if(UartRxTimer>=50)//50ms,等待，没收到新数据，说明已经收完一帧
			{
				UartRxTimer=0;
				UartRxFlag=0;
				UartRxOKFlag=0x55;
				UartRxIndex=0;
			}
		}
	}
}

//USART1管脚初始化
void GPIO_Config_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure USART1 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USART1 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void RCC_Configuration(void)
{
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  /* Enable USART1 Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
}

void USART_Config_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  /* Enable USARTy Receive  interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
}

void NVIC_Configuration2(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

