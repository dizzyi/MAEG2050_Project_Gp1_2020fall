#include <can.h>
#include <SPI.h>
#include <mcp2515.h>
#include "Robot_Leg.h"
#include "MechSpec.h"

// boot up CAN bus
struct can_frame canMsg;
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
  Serial.println("------------Initialization Finished ------------");
}

void loop() {
  // put your main code here, to run repeatedly
  Serial.println("------------New Loop started ------------");
  Serial.println(++t);

  
}
