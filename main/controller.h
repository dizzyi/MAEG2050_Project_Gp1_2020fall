#ifndef Controller_def
#define Controller_der

#include <can.h>
#include <mcp2515.h>
MCP2515 mcp2515(10);

class Controller{
private:
  int16_t setpoint, now_pos, error, output, last_pos;
  double Pgain, Igain, Dgain, PIDout = 0; 
  int32_t error_sum = 0;
  struct can_frame * frame_r;
  const int32_t motorID;
  
  void saturate(){
    if (PIDout >  ALL_SIG) output = (int16_t) ALL_SIG;
    if (PIDout < -ALL_SIG) output = (int16_t)-ALL_SIG;
    output = (int16_t)round(PIDout);
  }
  
  void calculate_error(){
    error = setpoint - now_pos;
  }
  
  void calculate_PID(){
    PIDout = Pgain * error;
  }
  
  int get_pos(){
    frame_r->can_id = motorID;
    if (mcp2515.readMessage(frame_r) == MCP2515::ERROR_OK) {
      now_pos = (int16_t) frame_r->data[0] << 8 | frame_r->data[1];
      Serial.print("16bit Angle     : ");
      Serial.println(now_pos);
      Serial.print("HEX             : ");
      Serial.println(now_pos,HEX);
      Serial.print("mechanical angle: ");
      Serial.println( now_pos / 8191.0 * 360.0 , 2);
      return 0;
    }
    else{
      return 1;
    }
  }
  
public:
  Controller(int32_t _id, double _p, double _i, double _d, struct can_frame * _FR):motorID(_id){
    Pgain = _p;
    Igain = _i;
    Dgain = _d;
    frame_r = _FR; 
  };

  void set_setpoint(int16_t _set){
    setpoint = _set;
  }

  void control_flow(){
    if( ! get_pos() );
    else {
      Serial.println("ERROR reading");
      return;
    }
    calculate_error();
    calculate_PID();
    saturate();
  }

  short get_output(){
    return output;
  }
};


#endif
