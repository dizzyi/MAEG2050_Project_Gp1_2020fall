#include <SPI.h>
#include "MechSpec.h"
#include "Commander.h"
#include "Controller.h"
#define MOTOR_SEND_ID 0x200
#define MOTOR_1_ID 0X201
#define MOTOR_2_ID 0x202

struct can_frame  frame_read, frame_send;
Controller motor_1(MOTOR_1_ID, 1, 0, 0, &frame_read);

int t = 0;

Commander Leg(A_LENGHT,B_LENGHT,C_LENGHT,D_LENGHT,F_LENGHT,ALPHA_ANGLE);

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
  
  frame_read.can_dlc = 8;
  frame_send.can_dlc = 8;
  frame_send.can_id = MOTOR_SEND_ID;
  
  Serial.println("------------Initialization Finished ------------");
}

void loop() {
  // put your main code here, to run repeatedly
  Serial.print("------------New Loop started ------------");
  Serial.println(t);
  
  motor_1.set_setpoint( Leg.get_setpoint() );
  motor_1.control_flow();

  frame_send.data[0] = motor_1.get_output() >> 8;
  frame_send.data[1] = motor_1.get_output();
  frame_send.data[2] = 0;
  frame_send.data[3] = 0;
  frame_send.data[4] = 0;
  frame_send.data[5] = 0;
  frame_send.data[6] = 0;
  frame_send.data[7] = 0;

  mcp2515.sendMessage(&frame_send);
  
  t++;
  //delay(1000);
}
