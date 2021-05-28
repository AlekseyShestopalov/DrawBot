#ifndef __DRAW_BOT_HEADER_IO__
#define __DRAW_BOT_HEADER_IO__

// status byte extention
#define ASH_STS_EXT_ZPOS      ASH_STS_EXTBYTE3
#define ASH_STS_EXT_PLANFULL  ASH_STS_EXTBYTE2

//=========================================
// ash protocol extention for DrawBot
#define DBW_ASH_CMDEXT_SERVO      0x21      // operation with servo 
#define DBW_ASH_CMDEXT_STEPPERS   0x22      // operation with stepper motors
// for DBW_ASH_CMDEXT_SERVO
#define DBW_ASH_CMDCID_SERVO_SETPOSABS      1   // operation with servo - absolute position
// for DBW_ASH_CMDEXT_STEPPERS
#define DBW_ASH_CMDCID_STEPPERS_STATE       0   // operation with steppers - get current state
#define DBW_ASH_CMDCID_STEPPERS_SETPOS      1   // operation with steppers - set position (+num)
#define DBW_ASH_CMDCID_STEPPERS_STOP        2   // operation with steppers - stop all steppers
#define DBW_ASH_CMDCID_STEPPERS_SETZERO     3   // operation with steppers - set current as zero (+num)
//=========================================

#define ASH_CMD_UCMD_PLAN    52    // plan line

#define ASH_CMD_SERVO_SETPOSABS 0   // servo absolute pos

#define ASH_CMD_MOV_STEPSTATE   0   // get steppers position
#define ASH_CMD_MOV_STEPSETPOS  1   // set one drive position
#define ASH_CMD_MOV_STEPSTOP    2   // emergency stop
#define ASH_CMD_MOV_STEPSETZERO 3   // set current pos as zero pos

#define DWB_PLAN_ACTION_DELAY   0     // delay, ms
#define DWB_PLAN_ACTION_STEPS   1     // 2 steppers move
#define DWB_PLAN_ACTION_SERVO   2     // servo move
#define DWB_PLAN_ACTION_LED13   3     // flash led 13

#endif // __DRAW_BOT_HEADER_IO__

