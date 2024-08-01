#include "ins.h"
#include "encoder.h"
#include "led.h"


ins_struc ins;

uint8_t n = 1;
uint8_t target = 1;

uint8_t flag_1 = 1;

uint8_t ins_mode = 1;

uint8_t xunhuan_flag = 0;

void get_realtime_coordinate(double va, double vb, double yaw, double delta_t, double wheel_base) 
{   
    // 计算小车的线速度和角速度
    double v = (va + vb) / 2.0;
    double omega = (vb - va) / wheel_base;

    // 计算yaw的弧度值
    double yaw_rad = yaw * PI / 180.0;

    // 计算增量
    double delta_x = va * delta_t * cos(yaw_rad);
    double delta_y = va * delta_t * sin(yaw_rad);

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

void ins_navigation(uint16_t dis_min)
{


    switch(ins_mode)
    {
        case 1:   //存点模式
			DL_Timer_stopCounter( PWM_1_INST );
			if( key_state == 4 )
				xunhuan_flag = 1;				
				Yao.stop = 1;
			if( key_state == 2 )
            {
//				Key1_Flag = 0;
				ins.cod_realtime.x = 0;
				ins.cod_realtime.y = 0;
//				flag_1 = 0;
            }
            if( key_state == 3 )    //第三个按键存点
            {
//				Key2_Flag = 0;
				if( ins.cod_realtime.x != ins.cod_saved[n-1].x && ins.cod_realtime.y != ins.cod_saved[n-1].y )
				{
					ins.cod_saved[n].x = ins.cod_realtime.x;
					ins.cod_saved[n].y = ins.cod_realtime.y;
					n++;
				}
            }
            if( key_state == 1 )
            {
//				Key0_Flag = 0;
				target = 1;
				ins.cod_realtime.x = 0;
				ins.cod_realtime.y = 0;
                ins_mode = 0;
				Yao.stop = 0;
//				flag_1 = 0;
            }
			Yao.target_speed = 0;
            break;

        case 0:	
			DL_Timer_startCounter( PWM_1_INST );
			if( key_state == 2 )
            {
//				Key2_Flag = 0;
				ins.cod_realtime.x = 0;
				ins.cod_realtime.y = 0;
//				flag_2 = 0;
            }
            if( key_state == 1 )
            {
//				Key0_Flag = 0;
				target = 1;
				ins.cod_realtime.x = 0;
				ins.cod_realtime.y = 0;
				Yao.target_speed = 500;
            }
			
            //得到目标距离和角度
            get_target(ins.cod_realtime.x,ins.cod_realtime.y,ins.cod_saved[target].x,ins.cod_saved[target].y);

            if(ins.dis_ins < dis_min )//到达目标点，切换到下一个
            {
				LED_ON();
                target++;

				if( target == n  && xunhuan_flag )//最后一个点，下一个点为库
					target = 1;
				else if( target == n  && !xunhuan_flag )
					Yao.stop = 1;
            }
            break;

        default:break;

    }
}
