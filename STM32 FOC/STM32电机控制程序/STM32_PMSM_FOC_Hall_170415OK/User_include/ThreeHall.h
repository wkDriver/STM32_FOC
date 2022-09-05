//############################################################
// FILE:  ThreeHall.h
// Created on: 2017年1月5日
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

#ifndef ThreeHall_H
#define ThreeHall_H
#include "stm32f10x.h"
#include "IQ_math.h"

typedef struct {
	    uint8_t       HallUVW[3];   // 读取三个霍尔的对应状态
	    uint8_t       Hall_State;   // 当前霍尔状态
	    uint8_t       OldHall_State; // 历史霍尔状态
	    uint8_t       HallLX_State;  // 当前和历史霍尔状态联接一个字节数据
	    uint8_t       Hall_num[8];    // 八种霍尔状态
	    int32_t       Hall_angle[8];  // 八个霍尔角度根据不同状态
			int32_t       step_angle_error; //步进角度误差
	    int32_t       step_angle ;      //步进角度 
			int32_t       step_angleFitter ;//步进角度 滤波值
	    uint16_t      Speed_count;      //速度计数值
	    uint16_t      Speed_countFitter; //速度计数滤波值
      uint16_t      Speed_count_old;   //速度计数历史值
			uint32_t      speed_coeff;       //速度系数
	    uint8_t       Poles;             //电机极对数
	    uint8_t       Move_State;        //电机旋转状态
      int32_t       initial_angle;     //电机初始角度
      int32_t       angleIQ;           //霍尔计算电机角度
      uint16_t      Speed_RPM;         //电机旋转速度 
      int32_t       Speed_ele_angleIQ; //速度电角度值（计算速度）
      int32_t       Speed_ele_angleIQFitter; //速度电角度值（计算速度）
      int32_t       old_ele_angleIQ;   // 电机历史电角度
      int32_t       ele_angleIQ;       // 电机电角度
	   } Hall;

#define  Hall_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0} // 初始化参数

void ThreeHallPara_init(void);  //三霍尔计算角度参数初始化
void ThreeHallanglecale(void);  //三霍尔计算角度函数
void Hall_Three_Speedcale(void); //三霍尔角度计算速度函数
#endif /* ThreeHall_H_*/
//===========================================================================
// End of file.
//===========================================================================
