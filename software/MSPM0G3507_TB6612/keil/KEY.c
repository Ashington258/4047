#include "key.h"


Key_state key_s = key_release;
uint8_t Key0_Flag=0,//������־ ��������
Key1_Flag=0,
Key2_Flag=0,
Key3_Flag=0,
Key4_Flag=0;

uint8_t  Key_flag,//���̰�
        Key_value;//��˭;
uint16_t  Key_time;


void KEY_Init(void)
{
    //DL_GPIO_initDigitalInput(IOMUX_PINCM49);
  DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM49, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
}

static uint8_t KEY_Read_All(void)
{
   uint8_t tm = 0;

   tm = ((!KEY0) | ((!KEY1) << 1) | ((!KEY2) << 2) | ((!KEY3) << 3)); // ��ȡ��������״̬������
   if (tm == 0x0F)
   {
       return 0;
   }
   return (~tm) & 0x0F;
}


void Key_Scan(void)         //���̰�����ɨ�躯������Ҫ����10-20ms�ж���ִ��
{

    
    switch(key_s)
    {
        case key_release:   if(KEY_Read_All()>0)key_s = key_press;  break;
        case key_press:     if(!KEY0)      Key_value = 1;
                            else if(!KEY1) Key_value = 2;
														else if(!KEY2) Key_value = 3;
														else if(!KEY3) Key_value = 4;
                            else
                            {
                                key_s = key_press;
                                break;
                            }
                            key_s = key_wait;
                            break;
        case key_wait:      if(KEY_Read_All()>0)
                            {
                                Key_time += 20;
                                break;
                            }
                            if(Key_time > 300) Key_flag = 2;   //����  //���ڳ�������Ӧʱ��
                            else               Key_flag = 1;
                            key_s = key_release;
                            Key_time = 0;
                            break;
    }
    
    // ���İ���״̬
    if((Key_flag == 1) && (Key_value == 1)) 
        Key0_Flag = 1;
    else if((Key_flag == 2) && (Key_value == 1)) 
        Key0_Flag = 2;
    
    if((Key_flag == 1) && (Key_value == 2)) 
        Key1_Flag = 1;
    else if((Key_flag == 2) && (Key_value == 2)) 
        Key1_Flag = 2; 
    
    if((Key_flag == 1) && (Key_value == 3)) 
        Key2_Flag = 1;
    else if((Key_flag == 2) && (Key_value == 3)) 
        Key2_Flag = 2;
    
    if((Key_flag == 1) && (Key_value == 4)) 
        Key3_Flag = 1;
    else if((Key_flag == 2) && (Key_value == 4)) 
        Key3_Flag = 2;
		
		Key_value = 0;
    
}














