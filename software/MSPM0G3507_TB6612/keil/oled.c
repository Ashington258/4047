#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"

u8 OLED_GRAM[144][8];

//反显函数
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}

//延时
void IIC_delay(void)
{
	 delay_us(10);
}

//起始信号
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

//结束信号
void I2C_Stop(void)
{
	OLED_SDA_Clr();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
	OLED_SDA_Set();
	IIC_delay();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//写入一个字节
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)//将dat的8位从最高位依次写入
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
		OLED_SCL_Clr();//将时钟信号设置为低电平
		dat<<=1;
  }
}

//发送一个字节
//mode:数据/命令标志 0,表示命令;1,表示数据;
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

//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

//更新显存到OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
		OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
		OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
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
//清屏函数
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh();//更新显示
}


















//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空	
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

//画线
//x1,y1:起点坐标
//x2,y2:结束坐标
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//画点
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
//x,y:圆心坐标
//r:圆的半径
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
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}






//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //调用0806字体
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
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







//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
//mode:0,反色显示;1,正常显示
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
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



//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
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


///有符号的数字
void OLED_ShowSignedNum(uint8_t x, uint8_t y, int32_t Number, uint8_t Length, u8 size1,u8 mode)
{

	if(Number >= 0)						//数字大于等于0
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
	else									//数字小于0
	{
		uint8_t i;
		int32_t Number1;
		u8 t,temp,m=0;
		if(size1==8)m=2;
		OLED_ShowChar(x+(size1/2+m)*t,y,'-',size1,mode);
		Number1 = -Number;					//Number1等于Number取负
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





////显示汉字
////x,y:起点坐标
////num:汉字对应的序号
////mode:0,反色显示;1,正常显示
//void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
//{
//	u8 m,temp;
//	u8 x0=x,y0=y;
//	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //得到字体一个字符对应点阵集所占的字节数
//	for(i=0;i<size3;i++)
//	{
//		if(size1==16)
//				{temp=Hzk1[num][i];}//调用16*16字体
//		else if(size1==24)
//				{temp=Hzk2[num][i];}//调用24*24字体
//		else if(size1==32)       
//				{temp=Hzk3[num][i];}//调用32*32字体
//		else if(size1==64)
//				{temp=Hzk4[num][i];}//调用64*64字体
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

////num 显示汉字的个数
////space 每一遍显示的间隔
////mode:0,反色显示;1,正常显示
//void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
//{
//	u8 i,n,t=0,m=0,r;
//	while(1)
//	{
//		if(m==0)
//		{
//	    OLED_ShowChinese(128,24,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
//			t++;
//		}
//		if(t==num)
//			{
//				for(r=0;r<16*space;r++)      //显示间隔
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
//		for(i=1;i<144;i++)   //实现左移
//		{
//			for(n=0;n<8;n++)
//			{
//				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
//			}
//		}
//		OLED_Refresh();
//	}
//}

////x,y：起点坐标
////sizex,sizey,图片长宽
////BMP[]：要写入的图片数组
////mode:0,反色显示;1,正常显示
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
//  * 函    数：次方函数
//  * 参    数：X 底数
//  * 参    数：Y 指数
//  * 返 回 值：等于X的Y次方
//  */
//uint32_t OLED_Pow(uint32_t X, uint32_t Y)
//{
//    uint32_t Result = 1;    //结果默认为1
//    while (Y --)            //累乘Y次
//    {
//        Result *= X;        //每次把X累乘到结果上
//    }
//    return Result;
//}

///**
//  * 函    数：将OLED显存数组部分清零
//  * 参    数：X 指定区域左上角的横坐标，范围：0~127
//  * 参    数：Y 指定区域左上角的纵坐标，范围：0~63
//  * 参    数：Width 指定区域的宽度，范围：0~128
//  * 参    数：Height 指定区域的高度，范围：0~64
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
//{
//    uint8_t i, j;
//    
//    /*参数检查，保证指定区域不会超出屏幕范围*/
//    if (X > 127) {return;}
//    if (Y > 63) {return;}
//    if (X + Width > 128) {Width = 128 - X;}
//    if (Y + Height > 64) {Height = 64 - Y;}
//    
//    for (j = Y; j < Y + Height; j ++)        //遍历指定页
//    {
//        for (i = X; i < X + Width; i ++)    //遍历指定列
//        {
//            OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));    //将显存数组指定数据清零
//        }
//    }
//}

///**
//  * 函    数：OLED显示一个字符
//  * 参    数：X 指定字符左上角的横坐标，范围：0~127
//  * 参    数：Y 指定字符左上角的纵坐标，范围：0~63
//  * 参    数：Char 指定要显示的字符，范围：ASCII码可见字符
//  * 参    数：FontSize 指定字体大小
//  *           范围：OLED_8X16        宽8像素，高16像素
//  *                 OLED_6X8        宽6像素，高8像素
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize)
//{
//    if (FontSize == OLED_8X16)        //字体为宽8像素，高16像素
//    {
//        /*将ASCII字模库OLED_F8x16的指定数据以8*16的图像格式显示*/
//        OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
//    }
//    else if(FontSize == OLED_6X8)    //字体为宽6像素，高8像素
//    {
//        /*将ASCII字模库OLED_F6x8的指定数据以6*8的图像格式显示*/
//        OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
//    }
//}

///**
//  * 函    数：OLED显示字符串
//  * 参    数：X 指定字符串左上角的横坐标，范围：0~127
//  * 参    数：Y 指定字符串左上角的纵坐标，范围：0~63
//  * 参    数：String 指定要显示的字符串，范围：ASCII码可见字符组成的字符串
//  * 参    数：FontSize 指定字体大小
//  *           范围：OLED_8X16        宽8像素，高16像素
//  *                 OLED_6X8        宽6像素，高8像素
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize)
//{
//    uint8_t i;
//    for (i = 0; String[i] != '\0'; i++)        //遍历字符串的每个字符
//    {
//        /*调用OLED_ShowChar函数，依次显示每个字符*/
//        OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
//    }
//}

///**
//  * 函    数：OLED显示数字（十进制，正整数）
//  * 参    数：X 指定数字左上角的横坐标，范围：0~127
//  * 参    数：Y 指定数字左上角的纵坐标，范围：0~63
//  * 参    数：Number 指定要显示的数字，范围：0~4294967295
//  * 参    数：Length 指定数字的长度，范围：0~10
//  * 参    数：FontSize 指定字体大小
//  *           范围：OLED_8X16        宽8像素，高16像素
//  *                 OLED_6X8        宽6像素，高8像素
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
//{
//    uint8_t i;
//    for (i = 0; i < Length; i++)        //遍历数字的每一位                            
//    {
//        /*调用OLED_ShowChar函数，依次显示每个数字*/
//        /*Number / OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
//        /*+ '0' 可将数字转换为字符格式*/
//        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
//    }
//}

///**
//  * 函    数：OLED显示有符号数字（十进制，整数）
//  * 参    数：X 指定数字左上角的横坐标，范围：0~127
//  * 参    数：Y 指定数字左上角的纵坐标，范围：0~63
//  * 参    数：Number 指定要显示的数字，范围：-2147483648~2147483647
//  * 参    数：Length 指定数字的长度，范围：0~10
//  * 参    数：FontSize 指定字体大小
//  *           范围：OLED_8X16        宽8像素，高16像素
//  *                 OLED_6X8        宽6像素，高8像素
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
//{
//    uint8_t i;
//    uint32_t Number1;
//    
//    if (Number >= 0)                        //数字大于等于0
//    {
//        OLED_ShowChar(X, Y, '+', FontSize);    //显示+号
//        Number1 = Number;                    //Number1直接等于Number
//    }
//    else                                    //数字小于0
//    {
//        OLED_ShowChar(X, Y, '-', FontSize);    //显示-号
//        Number1 = -Number;                    //Number1等于Number取负
//    }
//    
//    for (i = 0; i < Length; i++)            //遍历数字的每一位                                
//    {
//        /*调用OLED_ShowChar函数，依次显示每个数字*/
//        /*Number1 / OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
//        /*+ '0' 可将数字转换为字符格式*/
//        OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
//    }
//}

///**
//  * 函    数：OLED显示十六进制数字（十六进制，正整数）
//  * 参    数：X 指定数字左上角的横坐标，范围：0~127
//  * 参    数：Y 指定数字左上角的纵坐标，范围：0~63
//  * 参    数：Number 指定要显示的数字，范围：0x00000000~0xFFFFFFFF
//  * 参    数：Length 指定数字的长度，范围：0~8
//  * 参    数：FontSize 指定字体大小
//  *           范围：OLED_8X16        宽8像素，高16像素
//  *                 OLED_6X8        宽6像素，高8像素
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
//{
//    uint8_t i, SingleNumber;
//    for (i = 0; i < Length; i++)        //遍历数字的每一位
//    {
//        /*以十六进制提取数字的每一位*/
//        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
//        
//        if (SingleNumber < 10)            //单个数字小于10
//        {
//            /*调用OLED_ShowChar函数，显示此数字*/
//            /*+ '0' 可将数字转换为字符格式*/
//            OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
//        }
//        else                            //单个数字大于10
//        {
//            /*调用OLED_ShowChar函数，显示此数字*/
//            /*+ 'A' 可将数字转换为从A开始的十六进制字符*/
//            OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
//        }
//    }
//}

///**
//  * 函    数：OLED显示二进制数字（二进制，正整数）
//  * 参    数：X 指定数字左上角的横坐标，范围：0~127
//  * 参    数：Y 指定数字左上角的纵坐标，范围：0~63
//  * 参    数：Number 指定要显示的数字，范围：0x00000000~0xFFFFFFFF
//  * 参    数：Length 指定数字的长度，范围：0~16
//  * 参    数：FontSize 指定字体大小
//  *           范围：OLED_8X16        宽8像素，高16像素
//  *                 OLED_6X8        宽6像素，高8像素
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
//{
//    uint8_t i;
//    for (i = 0; i < Length; i++)        //遍历数字的每一位    
//    {
//        /*调用OLED_ShowChar函数，依次显示每个数字*/
//        /*Number / OLED_Pow(2, Length - i - 1) % 2 可以二进制提取数字的每一位*/
//        /*+ '0' 可将数字转换为字符格式*/
//        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
//    }
//}

///**
//  * 函    数：OLED显示浮点数字（十进制，小数）
//  * 参    数：X 指定数字左上角的横坐标，范围：0~127
//  * 参    数：Y 指定数字左上角的纵坐标，范围：0~63
//  * 参    数：Number 指定要显示的数字，范围：-4294967295.0~4294967295.0
//  * 参    数：IntLength 指定数字的整数位长度，范围：0~10
//  * 参    数：FraLength 指定数字的小数位长度，范围：0~9，过长的小数会有精度丢失
//  * 参    数：FontSize 指定字体大小
//  *           范围：OLED_8X16        宽8像素，高16像素
//  *                 OLED_6X8        宽6像素，高8像素
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
//{
//    uint32_t Temp;
//    
//    if (Number >= 0)                        //数字大于等于0
//    {
//        OLED_ShowChar(X, Y, '+', FontSize);    //显示+号
//    }
//    else                                    //数字小于0
//    {
//        OLED_ShowChar(X, Y, '-', FontSize);    //显示-号
//        Number = -Number;                    //Number取负
//    }
//    
//    /*显示整数部分*/
//    OLED_ShowNum(X + FontSize, Y, Number, IntLength, FontSize);
//    
//    /*显示小数点*/
//    OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);
//    
//    /*将Number的整数部分减掉，防止之后将小数部分乘到整数时因数过大造成错误*/
//    Number -= (uint32_t)Number;
//    
//    /*将小数部分乘到整数部分，并显示*/
//    Temp = OLED_Pow(10, FraLength);
//    OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, ((uint32_t)(Number * Temp)) % Temp, FraLength, FontSize);
//}


///**
//  * 函    数：OLED显示图像
//  * 参    数：X 指定图像左上角的横坐标，范围：0~127
//  * 参    数：Y 指定图像左上角的纵坐标，范围：0~63
//  * 参    数：Width 指定图像的宽度，范围：0~128
//  * 参    数：Height 指定图像的高度，范围：0~64
//  * 参    数：Image 指定要显示的图像
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
//{
//    uint8_t i, j;
//    
//    /*参数检查，保证指定图像不会超出屏幕范围*/
//    if (X > 127) {return;}
//    if (Y > 63) {return;}
//    
//    /*将图像所在区域清空*/
//    OLED_ClearArea(X, Y, Width, Height);
//    
//    /*遍历指定图像涉及的相关页*/
//    /*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
//    for (j = 0; j < (Height - 1) / 8 + 1; j ++)
//    {
//        /*遍历指定图像涉及的相关列*/
//        for (i = 0; i < Width; i ++)
//        {
//            /*超出边界，则跳过显示*/
//            if (X + i > 127) {break;}
//            if (Y / 8 + j > 7) {return;}
//            
//            /*显示图像在当前页的内容*/
//            OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[j * Width + i] << (Y % 8);
//            
//            /*超出边界，则跳过显示*/
//            /*使用continue的目的是，下一页超出边界时，上一页的后续内容还需要继续显示*/
//            if (Y / 8 + j + 1 > 7) {continue;}
//            
//            /*显示图像在下一页的内容*/
//            OLED_DisplayBuf[Y / 8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
//        }
//    }
//}

///**
//  * 函    数：OLED使用printf函数打印格式化字符串
//  * 参    数：X 指定格式化字符串左上角的横坐标，范围：0~127
//  * 参    数：Y 指定格式化字符串左上角的纵坐标，范围：0~63
//  * 参    数：FontSize 指定字体大小
//  *           范围：OLED_8X16        宽8像素，高16像素
//  *                 OLED_6X8        宽6像素，高8像素
//  * 参    数：format 指定要显示的格式化字符串，范围：ASCII码可见字符组成的字符串
//  * 参    数：... 格式化字符串参数列表
//  * 返 回 值：无
//  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
//  */
//void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...)
//{
//    char String[30];                        //定义字符数组
//    va_list arg;                            //定义可变参数列表数据类型的变量arg
//    va_start(arg, format);                    //从format开始，接收参数列表到arg变量
//    vsprintf(String, format, arg);            //使用vsprintf打印格式化字符串和参数列表到字符数组中
//    va_end(arg);                            //结束变量arg
//    OLED_ShowString(X, Y, String, FontSize);//OLED显示字符数组（字符串）
//}













//OLED的初始化
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