#include "sys.h"
#include "usart.h"		
#include "delay.h"	 	 
#include "key.h"	 
#include "led.h"
#include "STM32_WaveOutput.h"
//ע���е�f103ϵ��û��DAC
//tim2����dacͨ��һ���£�dac������dma2����
//WaceOutput.h�е�NԽ�����Ҳ���ʧ��ԽС���ܴﵽ������Ƶ������ԽС
//PA4������Ҳ�
//DAC1������治ʹ�ܣ�����ֱ�ӽӽϴ�ĸ���
//��ʹ�ܻ��壬ע��DAC���ݼĴ���Ϊ0ʱ����ѹ��Ϊ0��ע�����SineWave_Data()����
int main(void)
{						 
	Stm32_Clock_Init(9);	//ϵͳʱ������
//	uart_init(72,9600);	 	//���ڳ�ʼ��Ϊ9600
	delay_init(72);	   	 	//��ʱ��ʼ�� 
//	LED_Init();
//	KEY_Init();         	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	SineWave_Init(1000);//Ƶ���趨 1HzΪ��λ
	while(1)
	{
		
	} 
}


