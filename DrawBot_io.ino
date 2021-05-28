// Arduino project with 2 steppers and pen: communication with PC
// extends "Arduino Simple Host" project
// Aleksey Shestopalov, Artem Shestopalov

int DWB_ASH_Parser(unsigned char *buf, unsigned char *outbuf, unsigned char in, unsigned char on)
{
   unsigned char cmd = buf[in++];
//   long int valA0;
//   int valA1;
   
   switch( cmd )
   {
      case ASH_CMD_UD: // 0x14 - user data - plan uploading 
        //flash13( 2, 300);
        on = DWB_ASH_Parser_UserData( buf, outbuf, in, on);
      break;
      case DBW_ASH_CMDEXT_SERVO:
        on = DWB_ASH_Parser_CMDServo( buf, outbuf, in, on);
      break;
      case DBW_ASH_CMDEXT_STEPPERS:
        on = DWB_ASH_Parser_CMDSteppers( buf, outbuf, in, on);
      break;
      default:  // unknown command
        flash13( 2, 300 );
      break;
/*      case 0x20: 
//      _flash13( 1, 300);
        valA0 = analogRead( A0 );
        valA1 = analogRead( A1 );
        outbuf[on++] = (unsigned char) (valA0 & 0xFF);
        outbuf[on++] = (unsigned char) ((valA0>>8) & 0xFF);
        outbuf[on++] = (unsigned char) ((valA0>>16) & 0xFF);
        outbuf[on++] = (unsigned char) ((valA0>>24) & 0xFF);
        outbuf[on++] = (unsigned char) (valA1 & 0xFF);
        outbuf[on++] = (unsigned char) ((valA1>>8) & 0xFF);
      break;
*/      
   }
   return on;  
}

// parse commands to servo operating
int DWB_ASH_Parser_CMDServo(unsigned char *buf, unsigned char *outbuf, unsigned char in, unsigned char on)
{
  unsigned char cid = buf[in++];
  switch(cid)
  {
      case ASH_CMD_SERVO_SETPOSABS:
        in++;  // servo num
        dbot.penpos = buf[in++]; // servo position
      break;  
  }

  return on;
}

// parse commands to steppers operating
int DWB_ASH_Parser_CMDSteppers(unsigned char *buf, unsigned char *outbuf, unsigned char in, unsigned char on)
{
  byte num;
  unsigned char cid = buf[in++];
  long int pos;
  
  switch(cid)
  {
    // get state
    case DBW_ASH_CMDCID_STEPPERS_STATE:  
      outbuf[on++] = DBW_ASH_CMDEXT_STEPPERS;
      outbuf[on++] = DBW_ASH_CMDCID_STEPPERS_STATE;
      for( int i=0; i < DWB_STEPS_COUNT; i++ )
      {
        outbuf[on++] = i;
        outbuf[on++] = dbot.steps[i].sts;
        pos = dbot.steps[i].cur_pos - dbot.steps[i].zero_pos;
        outbuf[on++] = (unsigned char) (pos & 0xFF);
        outbuf[on++] = (unsigned char) ((pos>>8) & 0xFF);
        outbuf[on++] = (unsigned char) ((pos>>16) & 0xFF);
        outbuf[on++] = (unsigned char) ((pos>>24) & 0xFF);
      }
    //flash13( 2, 300);
    break;  
    // set pos
    case DBW_ASH_CMDCID_STEPPERS_SETPOS:  
     // flash13( 3, 200);
      num = buf[in++];
      if( num >= 0 && num < DWB_STEPS_COUNT )
        dbot.steps[num].target_pos = Get4BytesAsLong(buf+in); 
      in+=4;
     break;
     case DBW_ASH_CMDCID_STEPPERS_STOP:
      for( int i=0; i < DWB_STEPS_COUNT; i++ )
        dbot.steps[i].target_pos = dbot.steps[i].cur_pos;
     break;                
     case DBW_ASH_CMDCID_STEPPERS_SETZERO:
      num = buf[in++];
      dbot.steps[num].zero_pos = dbot.steps[num].cur_pos;
      dbot.steps[num].target_pos = 0;
     break;                
  }
  return on;
}

// parse commands to plan lines
int DWB_ASH_Parser_UserData(byte *buf, byte *outbuf, byte in, byte on)
{
    unsigned char cid = buf[in++];

    switch(cid)
    {
        case ASH_CMD_UCMD_PLAN: // plan line
          if((dbot.plan_end == dbot.plan_pos-1) ||
             (dbot.plan_end == PLAN_LINE_COUNT-1 && dbot.plan_pos==0)) 
                break;  // plan full
                
          //flash13( 2, 200 );
          dbot.plan[dbot.plan_end].action = buf[in++];  
          dbot.plan[dbot.plan_end].val1 = Get4BytesAsLong(buf+in); in+=4;
          dbot.plan[dbot.plan_end].val2 = Get4BytesAsLong(buf+in); in+=4;
          if( dbot.plan_end < PLAN_LINE_COUNT-1 ) dbot.plan_end++;
          else dbot.plan_end=0;
          
          if((dbot.plan_end == dbot.plan_pos-1) ||
             (dbot.plan_end == PLAN_LINE_COUNT-1 && dbot.plan_pos==0)) 
                al->SetStateBit( ASH_STS_EXT_PLANFULL, 1 );
        break;  
    }
    return on;  
}

// get 4 bytes as long int big-endian
long int Get4BytesAsLong(byte *buf)
{
    long int b0, b1, b2, b3;
    b0 = buf[0];
    b1 = buf[1];
    b2 = buf[2];
    b3 = buf[3];
    return b0+(b1 << 8)+(b2 << 16)+(b3 << 24); 
}


