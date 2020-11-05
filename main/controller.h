#ifndef Controller_def
#define Controller_der

#include <can.h>
#include <mcp2515.h>
MCP2515 mcp2515(10);

class Controller{
private:
  int16_t now_pos, output, last_pos, offset;
  double Pgain, Igain, Dgain, PIDout = 0, counter = 0; 
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
    error = setpoint - (counter * 8190 + now_pos) + offset;
    Serial.print("  ");
    Serial.print(error);
  }
  
  void calculate_PID(){
    PIDout  =  Pgain * error;
    error_sum += error * Igain;
    PIDout +=  error_sum;
    PIDout += Dgain * (error - last_error); 
    Serial.print("  ");
    Serial.println(PIDout);
  }
  
  int get_pos(){
    while(1){
      if(mcp2515.readMessage(frame_r) == MCP2515::ERROR_OK){
        if( frame_r->can_id != motorID ) continue;
        now_pos = (int16_t) frame_r->data[0] << 8 | frame_r->data[1];
        /*Serial.print(frame_r->can_id,HEX);
        Serial.print("  16bit Angle     : ");
        Serial.println(now_pos);
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
    Serial.print(offset);
  }

  void set_setpoint(int32_t _set){
    setpoint = _set;
  }

  void control_flow(){
    get_pos();

    if(last_pos < 800  && now_pos > 7200 ) counter--;
    if(last_pos > 7200 && now_pos < 800  ) counter++;
    
    calculate_error();
    calculate_PID();
    saturate();
    last_pos = now_pos;
    last_error = error;
  }

  short get_output(){
    return output;
  }
};


#endif
