#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"	    				
#include "servor.h"							
#include "usart.h"		  					
#include "delay.h"						
#include "timer.h"					
#include "PS2.h"
#include "key.h"
#include "led.h"
#include "beep.h"
#include "flash.h"
#include "common.h"
#include "adc.h"
#include "sensor.h"

#define SIZE sizeof(TEXT_Buffer)	
	
const u8 TEXT_Buffer[]={"Flash SPI TEST"};
u8 yysb_init[] = {0xa0, 0xa0, 0xa0};			//��ʼ��
u8 yysb_mul[] = {0xab, 0xab, 0x00};			//��������ʶ��
u8 yysb_exit[] = {0xac,0xac,0x00};
u8 yysb_rmul=0;
u8 yysb_flag=0;
u8 datatemp[SIZE];
u32 FLASH_SIZE;
u8 date[3];			//�ñ��������洢��׿�ն˷���������
u32 value;									
u32 key,key_bak;
u8 yysb_start,Total_index;
unsigned int dj1=1500;
unsigned int dj2=1500;
unsigned int dj3=1500;
unsigned int dj4=1500;
unsigned int dj5=1500;
unsigned int dj6=1600;
unsigned char buf[30];
uint8 line=0;			//�������������֮��ľ��룬����ǰ�������ж��ٸ�û��ִ�е�����
char point_now=0;			//��point_aimһ���ǻ������λ�ã���ȡ��λ��
char point_aim=1;
char point_in=1;			//��ǻ������λ�ã�����һ�����ݴ��λ��
uint8 flag_connect = 0;
uint8 flag_stop=1;			//��ʾһ��ִ�н���
uint8 flag_vpwm=0;			//��ʾ�����˸ø���pwm[]��ʱ��
uint8 flag_in=1;			//��ʾ�������п��пռ�
uint8 flag_out=0;			//��ʾ�������п�ִ�����ݵı�־λ
uint8 flag_run_ready=0;			//��ʾ��ҪҪ���뻺���EErom����
uint16 n=1000;			//����������Ҫ�������ٸ��м�����
uint16 m=1;			//�����ۼ��Ѿ�ִ���˶����м�����
double dp;
double dp0[MOTOR_NUM] = {0};			//�岹����
uint8 flag_download = 0;			//�ж��Ƿ�����
uint8 flag_All_download = 0;			//�ж��Ƿ�������������
uint8 flag_All_Stop_Down=0;			//��־λ1
uint8 flag_read = 0;			//��ȡflash���ݣ�������λ��
uint8 flag_connect_run = 0;			//������λ����ִ��flash���������
uint8 flag_stop_download = 0;			//ֹͣ����
uint8 flag_online_run = 0;
uint8 flag_uart2_rev = 0;
uint8 flag_uart2_rev_time_out = 0;
uint8 flag_ps2_rev = 0;
char redata[257] = {0};    // ����������ݱ�������
unsigned long send_mode = 0;//��ǰ���ڽ��յ�����״̬�洢
MotorData motor_data;//�������Ϣ
MotorOneCmd motor_one_cmd;//��������
CurrentItem cur_item;
uint16 tuoji_count = 0;//�ѻ�ִ�д���
unsigned char flag_scan_ps2 = 0;
uint8 flag_adc=0;
uint8 flag_sensor=0;
uint8 error = 0;
uint8 file_list[MAX_SUPPORT_FILE_SAVE] = {0};
int file_list_count = 0;
int file_last_num = 1;
unsigned char ps2_buf[120] = {0};
char uart2_buf[50] = {0};
uint16 cur_count = 0;
uint16 adc_value=0;
u8 ps2_mode=0;
char csb=1;
char All_down_num=-1;
uint8 Flag_Flash=0;

extern u32 a;			//a�������������ϵͳ�δ�ʱ���ɼ���������ʱ��
extern uint8 flag_RecFul;

uint16 CPWM[MOTOR_NUM]=   {1500,1500,1500,1500,1500,1500,1500,1500,1500};
uint16 UartRec[MOTOR_NUM]={1500,1500,1500,1500,1500,1500,1500,1500,1500};

uint16 pos[7][MOTOR_NUM]={ {1000,1500,1500,1500,1500,1500,1500,1500,1500},
					 {1000,1500,1500,1500,1500,1500,1500,1500,1500},
					 {1000,1500,1500,1500,1500,1500,1500,1500,1500},
					 {1000,500,500,500,500,500,500,500,500},
					 {1000,1500,1500,1500,1500,1500,1500,1500,1500},
					 {1000,500,500,500,500,500,500,500,500},
					 {1000,1500,1500,1500,1500,1500,1500,1500,1500}
};			//��е��λ��

void SerVor_Test(void);
void Ps2_Test(void);
void Flash_Test(void);
void updata_file_list(void);
void InitMotor(void);
void vpwm(void);

/*********************************������*******************************************/
int main(void)
{   
	SysTick_Init();			//ϵͳ�δ�ʱ����ʼ��
	Servor_GPIO_Config();			//GPIO��ʼ��
  Uart_Init(1);
  Uart_Init(2);	
	Uart_Init(3);
	Timer_Init();			//��ʱ����ʼ��
	Timer_ON();			//������ʱ��	 		
	LED_Init();	
	Beep_Init();
	PS2_Init();
	Adc_Init();
	Sensor_init();
	USART_Config(USART1,115200); 
	USART_Config(USART2,115200); 
	USART3_Config(115200);
	SpiFlashInit();			//��ʼ��flash	 	
	while(SpiFlashReadID()!=W25Q64)			//��ⲻ��W25Q64
	{
		UART_PutStr (USART1,"Not Found W25Q64\r\n");
	}
	Led_Test();
	Beep_Test();
	Delay_ms(500);
	Beep_Test();
  InitMotor();
	
	while (1)
	{	
		static unsigned int i = 0;
		static unsigned int z = 1;
		u8 Res;		
		if(flag_vpwm == 1)		  
		{	
			vpwm();			//����pwm[]����
			flag_vpwm=0;
		}
		if(flag_RecFul==1)		   //���ڽ�����һ��ָ��
		{
			
		 	DealRec();	 			//�����ڻ����е�����
			flag_RecFul=0;
		}
		GetOneMotorCMD();			//��ȡһ������
		SendUartState();			//����״̬��Ϣ
	}
} 

void updata_file_list(void)
{
	unsigned char i = 0;
	unsigned char j = 0;
	file_last_num = -1;
	ReadMoterInfor();
	for (i = 0; i < motor_data.filecount; i++)
	{
		if (motor_data.file_flag[i] == 1)
		{
			file_list[j] = i;
			j++;
			file_last_num = i;
		}
	}
	file_list_count = j;
}

void InitMotor(void)
{
	ReadMoterInfor();//��ȡ���������Ϣ
	updata_file_list();
	memset(&cur_item,0,sizeof(cur_item));
}

/*****************************��flash��ȡ����ܵ���Ϣ*********************************/
void ReadMoterInfor(void)
{
	memset(&motor_data,0,sizeof(motor_data));//�� 0
	SpiFlashRead((unsigned char *)&motor_data,(CMD_FLASH_ADDR)<<WRITE_BIT_DEPTH,sizeof(motor_data));			//��ȡ��Ϣ
	if (motor_data.CRC1 != 0x12345678 || motor_data.sum < 0 || motor_data.duoji_count  > MOTOR_NUM)			//�ж���Ϣ�洢�Ƿ��д�
	{
		memset(&motor_data,0,sizeof(motor_data));
	}
}

void ReadOneCmdInfor(unsigned int addr)
{
	memset(&motor_one_cmd,0,sizeof(motor_one_cmd));			//�� 0
	SpiFlashRead((unsigned char *)&motor_one_cmd,((((unsigned long)addr)<<4) + FILE_FLASH_ADDR)<<WRITE_BIT_DEPTH,sizeof(motor_one_cmd));//��ȡ��Ϣ
	if (motor_one_cmd.start >= motor_one_cmd.end || motor_one_cmd.cur_file_num != (addr) || motor_data.file_flag[motor_one_cmd.cur_file_num] == 0)//�ж���Ϣ�洢�Ƿ��д�
	{
		memset(&motor_one_cmd,0,sizeof(motor_one_cmd));
	}
	else			//������Ϣ,�Ժ�������֤�õ�
	{
		cur_item.tuoji_count = motor_one_cmd.tuoji_count;//�ѻ����д���
		cur_item.cur_num = motor_one_cmd.start;
	}
}

/****************************�Ѷ����Ϣд��flash��***********************************/	
void WriteMoterInfor(void)
{
	unsigned char temp =0;
	motor_data.CRC1 = 0x12345678;			//У����
	motor_data.duoji_count = MOTOR_NUM-1;
	temp = motor_data.filecount;
	SpiFlashEraseSector(CMD_FLASH_ADDR >> 4);			//������ǰ�洢����Ϣ
	SpiFlashWrite((unsigned char *)&motor_data,CMD_FLASH_ADDR<<WRITE_BIT_DEPTH,sizeof(motor_data));			//д��flash
	ReadMoterInfor();
	if (temp != motor_data.filecount)
	{
		error |= ERROR_FLASH_WRITE;
	}
	else
	{
		error &= ~ERROR_FLASH_WRITE;
	}
}

void WriteOneCmdInfor(unsigned int addr)
{
	unsigned char temp = 0;
	temp = motor_one_cmd.end;
	if (((((unsigned long)addr)<<4) + FILE_FLASH_ADDR) % 16 == 0)
	SpiFlashEraseSector(((((unsigned long)addr)<<4) + FILE_FLASH_ADDR) >> 4);//������ǰ�洢����Ϣ
	SpiFlashWrite((unsigned char *)&motor_one_cmd,((((unsigned long)addr)<<4) + FILE_FLASH_ADDR)<<WRITE_BIT_DEPTH,sizeof(motor_one_cmd)); //д��flash
	ReadOneCmdInfor(addr);
	if (temp !=  motor_one_cmd.end)
	{
		error |= ERROR_FLASH_WRITE1;
	}
	else
	{
		error &= ~ERROR_FLASH_WRITE1;
	}
}

/****************************��ȡһ������Ŀ����ַ���********************************/
void GetOneMotorCMD(void)
{
#if DEBUG
	uchar buf[20] = {0};
#endif
	if(flag_stop_download)			//���յ�����λ����ֹͣ���ص�����
	{
		flag_download = 0;			//�������״̬��־λ
		flag_stop_download = 0;
		flag_read = 0;
		if(motor_data.filecount < MAX_SUPPORT_FILE_SAVE)
		{
			updata_file_list();
			motor_data.sum = motor_one_cmd.end;
			motor_data.file_flag[motor_data.filecount] = 1;
			motor_one_cmd.cur_file_num = file_last_num + 1;
			motor_data.filecount = motor_one_cmd.cur_file_num + 1;
			error &= ~MAX_SUPPORT_FILE_SAVE;
#if DEBUG		
			sprintf(buf,"%d %d\r\n",(unsigned int)motor_data.filecount,(unsigned int)motor_data.file_flag[motor_data.filecount-1]);
			UART_PutStr(USART1,buf);
#endif
			WriteMoterInfor();
			WriteOneCmdInfor(motor_one_cmd.cur_file_num);
			updata_file_list();
#if DEBUG
			sprintf(buf,"%d %d\r\n",(uint)motor_data.filecount,(uint)motor_data.file_flag[motor_data.filecount-1]);
			UART_PutStr(USART1,buf);
#endif
		}
		else
		{
			error |= MAX_SUPPORT_FILE_SAVE;
		}
		if	(!(error &(MAX_SUPPORT_FILE_SAVE | ERROR_FLASH_FULL)))
			send_mode |= SEND_DOWN_OK;			//״̬λ��Ϊ
	}
	if (flag_connect)			//�����ǰ����λ������״̬
	{
		if (flag_read)			//�����λ����ȡflash�ڴ洢����Ϣ
		{		
			if (cur_item.cur_num < motor_one_cmd.end)			//�ж��Ƿ񳬹�֮ǰ�洢����
			{
				if ((send_mode & SEND_SEND_FILE))			//��ʼ���յ���ȡ������Ҫ�ȷ��͸�start
				{
					UART_PutStr(USART1,"#Start\r\n");
					send_mode &= ~SEND_READ_FILE;
				}
				memset(redata,0,WRITE_SIZE);			//�� 0
				SpiFlashRead((unsigned char *)redata,(((long)cur_item.cur_num)<<WRITE_BIT_DEPTH),WRITE_SIZE);			//��ȡ��Ϣ
#if DEBUG
				sprintf(buf,"%d\r\n",cur_item.cur_num);
				UART_PutStr(USART1,buf);
#endif
				UART_PutStr(USART1,redata);			//����
				cur_item.cur_num++;
			}
			else			//����
			{
				if (cur_item.cur_num > 0)
					UART_PutStr(USART1,"#End\r\n");			//���ͽ����ַ���
				flag_read = 0;
			}
			send_mode = 0;			//�� 0
		}	
		if (flag_online_run)
		{
			if ((send_mode & SEND_CC) != 0  || cur_item.cur_num == motor_one_cmd.start)			//�����ǰ��Ҫ���¶������
			{
				if (cur_item.tuoji_count > 0)			//�ѻ�����û����
				{
					if (cur_item.cur_num < motor_one_cmd.end)			//�ж��Ƿ��ȡ����
					{
						SpiFlashRead((unsigned char *)redata,((long)cur_item.cur_num)<<WRITE_BIT_DEPTH,WRITE_SIZE);			//��ȡ����
						flag_RecFul = 1;			//��־λΪ1��
						cur_item.cur_num++;//
					}
					else			//ִ����һ��
					{
						cur_item.cur_num = motor_one_cmd.start;
						cur_item.tuoji_count--;			//��һ
					}
				}
				else			//ִ�����
				{
					flag_online_run = 0;
					if (flag_connect_run)			//�����λ��ѡ��ִ�еĹ��ܣ���Ҫ���� AGF��Ϊ����
					{
						UART_PutStr(USART1,"#AGF\r\n");
						flag_connect_run = 0;
					}
				}
			}
		}
	} 
	else			//�ѻ�
	{
		if (file_list_count < 0)
		{
			return;
		}
		if (cur_item.tuoji_count > 0)
		{
			if ((send_mode & SEND_CC) != 0  || cur_item.cur_num == motor_one_cmd.start)			//�����ǰ��Ҫ���¶������
			{
				if (cur_item.cur_num < motor_one_cmd.end)			//�ж��Ƿ��ȡ����
				{
					SpiFlashRead((unsigned char *)redata,((long)cur_item.cur_num)<<WRITE_BIT_DEPTH,WRITE_SIZE);			//��ȡ����
					flag_RecFul = 1;			//��־λΪ1��
					cur_item.cur_num++;//
				}
				else			//ִ����һ��
				{
					cur_item.cur_num = motor_one_cmd.start;
					cur_item.tuoji_count--;			//��һ
				}
			}
		}
		else
		{
			ReadOneCmdInfor(file_list[cur_item.file_num]);
			file_list_count--;
			cur_item.file_num++;
		}
	}
}

/*****************************���ʹ���״̬��Ϣ******************************/	
void SendUartState(void)
{
  char buf[40] = {0};
	unsigned char read_motor_num = 0;
	unsigned int i = 0;
	static int count = 0;
	if (send_mode)			//�����״̬��Ҫ����
	{
		if (send_mode & SEND_A)			//����A
		{
			 UART_PutChar(USART1,'A');
			send_mode &= ~SEND_A;			//��״̬
		}
		if (send_mode & SEND_CC) //����CC
		{
			UART_PutStr(USART1,"#CC\r\n");
			send_mode &= ~SEND_CC;
		}
		if (send_mode & SEND_DOWN_OK)			//��������ok��״̬�ַ���
		{
			sprintf(buf,"#Down+OK+%d\r\n",(int)motor_data.filecount-1);
			UART_PutStr(USART1,buf);
			send_mode &= ~SEND_DOWN_OK;
#if DEBUG
			sprintf(buf,"%d\r\n",(uint)motor_data.filecount);
			UART_PutStr(USART1,buf);
#endif
		}
		if (send_mode & SEND_START_OK)			//��������ʱ����ַ���
		{
			UART_PutStr(USART1,"#Veri+UART+OK+20160906+176\r\n");
			send_mode &= ~SEND_START_OK;
		}
		if (send_mode & SEND_READ_FILE)			//���Ͷ�ȡ�ļ���ʱ���ַ���
		{
			if (motor_data.filecount > 0)			//���������ж������
			{
				UART_PutStr(USART1,"#");			//����
				for (i = 0; i < motor_data.filecount;i++)
				{
					if (motor_data.file_flag[i] == 1)
					{
						ReadOneCmdInfor(i);
						if (motor_one_cmd.end - motor_one_cmd.start <= 0)
						{
							motor_data.file_flag[i] = 0;
							WriteMoterInfor();
#if DEBUG
							sprintf(buf,"E=%d S=%d",motor_one_cmd.end, motor_one_cmd.start);
#endif
						}
						else
						{
							sprintf(buf,"Name:%d.txt--Size:%d--",i,motor_one_cmd.end - motor_one_cmd.start);			//��ȡ�������
							UART_PutStr(USART1,buf);			//����
						}
					}
				}
				UART_PutStr(USART1,"\r\n");				
			}
			else
			{
				sprintf(buf,"#\r\n",motor_data.sum);
				UART_PutStr(USART1,buf);
			}
			send_mode &= ~SEND_READ_FILE;
		}
		if (send_mode & SEND_SET_OFFLINE_OK)			//�����ѻ����д���
		{
			WriteOneCmdInfor(motor_one_cmd.cur_file_num);			//����
			UART_PutStr(USART1,"#Enable+OK...\r\n");
			send_mode &= ~SEND_SET_OFFLINE_OK;
		}
		if (send_mode & SEND_SET_DISABLEOFFLINE_OK)			//��ֹ�ѻ�����
		{
			for (i = 0; i < motor_data.filecount;i++)
			{
				if (motor_data.file_flag[i] == 1)
				{
					ReadOneCmdInfor(i);
					motor_one_cmd.tuoji_count = 0;
					WriteOneCmdInfor(i);
				}
			}
			WriteMoterInfor();
			UART_PutStr(USART1,"#Disable+OK...\r\n");
			send_mode &= ~SEND_SET_DISABLEOFFLINE_OK;
		}
		if (send_mode & SEND_SET_ONLINE_OK)			//������������״̬
		{
			UART_PutStr(USART1,"#OK\r\n");
			sprintf(buf,"#%dGC%d\r\n",cur_item.file_num,cur_item.tuoji_count);
			UART_PutStr(USART1,buf);
			UART_PutStr(USART1,"#LP=0\r\n");
			send_mode &= ~SEND_SET_ONLINE_OK;
			flag_connect_run = 1;
		}
		if (send_mode & SEND_SET_DELETE_ONE_FILE_OK)			//����ɾ���ļ�����
		{
			if (cur_item.delete_num < motor_data.filecount)
			{
				motor_data.file_flag[cur_item.delete_num] = 0;
				WriteMoterInfor();
				updata_file_list();
				if (cur_item.delete_num  == motor_data.filecount-1)
				{
					motor_data.filecount = file_last_num + 1; 
					if (file_last_num == -1)
					{
						motor_data.sum = 0;
					}
					else
					{
						ReadOneCmdInfor(file_last_num);
						motor_data.sum = motor_one_cmd.end;
						motor_data.sum = (((long int)(motor_data.sum) >>4)<<4) + (1<<4);
					}
					WriteMoterInfor();
				}
				updata_file_list();		
				UART_PutStr(USART1,"#FDel+OK\r\n");
			}
			send_mode &= ~SEND_SET_DELETE_ONE_FILE_OK;
			
		}
		if (send_mode & SEND_SET_DELETE_ALL_FILE_OK)			//���Ͳ��������ļ�������
		{
			UART_PutStr(USART1,"#Format+Start\r\n");
			Flag_Flash=1;
			SpiFlashEraseChip();
			cur_item.tuoji_count = 0;
			motor_data.sum = 0;
			motor_data.filecount = 0;
			memset(motor_data.file_flag,0,sizeof(motor_data.file_flag));
			WriteMoterInfor();
			UART_PutStr(USART1,"#Format+OK\r\n");
			Flag_Flash=0;
			send_mode &= ~SEND_SET_DELETE_ALL_FILE_OK;
			updata_file_list();
		}
		if (send_mode & SEND_SET_PS2_OK)
		{
			UART_PutStr(USART1,"#PS2+OK...\r\n");
			send_mode &= ~SEND_SET_PS2_OK;
		}
		
#define MATHION_HAND_NUM 20
		if (send_mode & SEND_SET_READ_UART_MOTOR_ANGLE)
		{
			if (cur_item.read_num < MATHION_HAND_NUM)
			{
				
				if (flag_uart2_rev)
				{
					read_motor_num = atoi(uart2_buf + 1);
					if (read_motor_num == cur_item.read_num)
					{
						i = atoi(uart2_buf+5);
						sprintf(uart2_buf,"#%dP%d",(int)read_motor_num,i);
						UART_PutStr(USART1,uart2_buf);
						cur_item.read_num++;
						buf[0] = '#';
						buf[1] = cur_item.read_num / 100 % 10 + 0x30;
						buf[2] = cur_item.read_num / 10 % 10 + 0x30;
						buf[3] = cur_item.read_num % 10 + 0x30;
						buf[4] = 'P';buf[5] = 'R';buf[6] = 'A';buf[7] = 'D';buf[8] = '\r';buf[9] = '\n';
						UART_PutStr(USART3,buf);
					}
					flag_uart2_rev = 0;
					count = 0;
				}
				else
				{
					count++;
					if (count >= 300)
					{
						cur_item.read_num++;
						buf[0] = '#';
						buf[1] = cur_item.read_num / 100 % 10 + 0x30;
						buf[2] = cur_item.read_num / 10 % 10 + 0x30;
						buf[3] = cur_item.read_num % 10 + 0x30;
						buf[4] = 'P';buf[5] = 'R';buf[6] = 'A';buf[7] = 'D';buf[8] = '\r';buf[9] = '\n';
						UART_PutStr(USART3,buf);		
						flag_uart2_rev = 0;
						flag_uart2_rev_time_out = 0;
						count = 0;
					}
				}
			}
			else
			{
				send_mode &= ~SEND_SET_READ_UART_MOTOR_ANGLE;
				cur_item.read_num= 0;
				UART_PutStr(USART1,"\r\n");
			}
		}
	}
	if (send_mode & SEND_SET_SET_UART_MOTOR_PULK)
	{
		if (cur_item.pulk_num < MATHION_HAND_NUM)
		{
			count++;
			if (count >= 20)
			{
				sprintf(uart2_buf,"#%dPULK\r\n",(int)cur_item.pulk_num);
				UART_PutStr(USART3,uart2_buf);
#if DEBUG
				UART_PutStr(USART1,uart2_buf);
#endif
				cur_item.pulk_num++;
				count = 0;
			}
		}
		else
		{
			send_mode &= ~SEND_SET_SET_UART_MOTOR_PULK;
		}
	}
	if (send_mode & SEND_SET_SET_UART_MOTOR_ANGLE)
	{
		if (cur_item.pulk_num < MATHION_HAND_NUM)
		{
			count++;
			if (count >= 5)
			{
				sprintf(uart2_buf,"#%dPMOD%d\r\n",(int)cur_item.pulk_num,(int)cur_item.angle_mode);
				UART_PutStr(USART3,uart2_buf);
#if DEBUG
				UART_PutStr(USART1,uart2_buf);
#endif
				cur_item.pulk_num++;
				count = 0;
			}
		}
		else
		{
			sprintf(buf,"#255PMOD%d+0K...\r\n",(int)cur_item.angle_mode);
			UART_PutStr(USART1,buf);
			send_mode &= ~SEND_SET_SET_UART_MOTOR_ANGLE;
			flag_uart2_rev = 0;
		}
	}
	if (send_mode & SEND_SET_BEEP_ON)
	{
		ReadMoterInfor();
		motor_data.beep_mode = 1;
		WriteMoterInfor();
		UART_PutStr(USART1,"#FMQENABLE+OK...\r\n");
		send_mode &= ~SEND_SET_BEEP_ON;
	}
	if (send_mode & SEND_SET_BEEP_OFF)
	{
		ReadMoterInfor();
		motor_data.beep_mode = 0;
		WriteMoterInfor();
		UART_PutStr(USART1,"#FMQDISABLE+OK...\r\n");
		send_mode &= ~SEND_SET_BEEP_OFF;
	}		
}

/**********************��ҵ��λ�ã�ĩβ�ø��º���**********************/
 void change(void)
{	
	unsigned char s;
	if (line > 0)			//������������
	{  
		line--;			//ִ��һ��
		if (line < 5)			//������������µ�����			
			flag_in = 1;
		point_now++;			//ȡ��λ�ø���
		point_aim++;
		
		if (point_aim == 7)
		   	point_aim = 0;
		if (point_now == 7)
		   	point_now = 0;
		n=pos[point_aim][0] / 20;			//�����µĲ岹����	

		for (s = 1; s < MOTOR_NUM; s++)			//�����µĲ岹����
		{
		 if (pos[point_aim][s] > pos[point_now][s])
			{
	   			dp = pos[point_aim][s] - pos[point_now][s];
			   	dp0[s] = dp / n;
			}
		    if (pos[point_aim][s] <= pos[point_now][s])
			{
				dp = pos[point_now][s] - pos[point_aim][s];
				dp0[s] = dp / n;
				dp0[s] =- dp0[s];
			}
	   }
		m = 0;			//m��0
		flag_stop = 0;			//�������µ�Ŀ��λ�ã�ֹͣ��־����
	}
	else			//û�л������ݣ���line==0
	{
		flag_out = 0;			//������û������
	}
}

/**************************���ݲ岹**************************/
/*�岹ʱ����Ϊ20/12ms����timer0���ƣ�ʹ���ƽ��ʵ���ٶȿ��ƣ���һ��������ִ����һ�к�ȥ������һ�����ݣ�������change()*/
void vpwm(void)		 
{
	unsigned char j = 0;
	unsigned char how = 0;
  static unsigned char flag_how;
	static unsigned char flag_Tover;

	if (flag_stop == 1)			//һ����ҵȫ�����
	{	
		if (flag_out == 1)			//����������������
		{
			change();			//������
		}
	}
	else			//����һ������ȫ����ɣ������м�岹�׶�
	{
		m++;			//�����ۼӲ岹���Ĵ���
		if (m == n)			//n�Ǳ�����ҵҪ�岹���ܴ���
		{
		  flag_Tover = 1;			//һ�����ݵ�ִ��ʱ���Ѿ����
			send_mode |= SEND_CC;
		}
		for (j = 1; j < MOTOR_NUM; j++)
		{
			if (abs(CPWM[j] - pos[point_aim][j]) < 5)			//��⿿���յ�λ��
			{			
			   how++;			//�ǣ����ۼ�һ��
			   CPWM[j] = pos[point_aim][j];			//����ֱ�ӹ��ȵ��յ�λ��
			}	
			else			//�������յ㣬�����岹
			{
				CPWM[j] = pos[point_now][j] + m*dp0[j];
			}
		} 								
		if(how == MOTOR_NUM - 1)
			flag_how = 1;			//16������������յ�
		how = 0; 
		if((flag_Tover == 1)&&(flag_how == 1))			//�Ӳ岹�������������������涼�����յ㣬����ҵ�����
		{								
			 flag_Tover = 0;
			 flag_how = 0;
			 flag_stop = 1;
		}
	 }
	return;
}


#if 0
void SerVor_Test(void)
{
		CPWM[0] = 700;CPWM[1] = 700;CPWM[2] = 700;CPWM[3] = 700;CPWM[4] = 700;CPWM[5] = 700;
		Delay_ms(2000);
		CPWM[0] = 2300;CPWM[1] = 2300;CPWM[2] = 2300;CPWM[3] = 2300;CPWM[4] = 2300;CPWM[5] = 2300;
		Delay_ms(2000);
}
#endif
#if 0
void Ps2_Test(void)
{
		key = PS2_DataKey();
		if (key != 0)
    	{
				printf("  \r\n   %d  is  pressed  \r\n",key);
    	}
     printf(" %5d %5d %5d %5d\r\n",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY),PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY));
    Delay_ms(1000);   
}
#endif

#if 1
void Flash_Test(void)
{
		while(SpiFlashReadID() != W25Q64)			//��ⲻ��W25Q64
	{
		UART_PutStr (USART1,"Not Found W25Q64\r\n");
	}
	UART_PutStr(USART1," Found W25Q64\r\n");
	SpiFlashWrite((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);			//�ӵ�����100����ַд������
	UART_PutStr(USART1,"Flash Write Success\r\n");
	SpiFlashRead(datatemp,FLASH_SIZE-100,SIZE);			//��ȡд��flash�е�����
	UART_PutStr(USART1,datatemp);
	UART_PutStr(USART1,"\r\n Flash Read Success\r\n");
	while(1);
}
#endif
