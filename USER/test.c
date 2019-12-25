#include "sys.h"
#include "usart.h"		
#include "delay.h"	 	 
#include "key.h"	 
#include "led.h"
#include "STM32_WaveOutput.h"
//注意有的f103系列没有DAC
//tim2触发dac通道一更新，dac数据由dma2传输
//WaceOutput.h中的N越大，正弦波形失真越小，能达到的正弦频率上限越小
//PA4输出正弦波
//DAC1输出缓存不使能，不能直接接较大的负载
//若使能缓冲，注意DAC数据寄存器为0时，电压不为0，注意调整SineWave_Data()函数
int main(void)
{						 
	Stm32_Clock_Init(9);	//系统时钟设置
//	uart_init(72,9600);	 	//串口初始化为9600
	delay_init(72);	   	 	//延时初始化 
//	LED_Init();
//	KEY_Init();         	//初始化与按键连接的硬件接口
	SineWave_Init(1000);//频率设定 1Hz为单位
	while(1)
	{
		
	} 
}


