#include "Robot_Leg.h"
#include "MechSpec.h"
#include "comm.h"
#define MOTOR_SEND_ID 0x200
#define MOTOR_1_ID 0X201
#define MOTOR_2_ID 0x202

// boot up CAN bus commuication
struct can_frame  frame_1,frame_2;
struct data_read  motor_1,motor_2;

// boot up testing
int t = 0;

// boot up Robot_leg IK
RobotLeg Leg(A_LENGHT,B_LENGHT,C_LENGHT,D_LENGHT,F_LENGHT,ALPHA_ANGLE);

void setup() {
  Serial.begin(9600);
  Serial.println("------------Starting Initalizaation------------");
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setLoopbackMode();
  frame_1.can_dlc = 8;
  frame_1.can_id = MOTOR_1_ID;
  frame_2.can_dlc = 8;
  frame_2.can_id = MOTOR_1_ID;
  motor_1.ID = '1';
  motor_2.ID = '2';
  /*
  motor_1.motor_pos = &frame_1.data[0];
  motor_1.motor_spd = &frame_1.data[2];
  motor_1.motor_toq = &frame_1.data[4];
  motor_2.motor_pos = &frame_2.data[0];
  motor_2.motor_spd = &frame_2.data[2];
  motor_2.motor_toq = &frame_2.data[4];*/
  
  Serial.println("------------Initialization Finished ------------");
}

void loop() {
  // put your main code here, to run repeatedly
  Serial.println("------------New Loop started ------------");
  Serial.println(t);
  
  if( fetchInfo(frame_1, motor_1) ) logInfo(motor_1);
  else Serial.println("Fail to read motor 1 data");
  Serial.println("-----------------------------------------");
  if( fetchInfo(frame_2, motor_2) ) logInfo(motor_2);
  else Serial.println("Fail to read motor 2 data");

  
  
  t++;
  delay(500);
}
