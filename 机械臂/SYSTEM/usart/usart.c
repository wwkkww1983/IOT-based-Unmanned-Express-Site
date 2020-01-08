#include "sys.h"
#include "usart.h"	
#include "common.h"
#include "oneline.h"
#include "led.h"

unsigned char flag_RecFul=0;
extern unsigned char flag_connect;
extern uint16 CPWM[];
extern unsigned char flag_download;
extern unsigned char flag_stop_download;
extern char uart2_buf[50];
extern unsigned char flag_uart2_rev;
extern u8 yysb_start,Total_index;
extern char yysb_flag;
extern uint8 redata[500];   // ����������ݱ�������

char u_buf[];
u8 index1, index2;
u8 flag_uart2=0;
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

void Uart1_NVIC_Init(void)
{
	//Usart1 NVIC ����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

void Uart2_NVIC_Init(void)
{
	//Usart2 NVIC ����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

void Uart3_NVIC_Init(void)
{
	//Usart2 NVIC ����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

void Uart1_Gpio_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
}

void Uart2_Gpio_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//ʹ��USART2��GPIOAʱ��
		//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
  //USART2_RX	  GPIOA.3��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3
}

void Uart3_Gpio_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  	
	/* config USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void Uart_Init(u16 uart_num)
{
	if(uart_num==1)
	{
		Uart1_NVIC_Init();
		Uart1_Gpio_Config();
		
	}
	else if(uart_num==2)
	{
		Uart2_NVIC_Init();
		Uart2_Gpio_Config();
	}
	else if(uart_num==3)
	{
		Uart3_NVIC_Init();
		Uart3_Gpio_Config();
	}
}

void USART_Config(USART_TypeDef* TUSARTx,u32 bound) 
{
  
	USART_InitTypeDef USART_InitStructure;
 
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(TUSARTx, &USART_InitStructure); //��ʼ������
  USART_ITConfig(TUSARTx, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(TUSARTx, ENABLE);                    //ʹ�ܴ���
}

void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
  //Usart1 NVIC ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void USART3_Config(u32 bound)
{
  USART_InitTypeDef USART_InitStructure;   
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_HalfDuplexCmd(USART3,ENABLE);
	USART_Cmd(USART3, ENABLE); 
}

void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data)  
{  
  USART_SendData(USARTx, Data);  
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){}  
}  

void Usart2_send_nbyte(u8 *Data, u16 size) 
{
	u16 i = 0;
	for(i=0; i<size; i++) 
	{
		USART_SendData(USART2, Data[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
	}
	return;
}

void Uart2_send_nbyte(u8 *Data, u16 size) 
{
	Usart2_send_nbyte(Data, size);
}

void UART_PutStr (USART_TypeDef* USARTx, char *str) 
{
	unsigned int i = 0;
	while(*str != '\0' && i < 500)
	{
		UART_PutChar(USARTx, *str++); 
		i++;
	}
}

void UART_Put_Num(uint16  dat)
{
	char temp[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  u32tostr(dat,temp);
  UART_PutStr(USART1,temp); 
}

void UART_Put_Inf(char *inf,uint16 dat)
{
  UART_PutStr (USART1,inf); 
  UART_Put_Num(dat);
  UART_PutStr (USART1,"\n");  
}

void USART1_IRQHandler(void)                
{
  static unsigned int i=0;
	u8 Res;
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		 USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{ 
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		Res =USART_ReceiveData(USART1);	
					
		if(!flag_RecFul)
		{
			redata[i]=Res;
			i++;
			if(redata[i-1]=='\n'||i>256)
			{
			
					redata[i] = 0;
					flag_RecFul=1;
					flag_connect = 1;
					i=0;
			}
		}
		else
		{
			if (flag_download && (Res == 'S'))
			{
				flag_stop_download = 1;
			}
    }
	} 
} 

void USART2_IRQHandler(void)                
{
  static unsigned int i=0;
	u8 Res;
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		 USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
	{ 
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Res =USART_ReceiveData(USART2);	
		if(!flag_RecFul)
		{
			redata[i]=Res;
			i++;
			if(redata[i-1]=='\n'||i>256)
			{
					redata[i] = 0;
					flag_RecFul=1;
					flag_connect = 1;
					i=0;
			}
		}
		else
		{
			if (flag_download && (Res == 'S'))
			{
				flag_stop_download = 1;
			}
    }
	} 
} 

void USART3_IRQHandler(void) 
{
	static char num=0;
	unsigned char ch=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{ 
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		 ch=USART_ReceiveData(USART3);	
		if(!flag_uart2_rev)
		{
			if(ch=='#')
			{
				num=0;
				uart2_buf[num]=ch;
			}
			else if(uart2_buf[num-1]=='\r')
			{
				uart2_buf[num]=0;
				flag_uart2_rev=1;
				num=0;
				LED1(0);
			}
			uart2_buf[num]=ch;
			num++;
		}
	}
}

void DealRec(void)
{	
	  OneLine((char *)redata);
}




