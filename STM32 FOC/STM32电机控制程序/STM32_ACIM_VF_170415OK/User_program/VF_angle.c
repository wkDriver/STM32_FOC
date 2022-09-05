//############################################################
// FILE: VF_angle.h
// Created on: 2017年1月15日
// Author: XQ
// summary: VF_angle
// VF角度
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//DSP/STM32电机控制开发板   
//硕历电子
//网址: https://shuolidianzi.taobao.com
//修改日期:2017/1/24
//版本：V17.3-1
//Author-QQ: 616264123
//电机控制QQ群：314306105
//############################################################ 
#include "VF_angle.h"
#include "Tim1_PWM.h"
#include "Timer.h"

extern   VF_Angle   VF_AnglePare;
extern   TaskTime   TaskTimePare;  
extern   int32_t    PWM_udq[3];
extern   uint16_t   Speed_Ref;
uint16_t VFp=25; // 电压和频率的比例 ，电压采用0---60000左右

void  VFAngle_init(void )
{
 VF_AnglePare.BASE_FREQ=135;   // 2000RPM  = n=60f/P= 135*60/4   电周期最大135      0---85MAX_angele
 VF_AnglePare.step_anglemax= (VF_AnglePare.BASE_FREQ<<16)/PWM_FREQ; //12500/VF_AnglePare.BASE_FREQ=92.5  1/92.5*2^16 IQ格式
 VF_AnglePare.step_Speed=3;  //频率的基本步进速度=3 
}

void VFAngle_cale(void)
{
   VF_AnglePare.Angle_theta+=VF_AnglePare.step_angle; //角度根据步进角度累加出三角波
   if( VF_AnglePare.Angle_theta>65536) // 限制角度 0--360 2pi    0---65536  
	 VF_AnglePare.Angle_theta-=65536;    //  
   else if( VF_AnglePare.Angle_theta<0)
   VF_AnglePare.Angle_theta+=65536; 
}

void VF_start_control(void)
{
    //VF 输入电压和频率	
		VF_AnglePare.SpeedRef= Speed_Ref ; //  
		VF_AnglePare.VF_Vq= VFp*VF_AnglePare.Speed_target ; // VFp电压与频率比值
	  if( VF_AnglePare.VF_Vq>=32000   )
    VF_AnglePare.VF_Vq=32000 ;
		
  	VF_AnglePare.Speederror= VF_AnglePare.SpeedRef-VF_AnglePare.Speed_target;

	    if(Abs(VF_AnglePare.Speederror)>= VF_AnglePare.step_Speed ) // VF 的斜率，转速的增增减减
	     {
			  if ( VF_AnglePare.SpeedRef>VF_AnglePare.Speed_target  )
			  VF_AnglePare.Speed_target+= VF_AnglePare.step_Speed ;
			  else if	( VF_AnglePare.SpeedRef<VF_AnglePare.Speed_target)
			  VF_AnglePare.Speed_target-= VF_AnglePare.step_Speed ;
			  else  
			  VF_AnglePare.Speed_target=VF_AnglePare.SpeedRef;
	     }
       else
       {
        VF_AnglePare.Speed_target=VF_AnglePare.SpeedRef;  
       }
	  VF_AnglePare.step_angle= (VF_AnglePare.step_anglemax*VF_AnglePare.Speed_target)/(VF_AnglePare.BASE_FREQ*60/4) ;  //（实际速度/最大转速）*最大速度的步进角度 771
}


//===========================================================================
// No more.
//===========================================================================
