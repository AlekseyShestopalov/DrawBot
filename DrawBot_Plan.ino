// Arduino project with 2 steppers and pen: plan processing
// Aleksey Shestopalov, Artem Shestopalov

#include "DrawBot.h"
#include "DrawBot_io.h"

void DWB_Plan_DoLine(DBot *dbot)
{
  if( dbot->plan_pos == dbot->plan_end ) return; // nothing to do
  planStr  *str = &dbot->plan[dbot->plan_pos];
  unsigned char ready2next=0;

  switch( str->action ) 
  {
    case DWB_PLAN_ACTION_DELAY:  // delay, ms
      dbot->pause  = str->val1;
      ready2next = 1;
    break;
    case DWB_PLAN_ACTION_STEPS:  // 2 steppers move
      if( str->val1 == dbot->steps[0].cur_pos && str->val2 == dbot->steps[1].cur_pos )
        ready2next = 1;     // ready 2 next because steppres finished
      else
      {
          dbot->steps[0].target_pos = str->val1;
          dbot->steps[1].target_pos = str->val2;
      }
    break;
    case DWB_PLAN_ACTION_SERVO:  // servo move
      dbot->penpos = (unsigned char) str->val1;
      dbot->pause  = str->val2;
      ready2next = 1;   // ready 2 next after pause
    break;
    case DWB_PLAN_ACTION_LED13:  // test: flash led 13
      digitalWrite(13, str->val1 );
      dbot->pause  = str->val2;
      ready2next = 1;   // ready 2 next after pause
    break;
  }
  
  // move to next line if ready
  if( ready2next )
  {
    if(dbot->plan_pos == (PLAN_LINE_COUNT - 1) )  
      dbot->plan_pos = 0;
    else
      dbot->plan_pos++;
  }
}

//LED on port 13 - flash count times 
int flash13(int count, int delay_ms)
{
  for( int i=0; i<count; i++ )
  {    
    digitalWrite(13, 1);
    delay(delay_ms);
    digitalWrite(13, 0);
    delay(delay_ms);
  }
}
