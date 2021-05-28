
// start init steppers
void DWB_SteppersInit()
{
  pinMode(IN1_STOP, INPUT_PULLUP);
  pinMode(IN1_1, OUTPUT);
  pinMode(IN1_2, OUTPUT);
  pinMode(IN1_3, OUTPUT);
  pinMode(IN1_4, OUTPUT);

  pinMode(IN2_STOP, INPUT_PULLUP);
  pinMode(IN2_1, OUTPUT);
  pinMode(IN2_2, OUTPUT);
  pinMode(IN2_3, OUTPUT);
  pinMode(IN2_4, OUTPUT);
  
  dbot.steps[0].bstop = IN1_STOP;
  dbot.steps[0].bplus = IN1_Fw;
  dbot.steps[0].bminus = IN1_Bk;
  dbot.steps[0].in1 = IN1_1;
  dbot.steps[0].in2 = IN1_2;
  dbot.steps[0].in3 = IN1_3;
  dbot.steps[0].in4 = IN1_4;
  dbot.steps[0].cur_pos = 0;
  dbot.steps[0].target_pos = 0;
  
  dbot.steps[1].bstop = IN2_STOP;
  dbot.steps[1].bplus = IN2_Fw;
  dbot.steps[1].bminus = IN2_Bk;
  dbot.steps[1].in1 = IN2_1;
  dbot.steps[1].in2 = IN2_2;
  dbot.steps[1].in3 = IN2_3;
  dbot.steps[1].in4 = IN2_4;
  dbot.steps[1].zero_pos = 0;
  dbot.steps[1].cur_pos = 0;
  dbot.steps[1].target_pos = 0;
}


// set stepper position
void DWB_Stepper_SetPos( DWBStepper *st, int step_pos)
{
    int rm;
    if( step_pos >= 0 )
      rm = step_pos%8;
    else
    {
       step_pos = -step_pos; 
       rm = 8-step_pos%8; // 0=0 1=7 2=6 3=5 4=4 5=3 6=2 7=1
    }
    
    switch(rm)
    {
      default:
      case 0:
        digitalWrite(st->in1, LOW);
        digitalWrite(st->in2, LOW);
        digitalWrite(st->in3, LOW);
        digitalWrite(st->in4, HIGH);
      break;
      case 1:
        digitalWrite(st->in1, LOW);
        digitalWrite(st->in2, LOW);
        digitalWrite(st->in3, HIGH);
        digitalWrite(st->in4, HIGH);
      break;
      case 2:
        digitalWrite(st->in1, LOW);
        digitalWrite(st->in2, LOW);
        digitalWrite(st->in3, HIGH);
        digitalWrite(st->in4, LOW);
      break;
      case 3:
        digitalWrite(st->in1, LOW);
        digitalWrite(st->in2, HIGH);
        digitalWrite(st->in3, HIGH);
        digitalWrite(st->in4, LOW);
      break;
      case 4:
        digitalWrite(st->in1, LOW);
        digitalWrite(st->in2, HIGH);
        digitalWrite(st->in3, LOW);
        digitalWrite(st->in4, LOW);
      break;
      case 5:
        digitalWrite(st->in1, HIGH);
        digitalWrite(st->in2, HIGH);
        digitalWrite(st->in3, LOW);
        digitalWrite(st->in4, LOW);
      break;
      case 6:
        digitalWrite(st->in1, HIGH);
        digitalWrite(st->in2, LOW);
        digitalWrite(st->in3, LOW);
        digitalWrite(st->in4, LOW);
      break;
      case 7:
        digitalWrite(st->in1, HIGH);
        digitalWrite(st->in2, LOW);
        digitalWrite(st->in3, LOW);
        digitalWrite(st->in4, HIGH);
      break;
/*
      default:
        digitalWrite(st->in1, LOW);
        digitalWrite(st->in2, LOW);
        digitalWrite(st->in3, LOW);
        digitalWrite(st->in4, LOW);
      break;
*/      
    }//switch
}

// one stepper step
int DWB_Stepper_Step( DWBStepper *st)
{
  long int pos;
  // check energency stop for this stepper
  int v1 = !digitalRead( st->bstop );
  if( v1 ) 
    st->sts |= ASH_STS_EXT_ZPOS;
  else
   st->sts &= ~ASH_STS_EXT_ZPOS;

  // check control buttons  for this stepper
  v1 = analogRead(st->bplus);
  if( v1>500 ) st->target_pos = st->cur_pos+1;
  v1 = analogRead(st->bminus);
  if( v1>500 ) //flash13(1,300);
  st->target_pos = st->cur_pos-1;
   
  pos = st->cur_pos - st->zero_pos;  
  if( pos != st->target_pos && !(st->sts & ASH_STS_EXT_ZPOS) )
  {
      st->sts |= ASH_STS_MOVE;
      
      if( pos > st->target_pos )
        DWB_Stepper_SetPos( st, --st->cur_pos );
      else
        DWB_Stepper_SetPos( st, ++st->cur_pos );

      return 1; // move
  } 
   
  st->sts &= ~ASH_STS_MOVE;
  return 0; // not move
}


