#include <SoftwareSerial.h>

//off == 0x01

SoftwareSerial hulk_Sholder(2,3);  // TX ->2,  RX ->3

int SW1 = 4;
int SW2 = 5;
int SW3 = 6;
int SW1_data;
int SW2_data;
int SW3_data;

void setup() {
  // put your setup code here, to run once:
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(SW3,INPUT);
  
  Serial.begin(9600);   // 손목
  hulk_Sholder.begin(9600);  // 머리, 어깨
}

void loop() {
  // put your main code here, to run repeatedly:
  SW1_data = digitalRead(SW1);  // 머리
  SW2_data = digitalRead(SW2);  // 어깨
  SW3_data = digitalRead(SW3);  // 손목

  if(SW3_data == 0){
    Serial.write(0x00);
  }
  else if(SW3_data ==1){
    Serial.write(0x01);
  }
  if(SW1_data == 0 && SW2_data == 0)
  {
   //hulk_Sholder.write(0x00);
   hulk_Sholder.write(0x00);
  }
  else if(SW1_data == 1 && SW2_data == 0)
  {
  // hulk_Sholder.write(0x01);
  hulk_Sholder.write(0x01);
  }
  else if(SW1_data == 0 && SW2_data == 1)
  {
   hulk_Sholder.write(0x80);
  }
  else if(SW1_data == 1 && SW2_data == 1)
  {
   hulk_Sholder.write(0x81);
  }
/*
  Serial.print("SW1 : ");
  Serial.print(SW1_data);
  Serial.print("   SW2 : ");
  Serial.print(SW2_data);
  Serial.print("   SW3 : ");
  Serial.println(SW3_data);*/
}
