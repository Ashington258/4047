#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"

u8 OLED_GRAM[144][8];

//���Ժ���
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
		}
}

//��Ļ��ת180��
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}

//��ʱ
void IIC_delay(void)
{
	 delay_us(10);
}

//��ʼ�ź�
void I2C_Start(void)
{
	OLED_SDA_Set();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Clr();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//�����ź�
void I2C_Stop(void)
{
	OLED_SDA_Clr();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Set();
}

//�ȴ��ź���Ӧ
void I2C_WaitAck(void) //�������źŵĵ�ƽ
{
	OLED_SDA_Set();
	IIC_delay();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//д��һ���ֽ�
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)//��dat��8λ�����λ����д��
		{
			OLED_SDA_Set();
    }
		else
		{
			OLED_SDA_Clr();
    }
		IIC_delay();
		OLED_SCL_Set();
		IIC_delay();
		OLED_SCL_Clr();//��ʱ���ź�����Ϊ�͵�ƽ
		dat<<=1;
  }
}

//����һ���ֽ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}

//����OLED��ʾ 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ
}

//�ر�OLED��ʾ 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
	OLED_WR_Byte(0xAE,OLED_CMD);//�ر���Ļ
}

//�����Դ浽OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xb0+i,OLED_CMD); //��������ʼ��ַ
		OLED_WR_Byte(0x00,OLED_CMD);   //���õ�����ʼ��ַ
		OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ
		I2C_Start();
		Send_Byte(0x78);
		I2C_WaitAck();
		Send_Byte(0x40);
		I2C_WaitAck();
		for(n=0;n<128;n++)
		{
			Send_Byte(OLED_GRAM[n][i]);
			I2C_WaitAck();
		}
		I2C_Stop();
  }
}
//��������
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//�����������
			}
  }
	OLED_Refresh();//������ʾ
}


















//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���	
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}

//����
//x1,y1:�������
//x2,y2:��������
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
//x,y:Բ������
//r:Բ�İ뾶
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}






//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size1:ѡ������ 6x8/6x12/8x16/12x24
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //����0806����
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //����1206����
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //����1608����
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //����2412����
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}







//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

//m^n
u32 OLED_Pow(u32 m,u32 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}



//��ʾ����
//x,y :�������
//num :Ҫ��ʾ������
//len :���ֵ�λ��
//size:�����С
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowNum(u8 x,u8 y,int32_t num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(num>0)
	{
		if(size1==8)m=2;
		for(t=0;t<len;t++)
		{
			temp=(num/OLED_Pow(10,len-t-1))%10;
				if(temp==0)
				{
					OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
				}
				else 
				{
				  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
				}
		}		
	
	}

}


///�з��ŵ�����
void OLED_ShowSignedNum(uint8_t x, uint8_t y, int32_t Number, uint8_t Length, u8 size1,u8 mode)
{

	if(Number >= 0)						//���ִ��ڵ���0
	{
		u8 t,temp,m=0;
		if(size1==8)m=2;
		for(t=0;t<Length;t++)
		{
			temp=(Number/OLED_Pow(10,Length-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
			}
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
		}
	}
	else									//����С��0
	{
		uint8_t i;
		int32_t Number1;
		u8 t,temp,m=0;
		if(size1==8)m=2;
		OLED_ShowChar(x+(size1/2+m)*t,y,'-',size1,mode);
		Number1 = -Number;					//Number1����Numberȡ��
		for(t=0;t<Length;t++)
		{
			temp=(Number1/OLED_Pow(10,Length-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*(t+1),y,'0',size1,mode);
			}
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*(t+1),y,temp+'0',size1,mode);
			}
		}
	}
	
}





////��ʾ����
////x,y:�������
////num:���ֶ�Ӧ�����
////mode:0,��ɫ��ʾ;1,������ʾ
//void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
//{
//	u8 m,temp;
//	u8 x0=x,y0=y;
//	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
//	for(i=0;i<size3;i++)
//	{
//		if(size1==16)
//				{temp=Hzk1[num][i];}//����16*16����
//		else if(size1==24)
//				{temp=Hzk2[num][i];}//����24*24����
//		else if(size1==32)       
//				{temp=Hzk3[num][i];}//����32*32����
//		else if(size1==64)
//				{temp=Hzk4[num][i];}//����64*64����
//		else return;
//		for(m=0;m<8;m++)
//		{
//			if(temp&0x01)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp>>=1;
//			y++;
//		}
//		x++;
//		if((x-x0)==size1)
//		{x=x0;y0=y0+8;}
//		y=y0;
//	}
//}

////num ��ʾ���ֵĸ���
////space ÿһ����ʾ�ļ��
////mode:0,��ɫ��ʾ;1,������ʾ
//void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
//{
//	u8 i,n,t=0,m=0,r;
//	while(1)
//	{
//		if(m==0)
//		{
//	    OLED_ShowChinese(128,24,t,16,mode); //д��һ�����ֱ�����OLED_GRAM[][]������
//			t++;
//		}
//		if(t==num)
//			{
//				for(r=0;r<16*space;r++)      //��ʾ���
//				 {
//					for(i=1;i<144;i++)
//						{
//							for(n=0;n<8;n++)
//							{
//								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
//							}
//						}
//           OLED_Refresh();
//				 }
//        t=0;
//      }
//		m++;
//		if(m==16){m=0;}
//		for(i=1;i<144;i++)   //ʵ������
//		{
//			for(n=0;n<8;n++)
//			{
//				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
//			}
//		}
//		OLED_Refresh();
//	}
//}

////x,y���������
////sizex,sizey,ͼƬ����
////BMP[]��Ҫд���ͼƬ����
////mode:0,��ɫ��ʾ;1,������ʾ
//void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
//{
//	u16 j=0;
//	u8 i,n,temp,m;
//	u8 x0=x,y0=y;
//	sizey=sizey/8+((sizey%8)?1:0);
//	for(n=0;n<sizey;n++)
//	{
//		 for(i=0;i<sizex;i++)
//		 {
//				temp=BMP[j];
//				j++;
//				for(m=0;m<8;m++)
//				{
//					if(temp&0x01)OLED_DrawPoint(x,y,mode);
//					else OLED_DrawPoint(x,y,!mode);
//					temp>>=1;
//					y++;
//				}
//				x++;
//				if((x-x0)==sizex)
//				{
//					x=x0;
//					y0=y0+8;
//				}
//				y=y0;
//     }
//	 }
//}



















//uint8_t OLED_DisplayBuf[8][128];

///**
//  * ��    �����η�����
//  * ��    ����X ����
//  * ��    ����Y ָ��
//  * �� �� ֵ������X��Y�η�
//  */
//uint32_t OLED_Pow(uint32_t X, uint32_t Y)
//{
//    uint32_t Result = 1;    //���Ĭ��Ϊ1
//    while (Y --)            //�۳�Y��
//    {
//        Result *= X;        //ÿ�ΰ�X�۳˵������
//    }
//    return Result;
//}

///**
//  * ��    ������OLED�Դ����鲿������
//  * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ���������Ͻǵ������꣬��Χ��0~63
//  * ��    ����Width ָ������Ŀ�ȣ���Χ��0~128
//  * ��    ����Height ָ������ĸ߶ȣ���Χ��0~64
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
//{
//    uint8_t i, j;
//    
//    /*������飬��ָ֤�����򲻻ᳬ����Ļ��Χ*/
//    if (X > 127) {return;}
//    if (Y > 63) {return;}
//    if (X + Width > 128) {Width = 128 - X;}
//    if (Y + Height > 64) {Height = 64 - Y;}
//    
//    for (j = Y; j < Y + Height; j ++)        //����ָ��ҳ
//    {
//        for (i = X; i < X + Width; i ++)    //����ָ����
//        {
//            OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));    //���Դ�����ָ����������
//        }
//    }
//}

///**
//  * ��    ����OLED��ʾһ���ַ�
//  * ��    ����X ָ���ַ����Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ���ַ����Ͻǵ������꣬��Χ��0~63
//  * ��    ����Char ָ��Ҫ��ʾ���ַ�����Χ��ASCII��ɼ��ַ�
//  * ��    ����FontSize ָ�������С
//  *           ��Χ��OLED_8X16        ��8���أ���16����
//  *                 OLED_6X8        ��6���أ���8����
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize)
//{
//    if (FontSize == OLED_8X16)        //����Ϊ��8���أ���16����
//    {
//        /*��ASCII��ģ��OLED_F8x16��ָ��������8*16��ͼ���ʽ��ʾ*/
//        OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
//    }
//    else if(FontSize == OLED_6X8)    //����Ϊ��6���أ���8����
//    {
//        /*��ASCII��ģ��OLED_F6x8��ָ��������6*8��ͼ���ʽ��ʾ*/
//        OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
//    }
//}

///**
//  * ��    ����OLED��ʾ�ַ���
//  * ��    ����X ָ���ַ������Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ���ַ������Ͻǵ������꣬��Χ��0~63
//  * ��    ����String ָ��Ҫ��ʾ���ַ�������Χ��ASCII��ɼ��ַ���ɵ��ַ���
//  * ��    ����FontSize ָ�������С
//  *           ��Χ��OLED_8X16        ��8���أ���16����
//  *                 OLED_6X8        ��6���أ���8����
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize)
//{
//    uint8_t i;
//    for (i = 0; String[i] != '\0'; i++)        //�����ַ�����ÿ���ַ�
//    {
//        /*����OLED_ShowChar������������ʾÿ���ַ�*/
//        OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
//    }
//}

///**
//  * ��    ����OLED��ʾ���֣�ʮ���ƣ���������
//  * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ���������Ͻǵ������꣬��Χ��0~63
//  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��0~4294967295
//  * ��    ����Length ָ�����ֵĳ��ȣ���Χ��0~10
//  * ��    ����FontSize ָ�������С
//  *           ��Χ��OLED_8X16        ��8���أ���16����
//  *                 OLED_6X8        ��6���أ���8����
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
//{
//    uint8_t i;
//    for (i = 0; i < Length; i++)        //�������ֵ�ÿһλ                            
//    {
//        /*����OLED_ShowChar������������ʾÿ������*/
//        /*Number / OLED_Pow(10, Length - i - 1) % 10 ����ʮ������ȡ���ֵ�ÿһλ*/
//        /*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
//        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
//    }
//}

///**
//  * ��    ����OLED��ʾ�з������֣�ʮ���ƣ�������
//  * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ���������Ͻǵ������꣬��Χ��0~63
//  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
//  * ��    ����Length ָ�����ֵĳ��ȣ���Χ��0~10
//  * ��    ����FontSize ָ�������С
//  *           ��Χ��OLED_8X16        ��8���أ���16����
//  *                 OLED_6X8        ��6���أ���8����
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
//{
//    uint8_t i;
//    uint32_t Number1;
//    
//    if (Number >= 0)                        //���ִ��ڵ���0
//    {
//        OLED_ShowChar(X, Y, '+', FontSize);    //��ʾ+��
//        Number1 = Number;                    //Number1ֱ�ӵ���Number
//    }
//    else                                    //����С��0
//    {
//        OLED_ShowChar(X, Y, '-', FontSize);    //��ʾ-��
//        Number1 = -Number;                    //Number1����Numberȡ��
//    }
//    
//    for (i = 0; i < Length; i++)            //�������ֵ�ÿһλ                                
//    {
//        /*����OLED_ShowChar������������ʾÿ������*/
//        /*Number1 / OLED_Pow(10, Length - i - 1) % 10 ����ʮ������ȡ���ֵ�ÿһλ*/
//        /*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
//        OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
//    }
//}

///**
//  * ��    ����OLED��ʾʮ���������֣�ʮ�����ƣ���������
//  * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ���������Ͻǵ������꣬��Χ��0~63
//  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��0x00000000~0xFFFFFFFF
//  * ��    ����Length ָ�����ֵĳ��ȣ���Χ��0~8
//  * ��    ����FontSize ָ�������С
//  *           ��Χ��OLED_8X16        ��8���أ���16����
//  *                 OLED_6X8        ��6���أ���8����
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
//{
//    uint8_t i, SingleNumber;
//    for (i = 0; i < Length; i++)        //�������ֵ�ÿһλ
//    {
//        /*��ʮ��������ȡ���ֵ�ÿһλ*/
//        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
//        
//        if (SingleNumber < 10)            //��������С��10
//        {
//            /*����OLED_ShowChar��������ʾ������*/
//            /*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
//            OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
//        }
//        else                            //�������ִ���10
//        {
//            /*����OLED_ShowChar��������ʾ������*/
//            /*+ 'A' �ɽ�����ת��Ϊ��A��ʼ��ʮ�������ַ�*/
//            OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
//        }
//    }
//}

///**
//  * ��    ����OLED��ʾ���������֣������ƣ���������
//  * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ���������Ͻǵ������꣬��Χ��0~63
//  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��0x00000000~0xFFFFFFFF
//  * ��    ����Length ָ�����ֵĳ��ȣ���Χ��0~16
//  * ��    ����FontSize ָ�������С
//  *           ��Χ��OLED_8X16        ��8���أ���16����
//  *                 OLED_6X8        ��6���أ���8����
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
//{
//    uint8_t i;
//    for (i = 0; i < Length; i++)        //�������ֵ�ÿһλ    
//    {
//        /*����OLED_ShowChar������������ʾÿ������*/
//        /*Number / OLED_Pow(2, Length - i - 1) % 2 ���Զ�������ȡ���ֵ�ÿһλ*/
//        /*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
//        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
//    }
//}

///**
//  * ��    ����OLED��ʾ�������֣�ʮ���ƣ�С����
//  * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ���������Ͻǵ������꣬��Χ��0~63
//  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��-4294967295.0~4294967295.0
//  * ��    ����IntLength ָ�����ֵ�����λ���ȣ���Χ��0~10
//  * ��    ����FraLength ָ�����ֵ�С��λ���ȣ���Χ��0~9��������С�����о��ȶ�ʧ
//  * ��    ����FontSize ָ�������С
//  *           ��Χ��OLED_8X16        ��8���أ���16����
//  *                 OLED_6X8        ��6���أ���8����
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
//{
//    uint32_t Temp;
//    
//    if (Number >= 0)                        //���ִ��ڵ���0
//    {
//        OLED_ShowChar(X, Y, '+', FontSize);    //��ʾ+��
//    }
//    else                                    //����С��0
//    {
//        OLED_ShowChar(X, Y, '-', FontSize);    //��ʾ-��
//        Number = -Number;                    //Numberȡ��
//    }
//    
//    /*��ʾ��������*/
//    OLED_ShowNum(X + FontSize, Y, Number, IntLength, FontSize);
//    
//    /*��ʾС����*/
//    OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);
//    
//    /*��Number���������ּ�������ֹ֮��С�����ֳ˵�����ʱ����������ɴ���*/
//    Number -= (uint32_t)Number;
//    
//    /*��С�����ֳ˵��������֣�����ʾ*/
//    Temp = OLED_Pow(10, FraLength);
//    OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, ((uint32_t)(Number * Temp)) % Temp, FraLength, FontSize);
//}


///**
//  * ��    ����OLED��ʾͼ��
//  * ��    ����X ָ��ͼ�����Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ��ͼ�����Ͻǵ������꣬��Χ��0~63
//  * ��    ����Width ָ��ͼ��Ŀ�ȣ���Χ��0~128
//  * ��    ����Height ָ��ͼ��ĸ߶ȣ���Χ��0~64
//  * ��    ����Image ָ��Ҫ��ʾ��ͼ��
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
//{
//    uint8_t i, j;
//    
//    /*������飬��ָ֤��ͼ�񲻻ᳬ����Ļ��Χ*/
//    if (X > 127) {return;}
//    if (Y > 63) {return;}
//    
//    /*��ͼ�������������*/
//    OLED_ClearArea(X, Y, Width, Height);
//    
//    /*����ָ��ͼ���漰�����ҳ*/
//    /*(Height - 1) / 8 + 1��Ŀ����Height / 8������ȡ��*/
//    for (j = 0; j < (Height - 1) / 8 + 1; j ++)
//    {
//        /*����ָ��ͼ���漰�������*/
//        for (i = 0; i < Width; i ++)
//        {
//            /*�����߽磬��������ʾ*/
//            if (X + i > 127) {break;}
//            if (Y / 8 + j > 7) {return;}
//            
//            /*��ʾͼ���ڵ�ǰҳ������*/
//            OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[j * Width + i] << (Y % 8);
//            
//            /*�����߽磬��������ʾ*/
//            /*ʹ��continue��Ŀ���ǣ���һҳ�����߽�ʱ����һҳ�ĺ������ݻ���Ҫ������ʾ*/
//            if (Y / 8 + j + 1 > 7) {continue;}
//            
//            /*��ʾͼ������һҳ������*/
//            OLED_DisplayBuf[Y / 8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
//        }
//    }
//}

///**
//  * ��    ����OLEDʹ��printf������ӡ��ʽ���ַ���
//  * ��    ����X ָ����ʽ���ַ������Ͻǵĺ����꣬��Χ��0~127
//  * ��    ����Y ָ����ʽ���ַ������Ͻǵ������꣬��Χ��0~63
//  * ��    ����FontSize ָ�������С
//  *           ��Χ��OLED_8X16        ��8���أ���16����
//  *                 OLED_6X8        ��6���أ���8����
//  * ��    ����format ָ��Ҫ��ʾ�ĸ�ʽ���ַ�������Χ��ASCII��ɼ��ַ���ɵ��ַ���
//  * ��    ����... ��ʽ���ַ��������б�
//  * �� �� ֵ����
//  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
//  */
//void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...)
//{
//    char String[30];                        //�����ַ�����
//    va_list arg;                            //����ɱ�����б��������͵ı���arg
//    va_start(arg, format);                    //��format��ʼ�����ղ����б�arg����
//    vsprintf(String, format, arg);            //ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
//    va_end(arg);                            //��������arg
//    OLED_ShowString(X, Y, String, FontSize);//OLED��ʾ�ַ����飨�ַ�����
//}













//OLED�ĳ�ʼ��
void OLED_Init(void)
{
    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0???? 0xa1??
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0???? 0xc8??
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset        Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_Clear();
    OLED_WR_Byte(0xAF,OLED_CMD);
}