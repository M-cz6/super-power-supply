//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RCT6,����ԭ��MiniSTM32������,��Ƶ72MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��          3.3V         //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ4����SPI����
//     LCDģ��                STM32��Ƭ��    
//       SDA         ��          PB15         //Һ����SPI��������д�ź�
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//       BLK         ��          PB9          //Һ������������źţ��������Ҫ���ƣ���3.3V
//       SCK         ��          PB13         //Һ����SPI����ʱ���ź�
//       DC          ��          PB10         //Һ��������/��������ź�
//       RES         ��          PB12         //Һ������λ�����ź�

//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//      T_IRQ        ��          PC10         //�����������ж��ź�
//      T_DO         ��          PC2          //������SPI���߶��ź�
//      T_DIN        ��          PC3          //������SPI����д�ź�
//      T_CS         ��          PC13         //������Ƭѡ�����ź�
//      T_CLK        ��          PC0          //������SPI����ʱ���ź�
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "lcd.h"
#include "stdlib.h"	 
#include "spi.h"
#include "tim.h"
#include "stm32f1xx_hal.h"
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)

//������ɫ,������ɫ
u16 POINT_COLOR,BACK_COLOR;  
u16 DeviceCode;	


/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{ 
     LCD_CS_CLR;     
	 LCD_RS_CLR;
	 HAL_SPI_Transmit(&hspi1,&data,1,0xffffffff);
     LCD_CS_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
     LCD_CS_CLR;
	 LCD_RS_SET;
	 HAL_SPI_Transmit(&hspi1,&data,1,0xffffffff);
     LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
   LCD_CS_CLR;
   unsigned char buf;
   LCD_RS_SET;  
   buf=(Data>>8);
   HAL_SPI_Transmit(&hspi1,&buf,sizeof(buf),10);
   buf=Data;
   HAL_SPI_Transmit(&hspi1,&buf,sizeof(buf),10);
   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
	unsigned int i,m; 
	unsigned char buf;	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
		   buf=(Color>>8);
		   HAL_SPI_Transmit(&hspi1,&buf,sizeof(buf),10);
//		   SPI_WriteByte(SPI2,buf);
//		   HAL_SPI_Transmit_DMA(&hspi2,&buf,sizeof(buf));
		   buf=Color;
		   HAL_SPI_Transmit(&hspi1,&buf,sizeof(buf),10);
//		   SPI_WriteByte(SPI2,buf);
//		   HAL_SPI_Transmit_DMA(&hspi2,&buf,sizeof(buf));
		
//		    SPI_WriteByte(SPI2,Color>>8);
//			SPI_WriteByte(SPI2,Color);
		}
	}
	 LCD_CS_SET;
} 

void LCD_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 Color){
	unsigned int i,m; 
	unsigned char buf;
	LCD_SetWindows(x1,y1,x2,y2);
	LCD_RS_SET;
	for(i=x1;i<x2+1;i++)
	{
    for(m=y1;m<y2+1;m++)
    {	
		   buf=(Color>>8);
		   HAL_SPI_Transmit(&hspi1,&buf,sizeof(buf),10);
		   buf=Color;
		   HAL_SPI_Transmit(&hspi1,&buf,sizeof(buf),10);
		}
	}
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_CS_SET;
	LCD_RST_CLR;
	HAL_Delay(100);
	LCD_RST_SET;
	HAL_Delay(100);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	
 	LCD_RESET(); //LCD ��λ
//************* ST7789V��ʼ��**********//	
	LCD_WR_REG(0x11);
	HAL_Delay(200);
	
//	LCD_WR_REG(0x2a); 		//Column address set
//	LCD_WR_DATA(0x00); 		//start column
//	LCD_WR_DATA(0x00); 
//	LCD_WR_DATA(0x00);		//end column
//	LCD_WR_DATA(0xef);

//	LCD_WR_REG(0x2b); 		//Row address set
//	LCD_WR_DATA(0x00); 		//start row
//	LCD_WR_DATA(0x28); 
//	LCD_WR_DATA(0x01);		//end row
//	LCD_WR_DATA(0x17);

	LCD_WR_REG(0xb2); 		//Porch control
	LCD_WR_DATA(0x0c); 
	LCD_WR_DATA(0x0c); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x33); 
	LCD_WR_DATA(0x33); 

//	LCD_WR_REG(0x20); 		//Display Inversion Off

	LCD_WR_REG(0xb7); 		//Gate control
	LCD_WR_DATA(0x56);   		//35
//---------------------------------ST7789S Power setting--------------------------------------// 
	LCD_WR_REG(0xbb); //VCOMS Setting
	LCD_WR_DATA(0x18);  //1f

	LCD_WR_REG(0xc0); 		//LCM Control
	LCD_WR_DATA(0x2c); 

	LCD_WR_REG(0xc2); 		//VDV and VRH Command Enable
	LCD_WR_DATA(0x01); 

	LCD_WR_REG(0xc3); //VRH Set
	LCD_WR_DATA(0x1f); //12

	LCD_WR_REG(0xc4); 			//VDV Setting
	LCD_WR_DATA(0x20); 

	LCD_WR_REG(0xc6); 			//FR Control 2
	LCD_WR_DATA(0x0f); 
//LCD_WR_REG(0xca); 
//LCD_WR_DATA(0x0f); 
//LCD_WR_REG(0xc8); 
//LCD_WR_DATA(0x08); 
//LCD_WR_REG(0x55); 
//LCD_WR_DATA(0x90); 
	LCD_WR_REG(0xd0);  //Power Control 1
	LCD_WR_DATA(0xa6);   //a4
	LCD_WR_DATA(0xa1); 
//--------------------------------ST7789S gamma setting---------------------------------------// 

	LCD_WR_REG(0xe0); 
	LCD_WR_DATA(0xd0); 
	LCD_WR_DATA(0x0d); 
	LCD_WR_DATA(0x14); 
	LCD_WR_DATA(0x0b); 
	LCD_WR_DATA(0x0b); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x3a);  
	LCD_WR_DATA(0x44); 
	LCD_WR_DATA(0x50); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x13); 
	LCD_WR_DATA(0x13); 
	LCD_WR_DATA(0x2d); 
	LCD_WR_DATA(0x32); 

	LCD_WR_REG(0xe1); 				//Negative Voltage Gamma Contro
	LCD_WR_DATA(0xd0); 
	LCD_WR_DATA(0x0d); 
	LCD_WR_DATA(0x14); 
	LCD_WR_DATA(0x0b); 
	LCD_WR_DATA(0x0b); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x3a); 
	LCD_WR_DATA(0x44); 
	LCD_WR_DATA(0x50); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x13); 
	LCD_WR_DATA(0x13); 
	LCD_WR_DATA(0x2d); 
	LCD_WR_DATA(0x32);
	
//	LCD_WR_REG(0x36); 			//Memory data access control
//	LCD_WR_DATA(0x00); 
//	
	LCD_WR_REG(0x3A); 			//Interface pixel format
	LCD_WR_DATA(0x55);			//65K	
	//LCD_WR_DATA(0x66);			//262K  RGB 6 6 6

//	LCD_WR_REG(0xe7); 			//SPI2 enable    ����2����ͨ��ģʽ
//	LCD_WR_DATA(0x00); 


	LCD_WR_REG(0x21);			//Display inversion on
//	LCD_WR_REG(0x29); 			//Display on	
	
	LCD_direction(3);
	
	//LCD_SetWindows(0,0,320,240);
	
	LCD_Clear(BLACK);
	LCD_WR_REG(0x29); 			//Display on
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	

	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA((xStar+lcddev.xoffset)>>8);
	LCD_WR_DATA(xStar+lcddev.xoffset);		
	LCD_WR_DATA((xEnd+lcddev.xoffset)>>8);
	LCD_WR_DATA(xEnd+lcddev.xoffset);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA((yStar+lcddev.yoffset)>>8);
	LCD_WR_DATA(yStar+lcddev.yoffset);		
	LCD_WR_DATA((yEnd+lcddev.yoffset)>>8);
	LCD_WR_DATA(yEnd+lcddev.yoffset);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			lcddev.xoffset=0;
			lcddev.yoffset=0;
			LCD_WriteReg(0x36,0);//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=0;
			lcddev.yoffset=0;
			LCD_WriteReg(0x36,(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;
            lcddev.xoffset=0;
			lcddev.yoffset=0;			
			LCD_WriteReg(0x36,(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=0;
			lcddev.yoffset=0;
			LCD_WriteReg(0x36,(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	 
