#ifndef Comm
#define Comm

#include <can.h>
#include <mcp2515.h>

MCP2515 mcp2515(10);

struct data_read
{
  int16_t pos, spd, toq;
  char ID;
}__attribute__((packed));

bool fetchInfo(struct can_frame F, struct data_read D){
  if( mcp2515.readMessage(&F) == MCP2515::ERROR_OK){
    memcpy(&D.pos, &F.data[0], sizeof(int16_t));
    memcpy(&D.spd, &F.data[2], sizeof(int16_t));
    memcpy(&D.toq, &F.data[4], sizeof(int16_t));
    return true;
  }
  return false;
}

void logInfo(struct data_read D){
  Serial.print("Motor ");
  Serial.println(D.ID);
  Serial.print("position: ");
  Serial.println(D.pos);
  Serial.print("Speed   : ");
  Serial.println(D.spd);
  Serial.print("Torque  : ");
  Serial.println(D.toq);
}

#endif
