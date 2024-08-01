#include "uart.h"
#include <stdio.h>
/******************************************************************************************/
/* ?車豕?辰???∩迆??, ?∫3?printfo‘那y, ??2?D豕辰a????use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)           /* 那1車?AC6㊣角辰??¯那㊣ */
__asm(".global __use_no_semihosting\n\t"); /* 谷迄?¯2?那1車?～??¯?迆?㏒那? */
__asm(".global __ARM_use_no_argv \n\t");   /* AC6??D豕辰a谷迄?¯maino‘那y?a?T2?那y??那?㏒?﹞??辰2?﹞?角y3足?谷?邦3???～??¯?迆?㏒那? */

#else
/* 那1車?AC5㊣角辰??¯那㊣, 辰a?迆?a角??“辰?__FILE o赤 2?那1車?～??¯?迆?㏒那? */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 2?那1車?～??¯?迆?㏒那?㏒??芍谷迄D豕辰a???“辰?_ttywrch\_sys_exit\_sys_command_stringo‘那y,辰?赤?那㊣??豕YAC6o赤AC5?㏒那? */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ?“辰?_sys_exit()辰?㊣邦?a那1車?～??¯?迆?㏒那? */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE ?迆 stdio.h角????“辰?. */
FILE __stdout;

/* MDK??D豕辰a???“辰?fputco‘那y, printfo‘那y℅????芍赤“1y米¯車?fputc那?3?℅?﹞?∩?米?∩??迆 */
int fputc(int ch, FILE *f)
{

    DL_UART_Main_transmitDataBlocking(UART_0_INST, (uint8_t)ch);
    return ch;
}
#endif
void UART0_Text(void)
{
    uint8_t send_test[13] = {"hello world!\n"};
    //    uint8_t *p={"hello world!\n"};
    UART0_Send_Bytes(send_test, sizeof(send_test));
    //    UART0_Send_Bytes(p,sizeof(p));
}

void UART0_Send_Bytes(uint8_t *buf, int len)
{
    while (len--)
    {
        DL_UART_Main_transmitDataBlocking(UART_0_INST, *buf);
        buf++;
    }
}

void UART_Init(void)
{
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN); // ???D??1辰?e
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);       // 那1?邦∩??迆?D??
}

#define IMU_DATA_LENGTH 44

uint8_t imu_data_buffer[IMU_DATA_LENGTH];
uint8_t imu_data_index = 0;

void UART_0_INST_IRQHandler(void)
{
    static uint8_t header_index = 0;

    if (DL_UART_Main_getPendingInterrupt(UART_0_INST) == DL_UART_MAIN_IIDX_RX)
    {
        uint8_t Data = DL_UART_Main_receiveData(UART_0_INST);
        DL_UART_Main_transmitData(UART_0_INST, Data); // Echo received data (for debugging)

        if (header_index == 0)
        {
            if (Data == 0x55)
            {
                imu_data_buffer[header_index++] = Data;
            }
            else
            {
                // Reset header index if the first byte is not 0x55
                header_index = 0;
                imu_data_index = 0;
            }
        }
        else if (header_index == 1)
        {
            if (Data == 0x51)
            {
                imu_data_buffer[header_index++] = Data;
                imu_data_index = 2; // Set imu_data_index to 2 to continue storing subsequent data
            }
            else
            {
                // Reset header index if the second byte is not 0x51
                header_index = 0;
                imu_data_index = 0;
            }
        }
        else
        {
            // Store received data in buffer
            imu_data_buffer[imu_data_index++] = Data;

            // If buffer is full, process the data
            if (imu_data_index == IMU_DATA_LENGTH)
            {
                parseIMUData(imu_data_buffer, &imuData);
                imu_data_index = 0; // Reset buffer index for next data frame
                header_index = 0;   // Reset header index for next header validation
				imuData.angle.yaw = -imuData.angle.yaw;
				if( imuData.angle.yaw < 0 )
					imuData.angle.yaw += 360;			
            }
        }
    }
}
