#include "STM32_WaveOutput.h"	
#include "delay.h"	
#include "usart.h"
/******DAC�Ĵ�����ַ����*******/	
#define DAC_DHR12R1    (u32)&(DAC->DHR12R1)   //DACͨ��1����Ĵ�����ַ
u16 SineWave_Value[N];
/********�������Ҳ��������***********/
void SineWave_Data( u16 cycle ,u16 *D)
{
	u16 i;
	for(i=0;i<cycle;i++)
	{
		D[i]=(u16)((Um*sin(( 1.0*i/(cycle-1))*2*PI)+Um)*4095/3.3);
	}
}
/****************���ų�ʼ��******************/
void SineWave_GPIO_Config()
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	GPIOA->CRL&=0xFFF0FFFF;//A4��ʼ��
	GPIOA->CRL&=0x00030000;
	GPIOA->ODR|=(1<<4);
}

/******************DAC��ʼ��*************************/
void SineWave_DAC_Config()
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	  	
	RCC->APB1ENR|=1<<29;   //ʹ��DACʱ��	  
	
	GPIOA->CRL&=0XFFF0FFFF; 
	GPIOA->CRL|=0X00000000;//PA4 ģ������ 
	
	DAC->CR|=1<<0;	//ʹ��DACͨ��1
	DAC->CR|=1<<1;	//DAC1������治ʹ�� BOFF1=1
	DAC->CR|=1<<2;	//DACͨ��һ����ʹ�� TEN1=1
	DAC->CR|=4<<3;	//TIM2 TRGO�¼�����
	DAC->CR|=0<<6;	//��ʹ�ò��η���
	DAC->CR|=0<<8;	//���Ρ���ֵ����
	DAC->CR|=1<<12;	//DAC1 DMAʹ�� 
	DAC->DHR12R1=0;//���ͨ��һ�����ݼĴ���
}
/*********��ʱ������************/
void SineWave_TIM_Config( u32 Wave1_Fre)
{
	RCC->APB1ENR|=1<<0;	//TIM2ʱ��ʹ��  
	TIM2->PSC=0x0;//����Ƶ
	TIM2->CR1|=0<<4;//���ϼ���ģʽ
	TIM2->ARR=72000000/N/Wave1_Fre-1;  //���Ƶ��	
	TIM2->CR2 &= (u16)~((u16)0x0070);
	TIM2->CR2|=0x0020;	//TIM2�����¼���ѡΪTRGO
	
//	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��6
}
/*********DMA����***********/
void SineWave_DMA_Config( u16 *Wave_Mem )
{					
	RCC->AHBENR|=1<<1;			//����DMA2ʱ��
	delay_ms(5);				//�ȴ�DMAʱ���ȶ�
	DMA2_Channel3->CPAR=DAC_DHR12R1; 	 	//DMA2 �����ַ 
	DMA2_Channel3->CMAR=(u32)Wave_Mem; 	//DMA2,�洢����ַ
	DMA2_Channel3->CNDTR=N;    	//DMA2,����������
	DMA2_Channel3->CCR=0X00000000;	//��λ
	DMA2_Channel3->CCR|=1<<4;  		//�Ӵ洢����
	DMA2_Channel3->CCR|=1<<5;  		//ѭ������
	DMA2_Channel3->CCR|=0<<6; 		//�����ַ������ģʽ
	DMA2_Channel3->CCR|=1<<7; 	 	//�洢������ģʽ
	DMA2_Channel3->CCR|=1<<8; 	 	//�������ݿ��Ϊ16λ
	DMA2_Channel3->CCR|=1<<10; 		//�洢�����ݿ��16λ
	DMA2_Channel3->CCR|=1<<12; 		//������ȼ�
	DMA2_Channel3->CCR|=1<<13; 		//������ȼ�
	DMA2_Channel3->CCR|=0<<14; 		//�Ǵ洢�����洢��ģʽ	
	DMA2_Channel3->CCR|=1<<0;          //����DMA����
}
void MyTIM_SetAutoreload(TIM_TypeDef* TIMx, u16 Autoreload)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* Set the Autoreload Register value */
  TIMx->ARR = Autoreload;
}
void SineWave_Init(u32 Wave_Fre)
{
	SineWave_Data(N,SineWave_Value);		//���ɲ��α�
	SineWave_GPIO_Config();			  //��ʼ������ 
	SineWave_TIM_Config(Wave_Fre);			  //��ʼ����ʱ�� 
	SineWave_DAC_Config();			  //��ʼ��DAC
	SineWave_DMA_Config(SineWave_Value);			  //��ʼ��DMA
	TIM2->CR1|=0x01;//ʹ��TIM2,��ʼ��������
}

void Set_WaveFre(u32 fre)
{
	u16 reload=(u16)(72000000/N/fre-1);
	MyTIM_SetAutoreload( TIM2 ,reload);
}
