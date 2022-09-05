//############################################################
// FILE: Tim4_Encoder_PWMDAC.c
// Created on: 2016年1月15日
// Author: XQ
// summary: Tim4_Encoder_PWMDAC
//   正交编码器的定时器4   
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

#include "Tim4_Encoder_PWMDAC.h"
#include "stm32f10x_exti.h" 
 
extern  EQEP   EQEPPare; 
  
void TIM4_Encoder_Config(void)//定时器4初始化的编码器配置
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4
	
	TIM_DeInit(TIM4);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
  TIM_TimeBaseStructure.TIM_Period =(uint16_t) (SystemCoreClock / 18000) - 1;   // 5000计数 
  TIM_TimeBaseStructure.TIM_Prescaler =0;	    //设置预分频：
  TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	/*初始化TIM4定时器 */
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/*-----------------------------------------------------------------*/
	//编码配置                        编码模式
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  //TIM_ICPolarity_Rising上升沿捕获
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;         //比较滤波器
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
	TIM_Cmd(TIM4, ENABLE);   //使能定时器4
}
/*
	    int32_t  ElecThetaYS;      // 编码器电机位置角度电角度 0---1024 0--360
	    int32_t  ElecTheta;        // 电机的磁极电角度 0---65536 0--360   Electrical angle
	    int32_t  MechTheta;        // 编码器电机位置机械角度 0--4096   Mechanical Angle	   
	    int32_t  JZElecTheta;      // 矫正电机磁极位置角度 
	    uint16_t Poles;            //   Number of pole pairs  	  
	    int32_t  initial_angle;    //  电机初始位置角度   
      int32_t  Oid_JZElecTheta;  //  矫正后历史电机磁极位置角度 
      int32_t	 Speed_ele_angleIQ;//用于速度计算的电角度
	    int32_t  Speed_ele_angleIQFitter;// 滤波后速度电角度
		  uint16_t Speed_RPM;     // 电机速度RPM  		 
	    uint32_t speed_coeff;  // 计算速度的系数 
    	uint16_t SpeedK1;    // 速度计算一阶低通滤波器系数1
	    uint16_t SpeedK2;    // 速度计算一阶低通滤波器系数2
*/
void  QEPEncoder_Cale(void)
{ 
  EQEPPare.MechTheta = TIM_GetCounter(TIM4);  // 读编码器机械位置信号
  EQEPPare.ElecThetaYS = EQEPPare.MechTheta %1000; // 计算4对级的电机电角度
  EQEPPare.ElecTheta = EQEPPare.ElecThetaYS<<6;
  EQEPPare.JZElecTheta= EQEPPare.ElecTheta+ EQEPPare.initial_angle; // 矫正电机位置
	
	 if(   EQEPPare.JZElecTheta> 65536)
     EQEPPare.JZElecTheta-=65536;
   else if( EQEPPare.JZElecTheta<0)
     EQEPPare.JZElecTheta+=65536;	
}

void QEPEncoder_Speedcale(void)  // 2ms执行一次 速度计算
{  
 	 EQEPPare.Speed_ele_angleIQ =EQEPPare.Oid_JZElecTheta -EQEPPare.JZElecTheta;
   if( EQEPPare.Speed_ele_angleIQ < 0)
   EQEPPare.Speed_ele_angleIQ+=65536; 	 
 
 	 EQEPPare.Speed_ele_angleIQFitter= _IQ10mpy(EQEPPare.SpeedK1, EQEPPare.Speed_ele_angleIQFitter)+_IQ10mpy(EQEPPare.SpeedK2,  EQEPPare.Speed_ele_angleIQ);
   EQEPPare.Speed_RPM = (EQEPPare.Speed_ele_angleIQ*EQEPPare.speed_coeff)>>16; // 最大角度 2pi是一圈 65536
	 EQEPPare.Oid_JZElecTheta = EQEPPare.JZElecTheta ;
}

void  EQEP_Pare_init( void )
{
	// 初始角度如果启动不起来，或者母线电流过大，可以将电机和编码器连接位置一点点改动再试试
	// 初始角度如果启动不起来，或者母线电流过大，也可以改变一点点initial_angle值，以1000为单位在线调试加或者减
	EQEPPare.initial_angle=22180;   //初始位置角度，需要将编码器与电机连接
	EQEPPare.SpeedK1=355;   //滤波器系数，经验值
	EQEPPare.SpeedK2=669;	
	EQEPPare.Poles=4;
	EQEPPare.speed_coeff=(1000*60)/(2*EQEPPare.Poles ); // 2毫秒计算一次角度差值 1000/2ms=500   =7500	 
}


void EXTIX_Init(void)
{
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
 
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  
}

void EXTI9_5_IRQHandler(void) // Z信号外部中断清除计数器
{	
  TIM4->CNT = 0; // 0---4000  清除计数
  TIM_Cmd(TIM4, ENABLE);  
	EXTI_ClearITPendingBit(EXTI_Line5);  //清除EXTI0线路挂起位
}


//===========================================================================
// No more.
//===========================================================================
