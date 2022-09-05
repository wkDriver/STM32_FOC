//############################################################
// FILE: Tim1_ISR_MCLoop.c
// Created on: 2017年1月15日
// Author: XQ
// summary: Tim1_ISR_MCLoop
// 定时器1电机控制， 中断环路闭环控制  
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
#include "Tim1_ISR_MCLoop.h"
#include "ADC_int.h"
#include "Tim1_PWM.h"
#include "GPIO_int.h"
#include "Axis_transform.h"
#include "Svpwm_dq.h"
#include "Task_function.h"
#include "PI_Cale.h"
#include "Timer.h"
#include "Usart_RS232.h"
#include "Sensorless_SMO.h" 
#include "Tim4_Encoder_PWMDAC.h"
 
extern  PI_Control   pi_spd ;
extern  PI_Control   pi_id  ;
extern  PI_Control   pi_iq  ;
extern   ADCSamp    ADCSampPare;
extern   uint16_t   PWM_DUTY[3] ;
extern   logic      logicContr;
extern   TaskTime   TaskTimePare;  
extern   CLARKE     ClarkeI;
extern   PARK       ParkI;
extern   IPARK      IparkU;
extern   SVPWM      Svpwmdq;
extern   IQSin_Cos  AngleSin_Cos;
extern   Test       TestPare;
extern   IQAtan     IQAtan_Pare;
extern   Angle_SMO   Angle_SMOPare ;
extern   Speed_est   Speed_estPare ;
extern   SMO_Motor   SMO_MotorPare ;

uint16_t   detha1=0,detha2=0 ;
uint16_t   FilK1=328;  
uint16_t   FilK2=696;

void TIM1_UP_IRQHandler(void)// 触发ADC中断采样和电机环路控制
{
	//此程序更新中断一直执行
	 TIM_ClearFlag(TIM1, TIM_FLAG_Update); //清除标志位
   
   
	 TaskTimePare.PWMZD_count++;
	
   if(TaskTimePare.PWMZD_count==25 ) // 2ms的事件任务
	 {
	   TaskTimePare.PWMZD_count=0;
	   
		 SMO_Speedcale();	  // 根据无感角度计算速度  
     knob_control();    //转把电位器调速的控制
 	
		 TestPare.id_test=  (pi_id.Fbk>>3 ); //通讯测试参数输出
	   TestPare.iq_test=  (pi_iq.Fbk>>3 );
	   TestPare.ud_test=  (IparkU.Ds>>3 );
	   TestPare.uq_test=  (IparkU.Qs>>3 );
// 测试参数输出  
     TestPare.fact_BUS_Voil= ADCSampPare.BUS_Voltage;
	   TestPare.fact_BUS_Curr= ADCSampPare.BUS_Curr;
	   TestPare.Speed_fact= ( pi_spd.Fbk>>3);
	   TestPare.Speed_target=( pi_spd.Ref>>3);	
  
	   if(( logicContr.Start_order==1)||( logicContr.Start_order==2))
	   {  
		    pi_spd.Fbk = Speed_estPare.Speed_RPM ;  //   0---4096RPM
	      PI_Controller((p_PI_Control)&pi_spd);   // 速度环PI控制 
	      pi_spd.OutF= _IQ10mpy(FilK1,pi_spd.OutF)+_IQ10mpy(FilK2,pi_spd.Out);			
     } 
   }
	
	  ADC_Sample( );  //相电流母线电流电压采集
 
  	ClarkeI.As=ADCSampPare.PhaseU_Curr;
	  ClarkeI.Bs=ADCSampPare.PhaseV_Curr;

	  CLARKE_Cale((p_CLARKE)&ClarkeI ); // CLARKE变换

	  ParkI.Alpha=ClarkeI.Alpha;
	  ParkI.Beta=ClarkeI.Beta;
 
	  if(logicContr.Start_order)  // 根据无感角度控制的速度做角度补偿
	  {
			 detha2= 15* pi_spd.Ref;		 
			 if(detha1>= detha2)
			 detha1-=12; 
			 if(detha1<= detha2)
			 detha1+=12; 
			 if( Abs( detha1-detha2 )<=8 )
			 detha1= detha2; 
    	IQAtan_Pare.JZIQAngle= IQAtan_Pare.IQAngle + detha1; //矫正无感角度相位偏差
			if( IQAtan_Pare.JZIQAngle >=65536)
		  IQAtan_Pare.JZIQAngle-=65536;
		  AngleSin_Cos.IQAngle = IQAtan_Pare.JZIQAngle;
	  }
 
	  IQSin_Cos_Cale((p_IQSin_Cos)&AngleSin_Cos);  //磁极位置角度，正余弦计算函数

		ParkI.Sine = AngleSin_Cos.IQSin;  
	  ParkI.Cosine =AngleSin_Cos.IQCos;   

	  PARK_Cale((p_PARK)&ParkI);   // PARK变换
	
    pi_id.Ref = 0;
	  pi_iq.Ref= pi_spd.OutF;

	  pi_id.Fbk = ParkI.Ds;
	  PI_Controller((p_PI_Control)&pi_id); // id轴 PI控制
	  pi_id.OutF= _IQ10mpy(FilK1,pi_id.OutF)+_IQ10mpy(FilK2,pi_id.Out);

	  pi_iq.Fbk = ParkI.Qs;
	  PI_Controller((p_PI_Control)&pi_iq); // iq轴 PI控制
	  pi_iq.OutF= _IQ10mpy(FilK1,pi_iq.OutF)+_IQ10mpy(FilK2,pi_iq.Out);
  
		if(logicContr.Start_order==1)   //   闭环电流 速度 开环
	 {
		IparkU.Ds=  pi_id.OutF ;    //    
	  IparkU.Qs= 19* pi_spd.Ref;  //
		 if( Speed_estPare.Speed_RPM>300)
		 {
		  logicContr.Start_order=2;			 		  
		 }
		 pi_spd.ui =19* pi_spd.Ref ;
	 }
	  if(logicContr.Start_order==2)   //  闭环电流 速度环路闭环
	 {
		IparkU.Ds= pi_id.OutF;    //     	 
		IparkU.Qs= pi_spd.OutF + 16*pi_spd.Ref ;   	 
	 }
     if(logicContr.Start_order==3)   //  采用双闭环  
	 {
		IparkU.Ds= pi_id.OutF;    //     
	  IparkU.Qs= pi_iq.OutF;  //   	 
	 }   
	 
   IparkU.Sine=AngleSin_Cos.IQSin;
   IparkU.Cosine=AngleSin_Cos.IQCos;
	 IPARK_Cale((p_IPARK)&IparkU);      // 反park变换

	  Angle_SMOPare.Ialpha = ClarkeI.Alpha; //滑膜观测器需要的四个参数ab轴的电压电流参数
	  Angle_SMOPare.Ibeta  = ClarkeI.Beta;
	  Angle_SMOPare.Valpha = Svpwmdq.Ualpha;
	  Angle_SMOPare.Vbeta  = Svpwmdq.Ubeta;

	  Angle_Cale((p_Angle_SMO)&Angle_SMOPare); //滑膜观测器 估算反电动势
    IQAtan_Pare.Alpha =-Angle_SMOPare.Ealpha ;
		IQAtan_Pare.Beta  =Angle_SMOPare.Ebeta;
	  IQAtan_Cale((p_IQAtan)&IQAtan_Pare); 	 //滑膜观测器 估算角度
			
	 Svpwmdq.Ualpha =IparkU.Alpha;
	 Svpwmdq.Ubeta = IparkU.Beta;

   SVPWM_Cale((p_SVPWM)&Svpwmdq);     //SVPWM计算占空比
 
	 Svpwm_Outpwm( );  //PWM输出
		 
   TIM_SetCompare3(TIM4 ,   PWM_DUTY[0]); // DAC通道1   
	 TIM_SetCompare4(TIM4 , (IQAtan_Pare.IQAngle)>>5); // DAC通道2  
} 


//===========================================================================
// No more.
//===========================================================================
