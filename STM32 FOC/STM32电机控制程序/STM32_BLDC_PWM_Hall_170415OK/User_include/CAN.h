//############################################################
// FILE: CAN.h
// Created on: 2017年1月18日
// Author: XQ
// summary: ThreeHall
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

#ifndef __CAN_H
#define __CAN_H 
 
#include "stm32f10x.h"
#include "stm32f10x_can.h"

typedef struct {
        uint32_t   ext_Rece_ID;	    // 接收CAN通讯的ID号
        uint32_t   ext_Send_ID;     // 发送CAN通讯的ID号
        uint8_t    Can_Send_data[8];// CAN通讯发送数据
        uint8_t    Can_Rece_data[8];// CAN通讯接收数据
        uint8_t    CAN_rx_flag;     //CAN通讯接收标志位
	   }CANSR;

#define  CANSR_DEFAULTS  {0x18FA0170,0x18FA0171,0,0,0}  // 初始化参数

void CAN_Receivechuli(void);  // CAN接收数据处理函数
void CAN_Config(void);        // CAN通讯配置初始化
void CAN_Sendlen(void);       // CAN发送函数
void USB_LP_CAN1_RX0_IRQHandler(void); //CAN中断接收函数

#endif /* __CAN_H */
