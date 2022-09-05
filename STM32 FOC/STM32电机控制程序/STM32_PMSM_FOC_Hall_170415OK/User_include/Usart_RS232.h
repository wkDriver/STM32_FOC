//############################################################
// FILE:   Usart_RS232.h
// Created on: 2017年1月21日
// Author: XQ
// summary: Header file  and definition
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//DSP/STM32电机控制开发板
//硕历电子
//网址: https://shuolidianzi.taobao.com
//修改日期:2017/1/23
//版本：V17.3-1
//Author-QQ: 616264123
//电机控制QQ群：314306105
//############################################################
 
#ifndef Usart_RS232_H
#define Usart_RS232_H
#include "IQ_math.h"
#include "stm32f10x.h"

typedef struct {		     
        uint8_t   Uart_txdr[8];  //串口发送数据
        uint8_t   Uart_rxdr[8];  //串口接收数据
	      uint16_t  fact_BUS_Curr; // 实际母线电流
	      uint16_t  fact_BUS_Voil; // 实际母线电压
		    uint16_t  iq_test;       // q轴测试电流
	      uint16_t  id_test;       // d轴测试电流
	      uint16_t  uq_test;       // q轴测试电压
	      uint16_t  ud_test;       // d轴测试电压
	      int16_t   IqRef_test;    // d轴给定参数电流
	      uint16_t  Speed_target;  // 目标转速
	      uint16_t  Speed_fact;    // 实际转速
        uint8_t   Rece_flag;     // 接收标志  
	      uint8_t   Rxdr_byte;     // 接收字节数
        uint8_t   chaoshi_jishu;  //超时判断计数
        uint8_t   chaoshi_pand;   //超时判断时间
        uint8_t   chaoshi_pandold;//历史超时判断时间
        uint8_t   chaoshi_shu;    //超时判断数
	   }Test;

#define  Test_DEFAULTS  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // 初始化参数
 
void Uart3_RS232TX_sen(void); //RS232发送函数
void Usart3_RS232_init(void); //RS232串口初始化函数
void Uart2_Sendlen(uint8_t *data,u8 len); //发送一个字节函数
void USART3_IRQHandler(void);   //中断接收函数
void  ReceiveData_chuli(void);  //接收数据处理函数
#endif  // end of SCI_RS232.h definition

//===========================================================================
// End of file.
//===========================================================================
