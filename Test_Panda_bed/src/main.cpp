#include <Arduino.h>
#include <Panda_segmentBed_I2C.h>
I2C_SegmentBED I2CsegmentBED;
#define  PANDA_BED_SDA  15  
#define  PANDA_BED_SCL  13  
char tmp_1[50];

void setup() {
  
  delay(1000);
  I2CsegmentBED.i2c_init(PANDA_BED_SDA,PANDA_BED_SCL,0x3C);
  Serial.begin(115200);
  
  
}

void loop() {
  delay(1000);
  ////////// read all the segments bed temperature       
  I2CsegmentBED.I2C_read_str(tmp_1,0);
  Serial.printf(tmp_1);
  
  /////////// set the modular bed which the line [(100mm,100mm)~(110mm,110mm)] position to temperature 60 celsius degree
  delay(1000);
  sprintf(tmp_1,"M140 S60\n");
  I2CsegmentBED.I2C_send_str(tmp_1,0);
  
  sprintf(tmp_1,"G1 X100 Y100\n");
  I2CsegmentBED.I2C_send_str(tmp_1,0);
  delay(100);
  sprintf(tmp_1,"G1 X110 Y110\n");
  I2CsegmentBED.I2C_send_str(tmp_1,0);

 
}