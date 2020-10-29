#include <SPI.h>
#include "Robot_Leg.h"
#include "MechSpec.h"
#include "comm.h"
#define MOTOR_SEND_ID 0x200
#define MOTOR_1_ID 0X201
#define MOTOR_2_ID 0x202

struct can_frame  frame_1,frame_2, frame_s;
struct data_read  motor_1,motor_2;
struct data_send  motor_s;

String Input;

// boot up testing
int t = 0;

// boot up Robot_leg IK
RobotLeg Leg(A_LENGHT,B_LENGHT,C_LENGHT,D_LENGHT,F_LENGHT,ALPHA_ANGLE);

void setup() {
  Serial.begin(115200);
  //SPI.setClockDivider(10,84); 
  SPI.begin();
  Serial.println("------------Starting Initalizaation------------");
  mcp2515.reset();
  Serial.println("----"); 
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  Serial.println("----");
  mcp2515.setNormalMode();
  frame_1.can_dlc = 8;
  frame_1.can_id = MOTOR_1_ID;
  frame_2.can_dlc = 8;
  frame_2.can_id = MOTOR_2_ID;
  frame_s.can_dlc = 8;
  frame_s.can_id = MOTOR_SEND_ID;
  motor_1.ID = '1';
  motor_2.ID = '2';

  motor_s.m1 = 0;
  motor_s.m2 = 0;
  motor_s.m3 = 0;
  motor_s.m4 = 0;
  
  Serial.println("------------Initialization Finished ------------");
}

void loop() {
  // put your main code here, to run repeatedly
  Serial.println("------------New Loop started ------------");
  //Serial.println(t);
  
  if( getInfo(&frame_1, &motor_1) )logInfo(&motor_1);
  else Serial.println("Fail to read motor 1 data");
  Serial.println("-----------------------------------------");
  if( getInfo(&frame_2, &motor_2) )logInfo(&motor_2);
  else Serial.println("Fail to read motor 2 data");
  /*
  if(Serial.available()>0){
    Input = Serial.readString();
    Serial.print("The String is :");
    Serial.print(Input);
    Serial.println(":");
    
    if (Input[0] == '1' ) motor_s.m1 = 300;
    if (Input[0] == '0' ) motor_s.m1 = 0;
  }
  */ 
  motor_s.m1 = 400;
  motor_s.m2 = 400;
  postInfo(&frame_s, &motor_s);
  
  //for(int i = 0; i < 8 ; i++) Serial.println(frame_s.data[i],HEX);
  Serial.println(mcp2515.sendMessage(&frame_s));
  PrintHex8(frame_s.data, 8);
  Serial.println("");
  t++;
  delay(1000);
}
