#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H


//串口清除
uint8_t UartRecv_Clear(void);
//接收标志函数，返回0说明没收据接收，返回1说明有数据收到
uint8_t Uart_RecvFlag(void);
//串口1在1字节接收完成回调函数，中断服务程序USART1_IRQHandler
void HAL_UART_RxCpltCallback(void);
//1ms调用一次，用来判断是否收完一帧
void UART_RecvDealwith(void);
//USART1管脚初始化
void GPIO_Config_Init(void);
void RCC_Configuration(void);
void USART_Config_Init(void);
void NVIC_Configuration2(void);

#endif
