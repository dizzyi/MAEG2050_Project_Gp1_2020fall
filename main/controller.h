#ifndef Controller_def
#define Controller_der

#include <can.h>
#include <mcp2515.h>
MCP2515 mcp2515(10);

class Controller{
private:
  int16_t now_pos, output, last_pos, offset, now_spd = 0, last_spd = 0;
  long acc_count=0;//------
  double Pgain, Igain, Dgain, PIDout = 0, counter = 0, Iout = 0; 
  int32_t error_sum = 0, error, setpoint, last_error = 0, Iterm;
  struct can_frame * frame_r;
  const int32_t motorID;
  unsigned long last_time = 0;
  
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
    error = setpoint - acc_count ;
    //Serial.print("  total: ");
    //Serial.println(counter * 8192 + now_pos);
  }
  
  void calculate_PID(){
    PIDout  =  Pgain * error;
    
    error_sum = ( error_sum + error ) * (abs(error) > 300);

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
        now_spd = (int16_t) frame_r->data[2] << 8 | frame_r->data[3];/*
        Serial.print(motorID);
        Serial.print("  Raw Angle : ");
        Serial.print(now_pos);*/
        return 0;  
      }
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
    last_pos = now_pos;
  }

  void set_setpoint(double _set){
    setpoint = _set / 360. * 8192 * 19;
  }

  void control_flow(){
    get_pos();

    now_spd = now_pos - last_pos;

    if( abs(now_spd) > 4096){
      if(now_spd > 0 && last_spd < 0) last_spd = now_spd - 8192;
      if(now_spd < 0 && last_spd > 0) last_spd = now_spd + 8192;
      //if(now_spd * last_spd < 0) last_spd = now_spd + (last_spd/abs(last_spd))* 8192;
    }
    else last_spd = now_spd;
    
    last_pos = now_pos;
    
    acc_count += last_spd;
    /*
    Serial.print(motorID);
    Serial.print(" ");
    Serial.print(acc_count);
    Serial.print(" ");
    */
    calculate_error();
    calculate_PID();
    saturate();
    last_error = error;
  }

  short get_output(){
    return output;
  }
};


#endif
