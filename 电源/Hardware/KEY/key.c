#include "key.h"

void KEY_Init(void){ //微动开关的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(KEYPORT,&GPIO_InitStructure);			
}
