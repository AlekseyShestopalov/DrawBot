// Arduino project with 2 steppers and pen
// Aleksey Shestopalov, Artem Shestopalov
#ifndef __DRAW_BOT_HEADER__
#define __DRAW_BOT_HEADER__

#define SERVO_PIN 3
#define IN1_STOP 2
#define IN1_1 7
#define IN1_2 6
#define IN1_3 5
#define IN1_4 4
#define IN1_Fw A0   //forward
#define IN1_Bk A1   //back

#define IN2_STOP 12
#define IN2_1 8
#define IN2_2 9
#define IN2_3 10
#define IN2_4 11
#define IN2_Fw A2   //forward
#define IN2_Bk A3   //back

#define DN 22       // device number
#define PLAN_LINE_COUNT   100
#define DWB_STEPS_COUNT   2
#define STEPPER_PERIODms  1       // период между шагами шагового двигателя

typedef struct {
  unsigned char bstop;  // stop button
  unsigned char bplus;  // move forward button pin
  unsigned char bminus; // move backward button pin
  unsigned char in1;
  unsigned char in2;
  unsigned char in3;
  unsigned char in4;  
  unsigned char sts;
  long int zero_pos;    // zero position
  long int cur_pos;     // current position
  long int target_pos;  // target position
} DWBStepper;

// one string of device plan
typedef struct {
  unsigned char action; // current action: steppers, servo, or other action 
  long int val1;        // stepper0 for PLAN_ACTION_STEPS, position for PLAN_ACTION_SERVO
  long int val2;        // stepper1 for PLAN_ACTION_STEPS
} planStr;    

// device structure
typedef struct {
  DWBStepper steps[DWB_STEPS_COUNT];      // steppers-on-board data
  unsigned char penpos;                   // position of pen servo
  // plan to  do
  planStr plan[PLAN_LINE_COUNT];  // plan, cicled array       
  unsigned char plan_pos;         // current plan line num
  unsigned char plan_end;         // plan last line num
  
  unsigned long int pause;        // pause after plan line for moving, millis
} DBot;

#endif // __DRAW_BOT_HEADER__

