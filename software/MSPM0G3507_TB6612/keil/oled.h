#ifndef __OLED_H
#define __OLED_H 

#include "board.h"
#include "stdlib.h"	
#include "ti_msp_dl_config.h"
//#include "OLED_Data.h"
//#include "stdio.h"
//#include "stdarg.h"


#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif
//-----------------OLED端口定义---------------- 

///* Port definition for Pin Group OLED */
//#define OLED_PORT                                                        (GPIOA)

///* Defines for SCL: GPIOA.0 with pinCMx 1 on package pin 33 */
//#define OLED_SCL_PIN                                             (DL_GPIO_PIN_15)
//#define OLED_SCL_IOMUX                                            (IOMUX_PINCM37)
///* Defines for SDA: GPIOA.1 with pinCMx 2 on package pin 34 */
//#define OLED_SDA_PIN                                             (DL_GPIO_PIN_16)
//#define OLED_SDA_IOMUX                                            (IOMUX_PINCM38)


#define OLED_SCL_Clr() DL_GPIO_clearPins(OLED_PORT,OLED_SCL_PIN)//SCL
#define OLED_SCL_Set() DL_GPIO_setPins(OLED_PORT,OLED_SCL_PIN)

#define OLED_SDA_Clr() DL_GPIO_clearPins(OLED_PORT,OLED_SDA_PIN)//DIN
#define OLED_SDA_Set() DL_GPIO_setPins(OLED_PORT,OLED_SDA_PIN)

//#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//RES
//#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16                8
#define OLED_6X8                6

/*IsFilled参数数值*/
#define OLED_UNFILLED            0
#define OLED_FILLED                1


void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);

u32 OLED_Pow(u32 m,u32 n);

void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,int32_t num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_ShowSignedNum(uint8_t x, uint8_t y, int32_t Number, uint8_t Length, u8 size1,u8 mode);

//void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize);
//void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize);
//void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
//void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
//void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
//void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
//void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
//void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese);
//void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
//void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...);


void OLED_Init(void);

#endif

