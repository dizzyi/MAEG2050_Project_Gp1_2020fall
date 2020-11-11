#ifndef Controller_def
#define Controller_der

#include <can.h>
#include <mcp2515.h>
MCP2515 mcp2515(10);

class Controller{
private:
  int16_t now_pos, output, last_pos, offset, now_spd, last_spd, cross;
  long acc_count=0;//------
  double Pgain, Igain, Dgain, PIDout = 0, counter = 0, Iout = 0; 
  int32_t error_sum = 0, error, setpoint, last_error = 0, Iterm;
  struct can_frame * frame_r;
  const int32_t motorID;
  
  void saturate(){
    output = (int16_t)round(PIDout);
    output = (PIDout >   ALL_SIG) * ( (short) ALL_SIG - output )
           + (PIDout < - ALL_SIG) * ( (short)-ALL_SIG - output )
           +                                            output  ;
    /*
    if (PIDout >  ALL_SIG) output = (int16_t) ALL_SIG;
    if (PIDout < -ALL_SIG) output = (int16_t)-ALL_SIG;
    output = (int16_t)round(PIDout);*/
  }
  
  void calculate_error(){
    error = setpoint - (counter * 8192 + now_pos) + offset;
    error = setpoint - acc_count + offset;
    Serial.print("  total: ");
    Serial.println(counter * 8192 + now_pos);
  }
  
  void calculate_PID(){
    PIDout  =  Pgain * error;
    
    error_sum += error;

    Iout = error_sum * Igain;
 
    Iout = (Iout >  WINDUP) * ( WINDUP - Iout)
         + (Iout < -WINDUP) * (-WINDUP - Iout)
         +                               Iout ;
    
    PIDout +=  Iout;
    
    PIDout += Dgain * (error - last_error); 
  }
  
  int get_pos(){
    while(1){
      if(mcp2515.readMessage(frame_r) == MCP2515::ERROR_OK){
        if( frame_r->can_id != motorID ) continue;
        now_pos = (int16_t) frame_r->data[0] << 8 | frame_r->data[1];
        now_spd = (int16_t) frame_r->data[2] << 8 | frame_r->data[3];
        //Serial.print(frame_r->can_id,HEX);
        Serial.print("16bit Angle     : ");
        Serial.print(now_pos);
        Serial.print("   speed : ");
        Serial.print(now_spd);
        /*Serial.print("HEX             : ");
        Serial.println(now_pos,HEX);
        Serial.print("mechanical angle: ");
        Serial.println( now_pos / 8191.0 * 360.0 , 2);*/
        return 0;  
      }
      //else Serial.println("reading error");
    };
  }

  
public:
  Controller(int32_t _id, double _p, double _i, double _d, struct can_frame * _FR):motorID(_id){
    Pgain = _p;
    Igain = _i;
    Dgain = _d;
    frame_r = _FR; 
  };

  void boot(){
    get_pos();
    offset = now_pos;
    //acc_count = offset;//------
  }

  void set_setpoint(int32_t _set){
    setpoint = _set / 360. * 8192 * 19;
  }

  void control_flow(){
    get_pos();
    acc_count += (now_pos - last_pos);//------
    if( abs(now_pos - last_pos)>4096 ){
      acc_count += 8192*((now_pos - last_pos)>0?-1:1);
    }
    Serial.print(" Acc: ");Serial.print(acc_count);
    //cc_count += ;//------
    /*
    Serial.print(last_spd);
    Serial.print("  ");
    */
    
    cross = 0;
    if( (now_pos - last_pos) > (4096) ) cross =  1;
    if( (now_pos - last_pos) < (-4096) ) cross = -1;
    
    if(cross ==  1 && last_spd < 0) counter--;
    if(cross == -1 && last_spd > 0) counter++;
    cross = 0;
    
    calculate_error();
    calculate_PID();
    saturate();
    last_pos = now_pos;
    last_error = error;
    last_spd = now_spd;
  }

  short get_output(){
    return output;
  }
};


#endif
