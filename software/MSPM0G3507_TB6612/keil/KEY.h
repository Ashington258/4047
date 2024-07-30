#ifndef __KEY_H_
#define __KEY_H_

#include <stdint.h>
#include <ti/driverlib/driverlib.h>

#define KEY0					DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_18) 
#define KEY1					DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_21) 
#define KEY2					DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_19) 
#define KEY3					DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_22) 

typedef enum{
    key_release,
    key_press,
    key_wait,
}Key_state;

extern uint8_t Key0_Flag,//������־ ���������
Key1_Flag,
Key2_Flag,
Key3_Flag;

extern  uint8_t  Key_value;

void KEY_Init(void);
void Key_Scan(void);


//Key_value = 1,2,3,4��ʾKEY0,1,2,3
//Key_Flag =1 ��ʾ�̰�   Key_Flag =2 ��ʾ����
#endif

