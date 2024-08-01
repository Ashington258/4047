#include "uart.h"
#include <stdio.h>
/******************************************************************************************/
/* ?����?��???�䨲??, ?��3?printfo����y, ??2?D����a????use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)           /* ��1��?AC6������??�¨��� */
__asm(".global __use_no_semihosting\n\t"); /* ����?��2?��1��?��??��?��?�꨺? */
__asm(".global __ARM_use_no_argv \n\t");   /* AC6??D����a����?��maino����y?a?T2?��y??��?��?��??��2?��?��y3��?��?��3???��??��?��?�꨺? */

#else
/* ��1��?AC5������??�¨���, ��a?��?a��??����?__FILE o�� 2?��1��?��??��?��?�꨺? */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 2?��1��?��??��?��?�꨺?��??������D����a???����?_ttywrch\_sys_exit\_sys_command_stringo����y,��?��?����??��YAC6o��AC5?�꨺? */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ?����?_sys_exit()��?����?a��1��?��??��?��?�꨺? */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE ?�� stdio.h��????����?. */
FILE __stdout;

/* MDK??D����a???����?fputco����y, printfo����y��????������1y�̡¨�?fputc��?3?��?��?��?��?��??�� */
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
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN); // ???D??1��?e
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);       // ��1?����??��?D??
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
