#include "ins.h"
#include "encoder.h"
#include "led.h"


// ����INS�ṹ�����
ins_struc ins;

// �������n�����ڱ�ʾ�ѱ����Ŀ�������
uint8_t n = 1;

// �������target�����ڱ�ʾ��ǰĿ�������
uint8_t target = 1;

// �������flag_1����;δ֪���������ڱ��״̬
uint8_t flag_1 = 1;

// �������ins_mode�����ڱ�ʾINS�Ĺ���ģʽ
uint8_t ins_mode = 1;

// �������xunhuan_flag�����ڿ���ѭ����־
uint8_t xunhuan_flag = 0;

/**
 * ����ʵʱ����
 * @param va �����ٶ�
 * @param vb �����ٶ�
 * @param yaw ����Ƕ�
 * @param delta_t ʱ����
 * @param wheel_base ���ֻ��߾���
 */
void get_realtime_coordinate(double va, double vb, double yaw, double delta_t, double wheel_base) 
{   
    // ����С�������ٶȺͽ��ٶ�
    double v = (va + vb) / 2.0;
    double omega = (vb - va) / wheel_base;

    // ����yaw�Ļ���ֵ
    double yaw_rad = yaw * PI / 180.0;

    // ��������
    double delta_x = va * delta_t * cos(yaw_rad);
    double delta_y = va * delta_t * sin(yaw_rad);

    // ����ȫ������
    ins.cod_realtime.x += delta_x;
    ins.cod_realtime.y += delta_y;

}

/**
 * ��ȡĿ��㷽��;���
 * @param x1 ��ǰx����
 * @param y1 ��ǰy����
 * @param x2 Ŀ��x����
 * @param y2 Ŀ��y����
 */
void get_target( double x1,double y1 , double x2,double y2 )
{
    double temp1,temp2;
    double dx = x2 - x1;
    double dy = y2 - y1;
    //����нǣ�����
    temp1 = atan2(dy, dx);
    temp2 = sqrt(dx * dx + dy * dy);

    // ���Ƕȷ�Χת��Ϊ 0 �� 2��
    if (temp1 < 0)
        temp1 += 2 * PI;

    // ����Ŀ�����ͽǶ�
    ins.dis_ins = temp2;
    ins.yaw_ins = RAD_TO_ANGLE(temp1);

}

/**
 * INS��������
 * @param dis_min ��С������ֵ
 */
void ins_navigation(uint16_t dis_min)
{


    // ���ݲ�ͬ�Ĺ���ģʽִ�в�ͬ�Ĳ���
    switch(ins_mode)
    {
        case 1:     //���ģʽ
					// ֹͣ��ʱ��
					DL_Timer_stopCounter( PWM_1_INST );
				
					// �������״̬Ϊ4������ѭ����־
					if( key_state == 4 )
						xunhuan_flag = 1;
									
					 // ����Yao��ֹͣ��־
				    if( key_state == 2 )
            {
//								Key1_Flag = 0;
								// ����ʵʱ����
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
//							flag_1 = 0;
            }
            // �������״̬Ϊ3�����浱ǰ����
            if( key_state == 3 )      //�������������
            {
//								Key2_Flag = 0;
							if( ins.cod_realtime.x != ins.cod_saved[n-1].x && ins.cod_realtime.y != ins.cod_saved[n-1].y ){
                // ���±��������
                ins.cod_saved[n].x = ins.cod_realtime.x;
                ins.cod_saved[n].y = ins.cod_realtime.y;
								n++;
							}
                
            }
            // �������״̬Ϊ1��������ر���
            if( key_state == 1 )
            {
//								Key0_Flag = 0;
								// ����Ŀ���������ʵʱ����
								target = 1;
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
                // �л�����ģʽ
                ins_mode = 0;
								// ����Yao��ֹͣ��־
								Yao.stop = 0;
//							flag_1 = 0;
            }
						// ����Ŀ���ٶ�Ϊ0
            Yao.target_speed = 0;
            break;

        case 0:
					// ������ʱ��
					DL_Timer_startCounter( PWM_1_INST );
						// ����ʵʱ����
						if( key_state == 2 )
            {
//								Key2_Flag = 0;
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
//							flag_2 = 0;
            }
            // ����Ŀ���ٶ�
            if( key_state == 1 )
            {
//								Key0_Flag = 0;
								target = 1;
								ins.cod_realtime.x = 0;
								ins.cod_realtime.y = 0;
							Yao.target_speed = 500;
            }
							// ��ȡĿ�����ͽǶ�
            get_target(ins.cod_realtime.x,ins.cod_realtime.y,ins.cod_saved[target].x,ins.cod_saved[target].y);

            // �������Ŀ��㣬�л�����һ��
            if(ins.dis_ins < dis_min )
            {
								LED_ON();
                target++;

                // ��������һ���㣬����ѭ����־Ϊ�棬������Ŀ�������
                if( target == n  && xunhuan_flag )
                    target = 1;
								// ��������һ���㣬����ѭ����־Ϊ�٣�������ֹͣ��־
								else if( target == n  && !xunhuan_flag )
									Yao.stop = 1;

            }

            break;

        default:break;

    }
}