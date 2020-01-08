#include "timer.h"
#include "led.h"
#include "usart.h"
#include "sys.h"  	 

void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // ���ģʽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1000; // ռ�ձȲ���
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);

    TIM_ARRPreloadConfig(TIM3, ENABLE); // ����ǵ�Ҫ��
    TIM_Cmd(TIM3, ENABLE);
}

//���뷶Χ0-100
void Tim3_PWM(u8 Value)
{
 	TIM_OCInitTypeDef TIM_OCStructure;	 	//����������Ƚϳ�ʼ���ṹ��

	TIM_OCStructure.TIM_OCMode = TIM_OCMode_PWM1;	   //PWM1ģʽ
	TIM_OCStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCStructure.TIM_Pulse = Value*10;		//����ֵ ����CCR�Ĵ���
	TIM_OCStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�Ƚ�ƥ������ߵ�ƽ
	TIM_OC3Init(TIM3, &TIM_OCStructure);  //ʹ���ϲ�����Ч
}

