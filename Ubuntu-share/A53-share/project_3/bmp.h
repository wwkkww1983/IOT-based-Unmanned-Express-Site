#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

//����Ļ��W��H�������λ����ʾһ��ͼƬ��ͼƬ����str����ֵ

int show_bmp(char *str,int W,int H) //falid����return -1  sucess����return 0 
{
	//��bmpͼƬ
	int bmpfd=open(str,O_RDWR);
	if(bmpfd==-1)
	{
		//printf("open picture.bmp falid\n");
		return -1;
	}
	
	//��ȡͷ�ӽ�����
	char headbuf[54];
	read(bmpfd,headbuf,54);
	
	//��ȡͼƬ��Ⱥ͸߶�
	int with=headbuf[18]|headbuf[19]<<8|headbuf[20]<<16|headbuf[21]<<24;
	int high=headbuf[22]|headbuf[23]<<8|headbuf[24]<<16|headbuf[25]<<24;
	//printf("with=%d  high=%d\n",with,high);
	
	//��Ҫ��������ص����ȷ��Ϊ ��*�ߵ�ʵ��ֵ
	char RGBbuf[with*high*3];
	read(bmpfd,RGBbuf,with*high*3);
	//�ֽڶ���
	int bmpbuf[with*high];
	int i;
	for(i=0;i<with*high;i++)
		bmpbuf[i]=RGBbuf[3*i] |RGBbuf[3*i+1] <<8|RGBbuf[3*i+2] <<16;
		//RGB32 		��ɫ		��ɫ			��ɫ
	
	//��lcd��Ļ�ļ�	/dev/fb0
	int fd=open("/dev/fb0",O_RDWR);
	if(fd==-1)
	{
		printf("open falid\n");
		return -1;
	}
	//printf("open succes\n");
	
	//�ڴ�ӳ��---�����800*480��LCD��Ļ����BMPͼƬ�޹�
	int *lcdmem=mmap(NULL,	800*480*4,PROT_READ |PROT_WRITE,	
					MAP_SHARED, fd,0);
	if(lcdmem==MAP_FAILED)					
	{
		printf("mmap fialed\n");
		return -1;
	}
	//H+y<480 �� W+x<800  ��LCD��Ļ����
	// y<high �� x<with   ��bmpͼƬ����
	int x,y;
	for(y=0;H+y<480&&y<high;y++)
		for(x=0;W+x<800&&x<with;x++)
			*(lcdmem+(W+x)+800*(H+y))=bmpbuf[x+(high-1-y)*with];
											//x+(479-y)*800
	//�ر��ļ�
	close(fd);
	close(bmpfd);
	return 0;
}