// Arduino project with 2 steppers and pen
// Aleksey Shestopalov, Artem Shestopalov
#include <ASH_link.h>           // "Arduino Simple Host" project lib
#include <Servo.h>
#include "DrawBot.h"            // project definitions
#include "DrawBot_io.h"         // communication with PC throw ASH_link lib

ASH_link *al;                   // "Arduino Simple Host" - communication with PC
Servo PenServo;
DBot dbot;

unsigned long timer;
unsigned long pause_timer=0;
unsigned long pause = 0;

void setup()
{
  pinMode( 13, OUTPUT);       // built-in LED for testing              
  Serial.begin(115200);
  PenServo.attach(SERVO_PIN);
  DWB_SteppersInit();
// delay(1000);
  al = new ASH_link( DN );
  al->SetParser( DWB_ASH_Parser ); // parser for command extention of ASH_link
//  flash13( 5, 500);
  timer = millis();
  al->SetStateBit( ASH_STS_READY, 1 );       // set ready flag in sts byte
}

void loop()
{
  int v1;
  al->CheckIncomming();  
  if( millis()-timer <= STEPPER_PERIODms ) return; 

  timer = millis();    
  int mv=0;
  if( pause==0 || timer-pause_timer>pause )
  {
    pause = 0;
    DWB_Plan_DoLine(&dbot);    // do next line in uploaded plan
    // flash13(1,10);      
  }
  
  if((dbot.plan_pos != dbot.plan_end+1) &&                        // check the free plase on plan
     !(dbot.plan_pos == 0 && dbot.plan_end==PLAN_LINE_COUNT-1)) 
        al->SetStateBit( ASH_STS_EXT_PLANFULL, 0 );               // ready to upload new lines
        
  for( int i=0; i< DWB_STEPS_COUNT; i++ )             // for all steppers
      mv += DWB_Stepper_Step( &dbot.steps[i] );       // doing one step to target_pos
  al->SetStateBit( ASH_STS_MOVE, mv );                // if somthing move set flag in sts
  PenServo.write(dbot.penpos);                        // set current servo position
  
  // do pause in need
  if( dbot.pause ) 
  { pause = dbot.pause; pause_timer = millis(); dbot.pause = 0;  /*flash13(1,100);*/}
}

