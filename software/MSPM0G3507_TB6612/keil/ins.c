#include "ins.h"
#include "encoder.h"
#include "led.h"


// 定义INS结构体变量
ins_struc ins;

// 定义变量n，用于表示已保存的目标点索引
uint8_t n = 1;

// 定义变量target，用于表示当前目标点索引
uint8_t target = 1;

// 定义变量flag_1，用途未知，可能用于标记状态
uint8_t flag_1 = 1;

// 定义变量ins_mode，用于表示INS的工作模式
uint8_t ins_mode = 1;

// 定义变量xunhuan_flag，用于控制循环标志
uint8_t xunhuan_flag = 0;

/**
 * 计算实时坐标
 * @param va 左轮速度
 * @param vb 右轮速度
 * @param yaw 朝向角度
 * @param delta_t 时间间隔
 * @param wheel_base 车轮基线距离
 */
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

/**
 * 获取目标点方向和距离
 * @param x1 当前x坐标
 * @param y1 当前y坐标
 * @param x2 目标x坐标
 * @param y2 目标y坐标
 */
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

    // 更新目标距离和角度
    ins.dis_ins = temp2;
    ins.yaw_ins = RAD_TO_ANGLE(temp1);

}

/**
 * INS导航函数
 * @param dis_min 最小距离阈值
 */
void ins_navigation(uint16_t dis_min)
{


    // 根据不同的工作模式执行不同的操作
    switch(ins_mode)
    {
        case 1:     //存点模式
					// 停止计时器
					DL_Timer_stopCounter( PWM_1_INST );
				
					// 如果按键状态为4，设置循环标志
					if( key_state == 4 )
						xunhuan_flag = 1;
									
					 // 设置Yao的停止标志
				    if( key_state == 2 )
            {
//								Key1_Flag = 0;
								// 重置实时坐标
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
//							flag_1 = 0;
            }
            // 如果按键状态为3，保存当前坐标
            if( key_state == 3 )      //第三个按键存点
            {
//								Key2_Flag = 0;
							if( ins.cod_realtime.x != ins.cod_saved[n-1].x && ins.cod_realtime.y != ins.cod_saved[n-1].y ){
                // 更新保存的坐标
                ins.cod_saved[n].x = ins.cod_realtime.x;
                ins.cod_saved[n].y = ins.cod_realtime.y;
								n++;
							}
                
            }
            // 如果按键状态为1，重置相关变量
            if( key_state == 1 )
            {
//								Key0_Flag = 0;
								// 重置目标点索引和实时坐标
								target = 1;
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
                // 切换工作模式
                ins_mode = 0;
								// 设置Yao的停止标志
								Yao.stop = 0;
//							flag_1 = 0;
            }
						// 设置目标速度为0
            Yao.target_speed = 0;
            break;

        case 0:
					// 启动计时器
					DL_Timer_startCounter( PWM_1_INST );
						// 重置实时坐标
						if( key_state == 2 )
            {
//								Key2_Flag = 0;
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
//							flag_2 = 0;
            }
            // 设置目标速度
            if( key_state == 1 )
            {
//								Key0_Flag = 0;
								target = 1;
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
							Yao.target_speed = 500;
            }
							// 获取目标距离和角度
            get_target(ins.cod_realtime.x,ins.cod_realtime.y,ins.cod_saved[target].x,ins.cod_saved[target].y);

            // 如果到达目标点，切换到下一个
            if(ins.dis_ins < dis_min )
            {
								LED_ON();
                target++;

                // 如果是最后一个点，并且循环标志为真，则重置目标点索引
                if( target == n  && xunhuan_flag )
                    target = 1;
								// 如果是最后一个点，并且循环标志为假，则设置停止标志
								else if( target == n  && !xunhuan_flag )
									Yao.stop = 1;

            }

            break;

        default:break;

    }
}