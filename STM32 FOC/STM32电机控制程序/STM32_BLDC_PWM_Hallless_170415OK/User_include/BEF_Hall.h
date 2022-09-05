//############################################################
// FILE:  BEF_Hall.h
// Created on: 2017年1月15日
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

#ifndef  BEF_Hall_H
#define  BEF_Hall_H
#include "stm32f10x.h"
#include "IQ_math.h"
 
typedef struct {
	    uint8_t       HallUVW[3];  // 读取三个反电动势霍尔的对应状态
	    uint8_t       Hall_State;  //当前反电动势霍尔状态 
	    uint8_t       OldHall_State;  //历史反电动势霍尔状态
			int32_t       step_angle_error; //步进角度误差
	    int32_t       step_angle ;   //步进角度 
			int32_t       step_angleFitter ;  //步进角度 滤波值
	    uint16_t      Speed_count;  //速度计数值
	    uint16_t      Speed_countFitter;  //速度计数滤波值
      uint16_t      Speed_count_old;  //速度计数历史值
			uint32_t      speed_coeff;  //速度系数
	    uint8_t       Poles;   //电机极对数
	    uint8_t       Move_State;   //电机旋转状态
      uint16_t      Speed_RPM;    //电机旋转速度 
      uint16_t      Speed_RPMF;	  //电机旋转滤波速度 
	   } Hall;

#define  Hall_DEFAULTS {0,0,0,0,0,0,0,0,0,0,4,0,0,0}  // 初始化参数


#define   Hall_num1   0x5  // VF根据固定霍尔状态换相顺序状态
#define   Hall_num2   0x1
#define   Hall_num3   0x3
#define   Hall_num4   0x2
#define   Hall_num5   0x6
#define   Hall_num6   0x4

#define   BEFCAP_numZ1  0x2    // 0x4  根据反电动势状态正转换相顺序状态
#define   BEFCAP_numZ2  0x6    // 0x5
#define   BEFCAP_numZ3  0x4     //0x1
#define   BEFCAP_numZ4  0x5    // 0x3
#define   BEFCAP_numZ5  0x1    // 0x2
#define   BEFCAP_numZ6  0x3     //0x6

#define   BEFCAP_numF1  0x4    // 0x4  根据反电动势状态反转换相顺序状态
#define   BEFCAP_numF2  0x5    // 0x5
#define   BEFCAP_numF3  0x1    // 0x1
#define   BEFCAP_numF4  0x3    // 0x3
#define   BEFCAP_numF5  0x2    // 0x2
#define   BEFCAP_numF6  0x6    // 0x6	
 
void BEF_ThreeCAPPara_init(void );  //根据反电动势状态参数初始化
void BEFHall_huanxkz(void);   //根据反电动势状态换相 
#endif /* ThreeHall_H_*/
//===========================================================================
// End of file.
//===========================================================================
