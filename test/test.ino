#include <can.h>
#include <SPI.h>
#include <mcp2515.h>
#include "Robot_Leg.h"
#include "MechSpec.h"
#define MOTOR_SEND_ID 0x200
#define MOTOR_1_ID 0X201
#define MOTOR_2_ID 0x202

// boot up CAN bus
struct can_frame frame;
MCP2515 mcp2515(10);
// boot up testing
int t = 0;

// boot up Robot_leg IK
RobotLeg Leg(A_LENGHT,B_LENGHT,C_LENGHT,D_LENGHT,F_LENGHT,ALPHA_ANGLE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setLoopbackMode();
  frame.can_dlc = 8;
  Serial.println("------------Initialization Finished ------------");
}

void loop() {
  // put your main code here, to run repeatedly
  Serial.println("------------New Loop started ------------");
  Serial.println(t++);
  frame.can_id = t%2 ? MOTOR_1_ID : MOTOR_2_ID;
  if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
    Serial.println("Read Sucessful");
    Serial.print("Motor ");
    Serial.println( frame.can_id == MOTOR_1_ID ? "1:" : "2:" );
    Serial.print("Mechanical angle high: ");
    Serial.println(frame.data[0]);
    Serial.print("Mechanical angle low:  ");
    Serial.println(frame.data[1]);
    Serial.print("Rotational speed high: ");
    Serial.println(frame.data[2]);
    Serial.print("Rotational speed low:  ");
    Serial.println(frame.data[3]);
    Serial.print("Mechanical angle low:  ");
    Serial.println(frame.data[4]);
    Serial.print("Torque current high:   ");
    Serial.println(frame.data[5]);
    Serial.print("Troque current low:    ");
    Serial.println(frame.data[6]);
    Serial.print("NULL:                  ");
    Serial.println(frame.data[7]);
  }
  else{
    Serial.println("Reading Fail");
  }
  
  delay(1000);
}
