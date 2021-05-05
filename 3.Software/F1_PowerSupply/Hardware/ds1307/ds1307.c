/******************************************************************************
* @ File name --> ds1307.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 11 - 27 - 2013
* @ Brief     --> ʱ��оƬDS1307����
* @           --> ��������������DS1307��DS1338��DS1338ZоƬ
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.
* @ Author    --> By@
* @ Date      --> 
* @ Revise    --> 
*
******************************************************************************/

#include "ds1307.h"
#include "my_iic.h"


/******************************************************************************
                                    �������
******************************************************************************/

Time_Typedef TimeValue;  //����ʱ�仺��ָ��
uint8_t Time_Buffer[8];	//ʱ���������ݻ���

/******************************************************************************
* Function Name --> DS1307ĳ�Ĵ���д��һ���ֽ�����
* Description   --> none
* Input         --> REG_ADD��Ҫ�����Ĵ�����ַ
*                   dat��Ҫд�������
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void DS1307_Write_Byte(uint8_t REG_ADD,uint8_t dat)
{
	MYIIC_IIC_Start();
	MYIIC_IIC_Send_Byte(DS1307_Write); //����д������Ӧ��λ
	MYIIC_IIC_Send_Byte(REG_ADD);
	MYIIC_IIC_Send_Byte(dat);
	MYIIC_IIC_Stop();
}
/******************************************************************************
* Function Name --> DS1307ĳ�Ĵ�����ȡһ���ֽ�����
* Description   --> none
* Input         --> REG_ADD��Ҫ�����Ĵ�����ַ
* Output        --> none
* Reaturn       --> ��ȡ������ֵ
******************************************************************************/
uint8_t DS1307_Read_Byte(uint8_t REG_ADD)
{
	uint8_t rcv;

	MYIIC_IIC_Start();
	MYIIC_IIC_Send_Byte(DS1307_Write); //����д������Ӧ��λ
	//MYIIC_IIC_Wait_Ack();
	
	MYIIC_IIC_Send_Byte(REG_ADD);    //����Ҫ�����ļĴ�����ַ
	//MYIIC_IIC_Wait_Ack();
	MYIIC_IIC_Stop();
	HAL_Delay(10);
	MYIIC_IIC_Start();    //��������
	MYIIC_IIC_Send_Byte(DS1307_Read);    //���Ͷ�ȡ����
	MYIIC_IIC_Wait_Ack();
	
	rcv = MYIIC_IIC_Read_Byte(0);
//	MYIIC_IIC_Wait_Ack();
	MYIIC_IIC_Stop();
	return rcv;
	
	
	
	
//	MYIIC_IIC_Start();
// 
//    MYIIC_IIC_Send_Byte(addr+1);
//    MYIIC_IIC_Wait_Ack();
//    
//    temp = MYIIC_IIC_Read_Byte(0);
////    temp<<=8;    
////    temp |= MYIIC_IIC_Read_Byte(0);
//    
//    MYIIC_IIC_Stop();
//    return temp;
	
	

}
/******************************************************************************
* Function Name --> DS1307��ʱ�������Ĵ���������д�����ݻ��߶�ȡ����
* Description   --> ����д��n�ֽڻ���������ȡn�ֽ�����
* Input         --> REG_ADD��Ҫ�����Ĵ�����ʼ��ַ
*                   *WBuff��д�����ݻ���
*                   num��д����������
*                   mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS1307_Operate_Register(uint8_t REG_ADD,uint8_t *pBuff,uint8_t num,uint8_t mode)
{
	uint8_t i;
	if(mode)	//��ȡ����
	{
		MYIIC_IIC_Start();
		MYIIC_IIC_Send_Byte(DS1307_Write);	//����д������Ӧ��λ
		MYIIC_IIC_Send_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
		MYIIC_IIC_Stop();
		MYIIC_IIC_Start();	//��������
		MYIIC_IIC_Send_Byte(DS1307_Read);	//���Ͷ�ȡ����
		for(i = 0;i < num;i++)
		{
			*pBuff = MYIIC_IIC_Read_Byte(0x00);	//��ȡ����
			*pBuff=*pBuff-((*pBuff/16)*6);
			if(i == (num - 1))	MYIIC_IIC_NAck();	//���ͷ�Ӧ���ź�
			else MYIIC_IIC_Ack();	//����Ӧ���ź�
			pBuff++;
		}
		MYIIC_IIC_Stop();	
	}
	else	//д������
	{		 	
		MYIIC_IIC_Start();
		MYIIC_IIC_Send_Byte(DS1307_Write);	//����д������Ӧ��λ
		MYIIC_IIC_Send_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
		for(i = 0;i < num;i++)
		{
			MYIIC_IIC_Send_Byte(*pBuff);	//д������
			pBuff++;
		}
		MYIIC_IIC_Stop();
	}
}
/******************************************************************************
* Function Name --> DS1307��ȡ����д��ʱ����Ϣ
* Description   --> ����д��n�ֽڻ���������ȡn�ֽ�����
* Input         --> *pBuff��д�����ݻ���
*                   mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS1307_ReadWrite_Time(uint8_t mode)
{
	uint8_t Time_Register[8];	//����ʱ�仺��
	
	if(mode)	//��ȡʱ����Ϣ
	{
		DS1307_Operate_Register(Address_second,Time_Register,7,1);	//�����ַ��0x00����ʼ��ȡʱ����������
		
		/******�����ݸ��Ƶ�ʱ��ṹ���У��������������******/
		TimeValue.second = Time_Register[0] & Shield_secondBit;	//������
		TimeValue.minute = Time_Register[1] & Shield_minuteBit;	//��������
		TimeValue.hour = Time_Register[2] & Shield_hourBit;	//Сʱ����
		TimeValue.week = Time_Register[3] & Shield_weekBit;	//��������
		TimeValue.date = Time_Register[4] & Shield_dateBit;	//������
		TimeValue.month = Time_Register[5] & Shield_monthBit;	//������
		TimeValue.year = Time_Register[6];	//������
	}
	else
	{
		/******��ʱ��ṹ���и������ݽ���******/
		Time_Register[0] = TimeValue.second | Control_Chip_Run;	//�룬����оƬ
		Time_Register[1] = TimeValue.minute;	//����
		Time_Register[2] = TimeValue.hour | Hour_Mode24;	//Сʱ��24Сʱ��
		Time_Register[3] = TimeValue.week;	//����
		Time_Register[4] = TimeValue.date;	//��		
		Time_Register[5] = TimeValue.month;	//��
		Time_Register[6] = TimeValue.year;	//��
		
		DS1307_Operate_Register(Address_second,Time_Register,7,0);	//�����ַ��0x00����ʼд��ʱ����������
	}
}
/******************************************************************************
* Function Name --> DS1307���Ժû�
* Description   --> ��DS1307оƬ��RAM�����һ����ַд��һ�����ݲ��������ж�
*                   ���ϴ�д���ֵ��ȣ����ǵ�һ���ϵ磬�������ʼ��ʱ��
* Input         --> none
* Output        --> none
* Reaturn       --> 0���豸���������ǵ�һ���ϵ�
*                   1���豸�����������
******************************************************************************/	
uint8_t DS1307_Check(void)
{
	if(DS1307_Read_Byte(RAM_Address55) == test_data)    return 0;    //�豸���������ǵ�һ���ϵ�
	else    return 1;
}
/******************************************************************************
* Function Name --> DS1307���õ�RAMд���ݲ���
* Description   --> none
* Input         --> *pBuff��д���ݴ����
*                   WRadd����д��ʼ��ַ����Χ��RAM_Address0 ~ RAM_Address55֮�䣬���һλ��ַ��������;
*                   num����д�ֽ����ݵ���������Χ��1 ~ 55֮��
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS1307_RAM_Write_Data(uint8_t* pBuff,uint8_t WRadd,uint8_t num)
{
	uint8_t i;
	uint8_t ADDremain;   //д����������
    
	/******�ж�д�����ݵ���ʼ��ַ��Χ******/

	if(WRadd >= RAM_Address55)  return; //���һ��RAM��Ԫ������ֱ���˳�

	/******�жϷ������ݵ�����Ŀ******/

	if((WRadd + num) >= (RAM_Address55 - 1))    ADDremain = RAM_Address55 - 1 - WRadd;  //������Χ��д�����µĿռ�
	else    ADDremain = num;    //û�����ռ䣬ֱ��д��

	MYIIC_IIC_Start();
	MYIIC_IIC_Send_Byte(DS1307_Write); //����д������Ӧ���ź�
	MYIIC_IIC_Send_Byte(WRadd);  //����д�������׵�ַ
	for(i = 0;i < ADDremain;i++)
	{
		MYIIC_IIC_Send_Byte(pBuff[i]);   //д������
	}
	MYIIC_IIC_Stop();
}
/******************************************************************************
* Function Name --> DS1307���õ�RAM�����ݲ���
* Description   --> none
* Input         --> WRadd����д��ʼ��ַ����Χ��RAM_Address0 ~ RAM_Address55֮�䣬���һλ��ַ��������;
*                   num����д�ֽ����ݵ���������Χ��1 ~ 55֮��
* Output        --> *pBuff�������ݴ����
* Reaturn       --> none
******************************************************************************/
void DS1307_RAM_Read_Data(uint8_t* pBuff,uint8_t WRadd,uint8_t num)
{
	uint8_t i;
	uint8_t ADDremain;

	/******�ж϶�ȡ���ݵ���ʼ��ַ��Χ******/

	if(WRadd >= RAM_Address55)  return; //���һ��RAM��Ԫ������ֱ���˳�

	/******���һ����ַ���������DS1307���ã����Բ������һ����ַ����******/

	if((WRadd + num) >= RAM_Address55)  ADDremain = RAM_Address55 - 1 - WRadd;  //������Χ�ˣ���ȡ��ʼ��ַ�������ڶ�����ַ�ռ������
	else    ADDremain = num;    //û������ַ��Χ��ȫ����ȡ��

	MYIIC_IIC_Start();
	MYIIC_IIC_Send_Byte(DS1307_Write); //����д������Ӧ���ź�
	MYIIC_IIC_Send_Byte(WRadd);  //���Ͷ�ȡ���ݿ�ʼ�Ĵ�����ַ
	MYIIC_IIC_Start();
	MYIIC_IIC_Send_Byte(DS1307_Read);  //���Ͷ�ȡ������Ӧ���ź�
	for(i = 0;i < ADDremain;i++)
	{
		pBuff[i] = MYIIC_IIC_Read_Byte(0x00); //��ʼ����num������
		if(i == (ADDremain - 1))    MYIIC_IIC_NAck(); //��ȡ�����һ�����ݣ����ͷ�Ӧ���ź�
		else    MYIIC_IIC_Ack();  //����Ӧ���ź�
	}
	MYIIC_IIC_Stop();
}
/******************************************************************************
* Function Name --> ʱ��������ʼ��
* Description   --> none
* Input         --> *TimeVAL��RTCоƬ�Ĵ���ֵָ��
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS1307_Time_Init(Time_Typedef *TimeVAL)
{	
	//ʱ����������
	Time_Buffer[0] = TimeVAL->second | Control_Chip_Run;	//����RTCоƬ
	Time_Buffer[1] = TimeVAL->minute;
	Time_Buffer[2] = TimeVAL->hour;
	Time_Buffer[3] = TimeVAL->week;
	Time_Buffer[4] = TimeVAL->date;
	Time_Buffer[5] = TimeVAL->month;
	Time_Buffer[6] = TimeVAL->year;
	//Ƶ���������

    #ifdef  Chip_Type   //��������ˣ���ʹ�õ���DS1307оƬ
    
		Time_Buffer[7] = TimeVAL->SQWE;	//Ƶ���������

    #else   //û���壬��ʹ�õ���DS1338����DS1338ZоƬ

		Time_Buffer[7] = TimeVAL->SQWE | OSF_Enable;	//Ƶ���������

    #endif

//    DS1307_Write_Byte(Address_second, Control_Chip_Run);    //������оƬ
	
	DS1307_Operate_Register(Address_second,Time_Buffer,8,0);	//����Ĵ�����0x00����ʼд��8������

	DS1307_Write_Byte(RAM_Address55, test_data);    //�����һ��RAM��ַд��ʶ��ֵ
}

