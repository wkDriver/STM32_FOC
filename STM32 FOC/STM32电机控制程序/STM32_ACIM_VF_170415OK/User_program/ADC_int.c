//############################################################
// FILE:  ADC_int.c
// Created on: 2017年1月5日
// Author: XQ
// summary: ADCSampPare
// ADC采样 ，使用定时器1中断函数出发ADC中断采样 
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
#include "ADC_int.h"
#include "GPIO_int.h"
#define ADC1_DR_Address    0x4001244C 

extern    ADCSamp     ADCSampPare;
uint16_t  ADC_ConvertedValue[5]={0};
 

void ADC1_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;	
    /* ADC1 Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* ADC1 DeInit */ 
    ADC_DeInit(ADC1);
 
    /* Initialize ADC structure */
    ADC_StructInit(&ADC_InitStructure);
		
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //连续转换开启
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 5;     //设置转换序列长度为2
    ADC_Init(ADC1, &ADC_InitStructure);
    
		RCC_ADCCLKConfig(RCC_PCLK2_Div4); // 72/2   
  
    //常规转换序列1：通道0    采样时间>1.6us,(7cycles)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
    //常规转换序列2：通道1   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5);
    //常规转换序列1：通道2
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_7Cycles5);
    //常规转换序列2：通道3   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_7Cycles5);
    //常规转换序列2：通道8  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_7Cycles5);
  
    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_DMACmd(ADC1, ENABLE);
    
    // 下面是ADC自动校准，开机后需执行一次，保证精度
    // Enable ADC1 reset calibaration register 
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));

    // Start ADC1 calibaration
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
    // ADC自动校准结束---------------
   //启动第一次AD转换
   ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
   //因为已经配置好了DMA，接下来AD自动连续转换，结果自动保存在RegularConvData_Tab处   
    
}


void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
   
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
 	
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;	 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //BufferSize=2，因为ADC转换序列有2个通道
    //如此设置，使序列1结果放在RegularConvData_Tab[0]，序列2结果放在RegularConvData_Tab[1]
    DMA_InitStructure.DMA_BufferSize =5;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //循环模式开启，Buffer写满后，自动回到初始地址开始传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    //配置完成后，启动DMA通道
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

 
 
//校准作用,电流传感器的偏移值为1.65V
void Offset_CurrentReading(void)
{
	static uint8_t i;  
 
  /* ADC Channel used for current reading are read  in order to get zero currents ADC values*/
  //16次采样求平均值，电流传感器初始校准   
for(i=16; i!=0; i--)   
  {
   //对于相电流和母线电流的电阻法测量电流,需要上电读取初始偏执电压
    ADCSampPare.OffsetBUS_Curr += ADC_ConvertedValue[0];
 	  ADCSampPare.OffsetPhaseV_Curr += ADC_ConvertedValue[1];
    ADCSampPare.OffsetPhaseU_Curr += ADC_ConvertedValue[2];		   
    Delay(10000);
		Delay(10000);
  }
  ADCSampPare.OffsetBUS_Curr = ADCSampPare.OffsetBUS_Curr>>4;  //求16次电压信号平均值
	ADCSampPare.OffsetPhaseV_Curr= ADCSampPare.OffsetPhaseV_Curr>>4; 
	ADCSampPare.OffsetPhaseU_Curr=ADCSampPare.OffsetPhaseU_Curr>>4;
}

void   ADC_Sample(void )
{
  ADCSampPare.BUS_Curr  =       (ADC_ConvertedValue[0]-ADCSampPare.OffsetBUS_Curr)<<1;	
 	ADCSampPare.PhaseV_Curr  =    (ADC_ConvertedValue[1]-ADCSampPare.OffsetPhaseV_Curr)<<1;	    
  ADCSampPare.PhaseU_Curr  =    (ADC_ConvertedValue[2]-ADCSampPare.OffsetPhaseU_Curr)<<1;	
  ADCSampPare.RP_speed_Voltage = ADC_ConvertedValue[3];	
  ADCSampPare.BUS_Voltage   = ADC_ConvertedValue[4];	
 

}


//===========================================================================
// No more.
//===========================================================================
