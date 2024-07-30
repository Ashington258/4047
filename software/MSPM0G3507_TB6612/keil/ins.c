#include "ins.h"
#include "Key.h"
#include <math.h>
#include <stdint.h>

ins_struc ins;

uint8_t n = 1;
uint8_t target = 1;
uint8_t ins_mode = 0;
uint8_t flag_1;

void get_realtime_coordinate(double va, double vb, double yaw, double delta_t, double wheel_base) 
{   
    // 计算小车的线速度和角速度
    double v = (va + vb) / 2.0;
    double omega = (vb - va) / wheel_base;

    // 计算yaw的弧度值
    double yaw_rad = yaw * PI / 180.0;

    // 计算增量
    double delta_x = v * delta_t * cos(yaw_rad);
    double delta_y = v * delta_t * sin(yaw_rad);

    // 更新全局坐标
    ins.cod_realtime.x += delta_x;
    ins.cod_realtime.y += delta_y;

}
void get_target( double x1,double y1 , double x2,double y2 )
{
    double temp1,temp2;
    double dx = x2 - x1;
    double dy = y2 - y1;
    //计算夹角，距离
    temp1 = atan2(dy, dx);
    temp2 = sqrt(dx * dx + dy * dy);

    // 将角度范围转换为 0 到 2π
    if (temp1 < 0)
        temp1 += 2 * PI;

    ins.dis_ins = temp2;
    ins.yaw_ins = RAD_TO_ANGLE(temp1);

}

void ins_navigation(uint8_t dis_min)
{

    switch(ins_mode)
    {
        case 0:     //存点模式
            if( Key2_Flag == 1 )      //第三个按键存点
            {
								Key2_Flag = 0;
                ins.cod_saved[n].x = ins.cod_realtime.x;
                ins.cod_saved[n].y = ins.cod_realtime.y;
                n++;
            }
            if( Key0_Flag == 1 )
            {
								Key0_Flag = 0;
								target = 1;
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
                ins_mode = 1;
            }
            break;

        case 1:
            if( Key0_Flag == 1 )
            {
								Key0_Flag = 0;
								target = 1;
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
            }
//             if(flag_1)                  //每次上电进入导航模式时，读取一次flash
//             {
//                 flash_read_page_to_buffer(0, 10);        //读存点数
//                 n = flash_union_buffer[0].uint8_type;
//                 flash_buffer_clear();

//                 for( uint8 nnn = 0; nnn < n; nnn++)     //读坐标
//                 {
//                     flash_read_page_to_buffer(0, 0);
//                     cod_target[nnn].x = readFlash_to_double1(0,nnn);
//                     cod_target[nnn].y = readFlash_to_double1(1,nnn);
//                 }
//                 flash_buffer_clear();

// //                flash_read_page_to_buffer(0, 2);        //读目标点
// //                target = flash_union_buffer[0].uint8_type;
// //                if( target == 0 )
// //                    target = 1;
// //                flash_buffer_clear();
//                 flag_1 = 0;
//             }
            //得到目标距离和角度
            get_target(ins.cod_realtime.x,ins.cod_realtime.y,ins.cod_saved[target].x,ins.cod_saved[target].y);

            if(ins.dis_ins < dis_min &&ins.dis_ins != 0)         //到达目标点，切换到下一个
            {
                // speed_flag.flag_1_yw = 1;
                // if( target == 0 )                   //已经回库，倒地
                //     Jarvis.dynamic_pitch_angle = 20;

                target++;

                if( target == (n+1) )               //最后一个点，下一个点为库
                    target = 1;

            }

            break;

        default:break;

    }
}
