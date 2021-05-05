#include "24cxx.h" 
#include "my_iic.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32F103������V1
//24CXX���� ����(�ʺ�24C01~24C256)		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	MYIIC_IIC_Init();
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
    MYIIC_IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_IIC_Send_Byte(0XA0);	   //����д����
		MYIIC_IIC_Wait_Ack();
		MYIIC_IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}else MYIIC_IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	MYIIC_IIC_Wait_Ack(); 
    MYIIC_IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	MYIIC_IIC_Wait_Ack();	    
	MYIIC_IIC_Start();  	 	   
	MYIIC_IIC_Send_Byte(0XA1);           //�������ģʽ			   
	MYIIC_IIC_Wait_Ack();	 
    temp=MYIIC_IIC_Read_Byte(0);		   
    MYIIC_IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    MYIIC_IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_IIC_Send_Byte(0XA0);	    //����д����
		MYIIC_IIC_Wait_Ack();
		MYIIC_IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else MYIIC_IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	MYIIC_IIC_Wait_Ack();	   
    MYIIC_IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	MYIIC_IIC_Wait_Ack(); 	 										  		   
	MYIIC_IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	MYIIC_IIC_Wait_Ack();  		    	   
    MYIIC_IIC_Stop();					//����һ��ֹͣ���� 
	HAL_Delay(100);	 				//EEPROM��д���ٶȱȽ����������ӳ�
} 
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}








