//############################################################
// FILE: VF_angle.h
// Created on: 2017年1月19日
// Author: XQ
// summary:  Header file  and definition
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

#ifndef  VF_angle_H
#define  VF_angle_H
 
#include "IQ_math.h"
#include "stm32f10x.h"

typedef struct {
	uint16_t      Freq_T_Ref;    //开环根据固定霍尔状态启动频率
	uint16_t      Voilage_Ref;   //开环给定参考电压
	uint16_t      VF_huanxCount; //换相计数
	uint16_t      VF_Count;      //VF计算
	uint16_t      VF_state;      //VF换相状态
	uint16_t      VF_num[6];     //VF霍尔换相顺序
	uint16_t      OldVF_State;   //历史VF换相状态 
	uint16_t      initial_state;  //VF初始状态定位
	  }VF_start;

#define  VF_start_DEFAULTS   {0,0,0,0,0,0,0,0} // 初始化参数


void  VF_start_control(void); //无感启动的VF固定启动
void  VF_start_init(void );   // VF固定启动参数初始化

#endif /* VF_angle_H*/
