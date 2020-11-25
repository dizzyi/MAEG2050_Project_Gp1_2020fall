#include <SPI.h>
#include "MechSpec.h"
#include "Commander.h"
#include "Controller.h"
#define MOTOR_SEND_ID 0x200
#define MOTOR_1_ID 0X201
#define MOTOR_2_ID 0x202
#define PGAIN 0.4
#define IGAIN 0.1
#define DGAIN 0.1

struct can_frame  frame_read, frame_send;
Controller motor_1(MOTOR_1_ID, PGAIN, IGAIN, DGAIN, &frame_read),
           motor_2(MOTOR_2_ID, PGAIN, IGAIN, DGAIN, &frame_read);

long t = 0;

Commander Leg(A_LENGHT,B_LENGHT,C_LENGHT,D_LENGHT,F_LENGHT,ALPHA_ANGLE);

void setup() {
  Serial.begin(2000000);
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
  motor_1.boot();
  motor_2.boot();
  Serial.println("------------Initialization Finished ------------");
}

void loop() {

  Leg.UpdateXY( (t/10) % 360 );
  
  Serial.print("X: ");
  Serial.print(Leg.getX());
  Serial.print(" Y: ");
  Serial.print(Leg.getY());
  Serial.print(" ");

  Serial.print("Xe: ");
  Serial.print(Leg.getXe());
  Serial.print(" Ye: ");
  Serial.print(Leg.getYe());
  Serial.print(" ");
  
  Leg.UpdateAng();  
  
  Serial.print("  phi: ");
  Serial.print(Leg.get_phi());
  Serial.print("   theta: ");
  Serial.print(Leg.get_theta());
  Serial.print("  ");
  
  /*
  motor_1.set_setpoint( Leg.get_phi((t/10) % 360 ) );
  motor_1.control_flow();
  motor_2.set_setpoint( Leg.get_theta((t/10) % 360 ) );
  motor_2.control_flow();
  */
  motor_1.set_setpoint( Leg.get_phi() );
  motor_1.control_flow();
  motor_2.set_setpoint( Leg.get_theta() );
  motor_2.control_flow();

  frame_send.data[0] = motor_1.get_output() >> 8;
  frame_send.data[1] = motor_1.get_output();
  frame_send.data[2] = motor_2.get_output() >> 8;
  frame_send.data[3] = motor_2.get_output();
  frame_send.data[4] = 0;
  frame_send.data[5] = 0;
  frame_send.data[6] = 0;
  frame_send.data[7] = 0;
  
  mcp2515.sendMessage(&frame_send);
  
  t = (t+3)%3600;
  Serial.println(t);
  delay(10);
}
