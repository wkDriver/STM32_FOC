//############################################################
// FILE:  Usart_RS232.c
// Created on: 2017年1月18日
// Author: XQ
// summary: SCI_RS232
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//DSP/STM32电机控制开发板   
//硕历电子
//网址: https://shuolidianzi.taobao.com
//修改日期:2017/3/23
//版本：V17.3-1
//Author-QQ: 616264123
//电机控制QQ群：314306105
//############################################################

#include "Usart_RS232.h"
#include "Timer.h"
#include "Task_function.h"

extern  Test           TestPare;
extern  TaskTime       TaskTimePare;
extern  logic          logicContr;

void Usart3_RS232_init(void)
{
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel =  USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
 
  USART_DeInit(USART3);
  
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init( USART3, &USART_InitStructure);
    
  USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
  USART_Cmd( USART3, ENABLE);  
}

void Uart3_Sendlen(uint8_t *data,u8 len)
{
	USART3->SR;
	do{
		USART_SendData(USART3,*data);
		while(!(USART3->SR&0x40));
		data++;  
		len--;
	}while(len!=0);
}
 
void  Uart3_RS232TX_sen(void)
{     
  if(TaskTimePare.Tim1s_flag==1)
  {
   	TestPare.Uart_txdr[0] = 0xaa ; // 打包数据帧头aa 
   	TestPare.Uart_txdr[1] = TestPare.fact_BUS_Voil>>8; //其他位数据用户可以自行根据需求定义
   	TestPare.Uart_txdr[2] = TestPare.fact_BUS_Voil  ;  //
   	TestPare.Uart_txdr[3] = TestPare.Speed_target>>8 ; //
   	TestPare.Uart_txdr[4] = TestPare.Speed_target  ;   //
   	TestPare.Uart_txdr[5]=  logicContr.Run_mode ;    //
   	TestPare.Uart_txdr[6] = logicContr.Start_order ;
   	TestPare.Uart_txdr[7] = 0xcc ;  // 打包数据帧尾cc	
  
		Uart3_Sendlen(TestPare.Uart_txdr,8);   	 
 	}
}
 
void  ReceiveData_chuli(void)  // 串口通讯中断接收数据的处理函数
{
  uint8_t i=0; 
 if( TestPare.Rece_flag==1)  // 接收数据中断里的标志位
 {
	 TestPare.Rece_flag=0;
   for(i=0;i<8;i++   )
   TestPare.Uart_rxdr[i]  = TestPare.Uart_rxdr[i];
 }
 
}

void USART3_IRQHandler(void)  //接收程序的帧同步，超时判断。
{
        
  if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET
         || USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET) //????(?????????0x0d 0x0a??)
    {        
		 if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
			{
				USART_ReceiveData(USART3 );
				USART_ClearFlag(USART3, USART_FLAG_ORE);      
			}  
			TestPare.chaoshi_pand=TestPare.chaoshi_jishu; 
			TestPare.chaoshi_shu = TestPare.chaoshi_pand-TestPare.chaoshi_pandold;           
			TestPare.chaoshi_pandold=TestPare.chaoshi_pand;
			if( TestPare.chaoshi_shu >=3) 
			{
			TestPare.Rxdr_byte=0;             
			} 
			 
			TestPare.Uart_rxdr[TestPare.Rxdr_byte]=USART_ReceiveData(USART3) ;                                
			TestPare.Rxdr_byte ++;  
			 
			if( TestPare.Rxdr_byte>=8 )		
			{
			TestPare.Rece_flag=1; // 接收数据标志位，作为主程序的协议处理执行标志
			TestPare.Rxdr_byte=0; // 清除接收数据位  
			}           
    }            
} 


//===========================================================================
// No more.
//===========================================================================
