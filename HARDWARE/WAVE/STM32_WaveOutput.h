#ifndef  __STM32_WaveOutput_H
#define  __STM32_WaveOutput_H

#include "sys.h"
#include "math.h"
#define PI  3.1415926
#define Vref 3.2		//0.1~3.3V可调
#define Um  (Vref/2)
#define N 250
void SineWave_Init(u32 Wave1_Fre);
void Set_WaveFre( u32 fre);
/*******************用到的资源************************/
//GPIOA_Pin_4
//定时器6  //输出触发模式
//DMA2通道3	    //循环发送模式
//DAC通道1       //定时器触发模式
#endif

