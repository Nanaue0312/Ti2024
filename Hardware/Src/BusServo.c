/**
 * @file BusServo.c
 * @author nanaue0312 (nanaue0312@163.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "ti_msp_dl_config.h"
#include "BusServo.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
char cmd[20];

/**
 * @brief 总线舵机控制
 * 
 * @param cmd_type 命令类型 @ref BusServoCommandType
 * @param id 总线舵机ID
 * @param param1 参数1
 * @param param2 参数2
 */
void BusServo_SendCommand(BusServoCommandType cmd_type, uint8_t id, uint16_t param1, uint16_t param2) {
    switch (cmd_type) {
        case BusServo_CHANGE_ANGLE:
            sprintf(cmd, "#%03dP%04dT%04d!", id, param1, param2);
            break;
        case BusServo_GET_VERSION:
            sprintf(cmd, "#%03dPVER!", id);
            break;
        case BusServo_GET_ID:
            sprintf(cmd, "#%03dPID!", id); // param1 is used for new_id
            break;
        case BusServo_SET_ID:
            sprintf(cmd, "#%03dPID%03d!", id, param1); // param1 is used for new_id
            break;
        case BusServo_RELEASE_TORQUE:
            sprintf(cmd, "#%03dPULK!", id);
            break;
        case BusServo_RESUME_TORQUE:
            sprintf(cmd, "#%03dPULR!", id);
            break;
        case BusServo_GET_MODE:
            sprintf(cmd, "#%03dPMOD!", id);
            break;
        case BusServo_SET_MODE:
            sprintf(cmd, "#%03dPMOD%d!", id, param1); // param1 is used for mode
            break;
        case BusServo_GET_POSITION:
            sprintf(cmd, "#%03dPRAD!", id);
            break;
        case BusServo_PAUSE:
            sprintf(cmd, "#%03dPDPT!", id);
            break;
        case BusServo_CONTINUE:
            sprintf(cmd, "#%03dPDCT!", id);
            break;
        case BusServo_STOP:
            sprintf(cmd, "#%03dPDST!", id);
            break;
        case BusServo_SET_BAUD_RATE:
            sprintf(cmd, "#%03dPBD%d!", id, param1); // param1 is used for baud rate
            break;
        case BusServo_SET_CENTER:
            sprintf(cmd, "#%03dPSCK!", id);
            break;
        case BusServo_SET_START_POSITION:
            sprintf(cmd, "#%03dPCSD!", id);
            break;
        case BusServo_REMOVE_START_POSITION:
            sprintf(cmd, "#%03dPCSM!", id);
            break;
        case BusServo_RESET_START_POSITION:
            sprintf(cmd, "#%03dPCSR!", id);
            break;
        case BusServo_SET_MIN_VALUE:
            sprintf(cmd, "#%03dPSMI!", id);
            break;
        case BusServo_SET_MAX_VALUE:
            sprintf(cmd, "#%03dPSMX!", id);
            break;
        case BusServo_FACTORY_RESET:
            sprintf(cmd, "#%03dPCLE!", id);
            break;
        case BusServo_GET_TEMPERATURE_VOLTAGE:
            sprintf(cmd, "#%03dPRTV!", id);
            break;
        case BusServo_GET_TEMPERATURE_VOLTAGE_SETTINGS:
            sprintf(cmd, "#%03dPSTB!", id);
            break;
        case BusServo_SET_RELEASE_TORQUE_THRESHOLD:
            sprintf(cmd, "#%03dPSTB=%d!", id, param1); // param1 is used for threshold
            break;
        default:
            return; // Invalid command type
    }

    // Send the command using UART
    for (uint8_t i = 0; i < strlen(cmd); i++) {
        DL_UART_Main_transmitDataBlocking(UART_BusServo_INST, cmd[i]);
    }
}