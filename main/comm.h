#ifndef Comm
#define Comm
#include <can.h>
#include <mcp2515.h>

MCP2515 mcp2515(10);



struct Motor_Controller
{
  int16_t pos, spd, toq;
  int8_t  temp, nu, ID;
  
}__attribute__((packed));

struct data_send
{
  int16_t m1, m2, m3, m4;
}__attribute__((packed));

void PrintHex8(uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
       Serial.print("0x");
       for (int i=0; i<length; i++) {
         if (data[i]<0x10) {Serial.print("0");}
         Serial.print(data[i],HEX);
         Serial.print(" ");
       }
}


void PrintHex16(uint16_t *data, uint8_t length) // prints 16-bit data in hex with leading zeroes
{
       Serial.print("0x");
       for (int i=0; i<length; i++)
       {
         uint8_t MSB=byte(data[i]>>8);
         uint8_t LSB=byte(data[i]);
         
         if (MSB<0x10) {Serial.print("0");} Serial.print(MSB,HEX); Serial.print(" ");
         if (LSB<0x10) {Serial.print("0");} Serial.print(LSB,HEX); Serial.print(" ");
       }
}

void saturate(struct data_send D){
  D.m1 = ( D.m1 >=  ALL_SIG ) * ( ALL_SIG - D.m1) 
       + ( D.m1 <= -ALL_SIG ) * (-ALL_SIG - D.m1) 
                                          + D.m1;
  
  D.m2 = ( D.m2 >=  ALL_SIG ) * ( ALL_SIG - D.m2) 
       + ( D.m2 <= -ALL_SIG ) * (-ALL_SIG - D.m2) 
                                          + D.m2;
  
  D.m3 = ( D.m3 >=  ALL_SIG ) * ( ALL_SIG - D.m3) 
       + ( D.m3 <= -ALL_SIG ) * (-ALL_SIG - D.m3) 
                                          + D.m3;
  
  D.m4 = ( D.m4 >=  ALL_SIG ) * ( ALL_SIG - D.m4) 
       + ( D.m4 <= -ALL_SIG ) * (-ALL_SIG - D.m4) 
                                          + D.m4;
}

bool getInfo(struct can_frame * F, struct data_read * D){
  if( mcp2515.readMessage(F) == MCP2515::ERROR_OK){
    Serial.println("Get:");
    //PrintHex8(F.data, 8);
    //memcpy(D, &(F->data), sizeof(struct data_read));
    D->pos = F->data[0] << 8 | F->data[1];
    D->spd = F->data[2] << 8 | F->data[3];
    D->toq = F->data[4] << 8 | F->data[5];
    D->temp= F->data[6];
    D->nu  = F->data[7];
    /*
    Serial.println("");
    Serial.print("0x");
    Serial.print(D.pos,HEX);
    Serial.print(" ");
    Serial.print(D.spd,HEX);
    Serial.print(" ");
    Serial.print(D.toq,HEX);
    Serial.print(" ");
    Serial.print(D.temp,HEX);
    Serial.print(" ");
    Serial.println(D.nu);
    */
    return true;
  }
  return false;
}

bool postInfo(struct can_frame * F, struct data_send * D){
  //saturate(*D);
  /*Serial.println(D->m1,HEX);
  Serial.println(D->m2,HEX);
  Serial.println(D->m3,HEX);
  Serial.println(D->m4,HEX);
  Serial.println("**********");*/
  memcpy(&(F->data),D, sizeof(int16_t) * 4);
  
 // for(int i = 0; i < 8 ; i++) Serial.println(F->data[i],HEX);
  
  F->data[0] = D->m1 >> 8;
  F->data[1] = D->m1;
  F->data[2] = D->m2 >> 8;
  F->data[3] = D->m2;
  
//  for(int i = 0; i < 8 ; i++) Serial.println(F->data[i],HEX);
  
  //mcp2515.sendMessage(F);
  return true;
}
/*
void logInfo(struct data_read * D){
  Serial.print("Motor ");
  Serial.println(D->ID);
  Serial.print("position: ");
  Serial.print(D->pos);
  Serial.print(" ( angle: ");
  Serial.println(D->pos * 360. / 8191.);
  Serial.print("Speed   : ");
  Serial.println(D->spd);
  Serial.print("Torque  : ");
  Serial.println(D->toq);
}
*/
#endif
