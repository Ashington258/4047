#ifndef _ENCODER_H
#define _ENCODER_H
#include "ti_msp_dl_config.h"
#include "board.h"


bool compare_arrays(uint8_t array1[4], uint8_t array2[4]);
void openrational_decision(uint8_t sensor_vector[4]);

#endif