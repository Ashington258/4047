#include "led.h"
/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：V1.0
修改时间：2024-07-019

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update：2024-07-019

All rights reserved
***********************************************/
#include "led.h"

uint8_t count_flag = 0;


void LED_OFF(void)
{
	DL_GPIO_clearPins(LED1_PORT,LED1_PIN_0_PIN);
	DL_GPIO_clearPins(BUZZER_PORT,BUZZER_BUZZER_PIN_8_PIN);
}

void LED_ON(void)
{
	count_flag = 1;
	DL_GPIO_setPins(LED1_PORT,LED1_PIN_0_PIN);
	DL_GPIO_setPins(BUZZER_PORT,BUZZER_BUZZER_PIN_8_PIN);
}

void LED_Toggle(void)
{
	DL_GPIO_togglePins(LED1_PORT,LED1_PIN_0_PIN);
}

void LED_Flash(uint16_t time)
{
	static uint16_t temp;
	if(time==0) LED_ON();
	else if(++temp==time) LED_Toggle(),temp=0;
}

