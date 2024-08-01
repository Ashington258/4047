
#ifndef __ins_navi_h_
#define __ins_navi_h_

#include "itrt.h"
#include "Key.h"
#include <math.h>
#include <stdint.h>
#include <ti/driverlib/driverlib.h>
#include "ti_msp_dl_config.h"
#include "board.h"
#define ANGLE_TO_RAD(x)     ( (x) * PI / 180.0 )                                // 角度转换为弧度
#define RAD_TO_ANGLE(x)     ( (x) * 180.0 / PI )                                // 弧度转换为角度
#define PI                  ( 3.1415926535898 )

typedef struct {
    double x;
    double y;
} Coordinates;

typedef struct {
Coordinates cod_realtime;
Coordinates cod_saved[10];
double dis_ins;
double yaw_ins;
} ins_struc;

void get_realtime_coordinate(double va, double vb, double yaw, double delta_t, double wheel_base);
void get_target( double x1,double y1 , double x2,double y2 );
void ins_navigation(uint16_t dis_min);

extern ins_struc ins;
extern uint8_t flag_1;
extern uint8_t n;
extern uint8_t target;
extern uint8_t ins_mode;



#endif
