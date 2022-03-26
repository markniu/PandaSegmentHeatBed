
#include "Panda_segmentBed_I2C.h"


//bool  i2c_write(uint8_t value);
//void  i2c_stop(void);

//#define setLow(pin) {if (_pullup)  digitalWrite(pin, LOW); pinMode(pin, OUTPUT);}
void    I2C_SegmentBED::setLow(uint8_t pin) {
      noInterrupts();
       pinMode(pin, OUTPUT);
   // if (_pullup) 
      digitalWrite(pin, LOW);
    

    interrupts();
}

//#define setHigh(pin) {if (_pullup)  pinMode(pin, INPUT_PULLUP); else pinMode(pin, INPUT);}
//#define i2c_start(addr)  {setLow(I2C_BED_SDA);delayMicroseconds(DELAY); setLow(I2C_BED_SCL);return i2c_write(addr);}
//#define i2c_stop() {setLow(I2C_BED_SDA); delayMicroseconds(DELAY); setHigh(I2C_BED_SCL);  delayMicroseconds(DELAY); setHigh(I2C_BED_SDA);delayMicroseconds(DELAY);}


void   I2C_SegmentBED::setHigh(uint8_t pin) {
 // digitalWrite(pin,1);
 // return;
     noInterrupts();
    //if (_pullup) 
      pinMode(pin, INPUT_PULLUP);
   // else
   //   pinMode(pin, INPUT);

     interrupts();
}
bool  I2C_SegmentBED::i2c_init(uint8_t _sda,uint8_t _scl,unit8_t _addr) {
 I2C_BED_SDA=_sda;
 I2C_BED_SCL =_scl;
 I2C_7BITADDR = _addr;
  digitalWrite(I2C_BED_SDA, LOW);
  digitalWrite(I2C_BED_SCL, LOW);
  setHigh(I2C_BED_SDA);
  setHigh(I2C_BED_SCL);
  if (digitalRead(I2C_BED_SDA) == LOW || digitalRead(I2C_BED_SCL) == LOW) return false;
  return true;
}

// Start transfer function: <addr> is the 8-bit I2C address (including the R/W
// bit). 
// Return: true if the slave replies with an "acknowledge", false otherwise

bool  I2C_SegmentBED::i2c_start(uint8_t addr) {
  setLow(I2C_BED_SDA);
  delayMicroseconds(DELAY);
  setLow(I2C_BED_SCL);
  return i2c_write(addr);
}

// Try to start transfer until an ACK is returned
/*  i2c_start_wait(uint8_t addr) {
  long retry = I2C_MAXWAIT;
  while (!i2c_start(addr)) {
    i2c_stop();
    if (--retry == 0) return false;
  }
  return true;
}
*/
// Repeated start function: After having claimed the bus with a start condition,
// you can address another or the same chip again without an intervening 
// stop condition.
// Return: true if the slave replies with an "acknowledge", false otherwise
bool  I2C_SegmentBED::i2c_rep_start(uint8_t addr) {
  setHigh(I2C_BED_SDA);
  setHigh(I2C_BED_SCL);
  delayMicroseconds(DELAY);
  return i2c_start(addr);
}

// Issue a stop condition, freeing the bus.

void  I2C_SegmentBED::i2c_stop(void) {
  setLow(I2C_BED_SDA);
  delayMicroseconds(DELAY);
  setHigh(I2C_BED_SCL);
  delayMicroseconds(DELAY);
  setHigh(I2C_BED_SDA);
  delayMicroseconds(DELAY);
}

// Write one byte to the slave chip that had been addressed
// by the previous start call. <value> is the byte to be sent.
// Return: true if the slave replies with an "acknowledge", false otherwise
bool  I2C_SegmentBED::i2c_write(uint8_t value) {
  for (uint8_t curr = 0X80; curr != 0; curr >>= 1) {
    if (curr & value) {setHigh(I2C_BED_SDA);} else  setLow(I2C_BED_SDA); 
    setHigh(I2C_BED_SCL);
    delayMicroseconds(DELAY);
    setLow(I2C_BED_SCL);
    delayMicroseconds(DELAY);
  }
  // get Ack or Nak
  setHigh(I2C_BED_SDA);
  setHigh(I2C_BED_SCL);
  delayMicroseconds(DELAY);
  uint8_t ack = digitalRead(I2C_BED_SDA);
  setLow(I2C_BED_SCL);
  delayMicroseconds(DELAY);  
  setLow(I2C_BED_SDA);
  return ack == 0;
}

// Read one byte. If <last> is true, we send a NAK after having received 
// the byte in order to terminate the read sequence. 
uint8_t  I2C_SegmentBED::i2c_read(bool last) {
  uint8_t b = 0;
  setHigh(I2C_BED_SDA);
  for (uint8_t i = 0; i < 8; i++) {
    b <<= 1;
    delayMicroseconds(DELAY);
    setHigh(I2C_BED_SCL);
    delayMicroseconds(DELAY);
    if (digitalRead(I2C_BED_SDA)) b |= 1;
    setLow(I2C_BED_SCL);
  }
  if (last) {setHigh(I2C_BED_SDA);} else setLow(I2C_BED_SDA);
  setHigh(I2C_BED_SCL);
  delayMicroseconds(DELAY);
  setLow(I2C_BED_SCL);
  delayMicroseconds(DELAY);  
  setLow(I2C_BED_SDA);
  return b;
}


void  I2C_SegmentBED::I2C_send_str(char *dat_r,char send_now)
{
  int i=0;
  i2c_stop();
 // static unsigned long timeout = millis() + 100;
 // if(((long)(millis() - timeout) > 0L)||(send_now))
  {
  //  timeout=millis() + 100;
    if(i2c_start((I2C_7BITADDR<<1)|I2C_WRITE))
    {
      i2c_write(0x00);
    // i2c_rep_start((I2C_7BITADDR<<1)|I2C_WRITE);
      while(*dat_r)
        i2c_write(*dat_r++);
    }
    i2c_stop();
  }
    
  
}

void I2C_SegmentBED::I2C_read_str(char *dat_r,int addr)
{
  i2c_stop(); 
  if(i2c_start((I2C_7BITADDR<<1)|I2C_WRITE))
  {
    int i =0;
    i2c_write(addr);
    i2c_rep_start((I2C_7BITADDR<<1)|I2C_READ);
    while(i<=50)  // the max length is 50 bytes
    {
      dat_r[i]= i2c_read(false);
      if(dat_r[i]==0)
        break;
      i++;
    }
    i2c_read(true);
    
  }
  i2c_stop();
}