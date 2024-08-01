#ifndef __CONTROL_H
#define __CONTROL_H

#include "ti_msp_dl_config.h"
#include "board.h"


int16_t Get_Encoder();
void Turn_left(int16_t Aver_Encoder,int16_t v);
void Turn_right(int16_t Aver_Encoder,int16_t v);
void Speed(int16_t EncoderA,int16_t EncoderB);


#endif