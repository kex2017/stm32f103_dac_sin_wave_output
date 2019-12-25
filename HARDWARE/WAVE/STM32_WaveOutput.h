#ifndef  __STM32_WaveOutput_H
#define  __STM32_WaveOutput_H

#include "sys.h"
#include "math.h"
#define PI  3.1415926
#define Vref 3.2		//0.1~3.3V�ɵ�
#define Um  (Vref/2)
#define N 250
void SineWave_Init(u32 Wave1_Fre);
void Set_WaveFre( u32 fre);
/*******************�õ�����Դ************************/
//GPIOA_Pin_4
//��ʱ��6  //�������ģʽ
//DMA2ͨ��3	    //ѭ������ģʽ
//DACͨ��1       //��ʱ������ģʽ
#endif

