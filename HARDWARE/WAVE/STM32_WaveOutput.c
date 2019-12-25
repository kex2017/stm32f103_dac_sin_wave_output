#include "STM32_WaveOutput.h"	
#include "delay.h"	
#include "usart.h"
/******DAC寄存器地址声明*******/	
#define DAC_DHR12R1    (u32)&(DAC->DHR12R1)   //DAC通道1输出寄存器地址
u16 SineWave_Value[N];
/********生成正弦波形输出表***********/
void SineWave_Data( u16 cycle ,u16 *D)
{
	u16 i;
	for(i=0;i<cycle;i++)
	{
		D[i]=(u16)((Um*sin(( 1.0*i/(cycle-1))*2*PI)+Um)*4095/3.3);
	}
}
/****************引脚初始化******************/
void SineWave_GPIO_Config()
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	GPIOA->CRL&=0xFFF0FFFF;//A4初始化
	GPIOA->CRL&=0x00030000;
	GPIOA->ODR|=(1<<4);
}

/******************DAC初始化*************************/
void SineWave_DAC_Config()
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	  	
	RCC->APB1ENR|=1<<29;   //使能DAC时钟	  
	
	GPIOA->CRL&=0XFFF0FFFF; 
	GPIOA->CRL|=0X00000000;//PA4 模拟输入 
	
	DAC->CR|=1<<0;	//使能DAC通道1
	DAC->CR|=1<<1;	//DAC1输出缓存不使能 BOFF1=1
	DAC->CR|=1<<2;	//DAC通道一触发使能 TEN1=1
	DAC->CR|=4<<3;	//TIM2 TRGO事件触发
	DAC->CR|=0<<6;	//不使用波形发生
	DAC->CR|=0<<8;	//屏蔽、幅值设置
	DAC->CR|=1<<12;	//DAC1 DMA使能 
	DAC->DHR12R1=0;//清空通道一的数据寄存器
}
/*********定时器配置************/
void SineWave_TIM_Config( u32 Wave1_Fre)
{
	RCC->APB1ENR|=1<<0;	//TIM2时钟使能  
	TIM2->PSC=0x0;//不分频
	TIM2->CR1|=0<<4;//向上计数模式
	TIM2->ARR=72000000/N/Wave1_Fre-1;  //输出频率	
	TIM2->CR2 &= (u16)~((u16)0x0070);
	TIM2->CR2|=0x0020;	//TIM2更新事件被选为TRGO
	
//	TIM2->CR1|=0x01;    //使能定时器6
}
/*********DMA配置***********/
void SineWave_DMA_Config( u16 *Wave_Mem )
{					
	RCC->AHBENR|=1<<1;			//开启DMA2时钟
	delay_ms(5);				//等待DMA时钟稳定
	DMA2_Channel3->CPAR=DAC_DHR12R1; 	 	//DMA2 外设地址 
	DMA2_Channel3->CMAR=(u32)Wave_Mem; 	//DMA2,存储器地址
	DMA2_Channel3->CNDTR=N;    	//DMA2,传输数据量
	DMA2_Channel3->CCR=0X00000000;	//复位
	DMA2_Channel3->CCR|=1<<4;  		//从存储器读
	DMA2_Channel3->CCR|=1<<5;  		//循环发送
	DMA2_Channel3->CCR|=0<<6; 		//外设地址非增量模式
	DMA2_Channel3->CCR|=1<<7; 	 	//存储器增量模式
	DMA2_Channel3->CCR|=1<<8; 	 	//外设数据宽度为16位
	DMA2_Channel3->CCR|=1<<10; 		//存储器数据宽度16位
	DMA2_Channel3->CCR|=1<<12; 		//最高优先级
	DMA2_Channel3->CCR|=1<<13; 		//最高优先级
	DMA2_Channel3->CCR|=0<<14; 		//非存储器到存储器模式	
	DMA2_Channel3->CCR|=1<<0;          //开启DMA传输
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
	SineWave_Data(N,SineWave_Value);		//生成波形表
	SineWave_GPIO_Config();			  //初始化引脚 
	SineWave_TIM_Config(Wave_Fre);			  //初始化定时器 
	SineWave_DAC_Config();			  //初始化DAC
	SineWave_DMA_Config(SineWave_Value);			  //初始化DMA
	TIM2->CR1|=0x01;//使能TIM2,开始产生波形
}

void Set_WaveFre(u32 fre)
{
	u16 reload=(u16)(72000000/N/fre-1);
	MyTIM_SetAutoreload( TIM2 ,reload);
}
