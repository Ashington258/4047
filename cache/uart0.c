/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-08-01 12:00:37
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 12:00:44
 * @FilePath: \4047\cache\uart0.c
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
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
                header_index = 0; // Reset header index for next header validation
            }
        }
    }
}
