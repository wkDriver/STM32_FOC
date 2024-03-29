 //############################################################
// FILE:  PI_Cale.c
// Created on: 2017年1月4日
// Author: XQ
// summary: PI_Cale
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

#include "PI_Cale.h"

extern  PI_Control   pi_spd ;



void  PI_Controller(p_PI_Control  pV)
{
    /* proportional term */
	pV->up = pV->Ref - pV->Fbk;

	/* integral term */
	pV->ui = (pV->Out == pV->v1)?(_IQmpy(pV->Ki, pV->up)+ pV->i1) : pV->i1; // 判断积分饱和
	pV->i1 = pV->ui;

	/* control output */
	pV->v1 = _IQmpy(pV->Kp, (pV->up )) + pV->ui;	 
	pV->Out=IQsat( pV->v1, pV->Umax,  pV->Umin); //限制输出
}

void  PI_Pare_init(void )
{
  pi_spd.Kp=18000;     // _ 
  pi_spd.Ki=260;  //    T* 环路周期 /0.2
  pi_spd.Umax =30000;  //  
  pi_spd.Umin =0;

 }

//===========================================================================
// No more.
//===========================================================================

