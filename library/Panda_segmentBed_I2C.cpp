#include "Arduino.h"
#include "Panda_segmentBed_I2C.h"


//bool  i2c_write(unsigned char value);
//void  i2c_stop(void);

//#define setLow(pin) {if (_pullup)  digitalWrite(pin, LOW); pinMode(pin, OUTPUT);}
void    I2C_SegmentBED::setLow(unsigned char pin) {
      noInterrupts();
       pinMode(pin, OUTPUT);
   // if (_pullup) 
      digitalWrite(pin, LOW);
    

    interrupts();
}

//#define setHigh(pin) {if (_pullup)  pinMode(pin, INPUT_PULLUP); else pinMode(pin, INPUT);}
//#define i2c_start(addr)  {setLow(I2C_BED_SDA);delayMicroseconds(DELAY); setLow(I2C_BED_SCL);return i2c_write(addr);}
//#define i2c_stop() {setLow(I2C_BED_SDA); delayMicroseconds(DELAY); setHigh(I2C_BED_SCL);  delayMicroseconds(DELAY); setHigh(I2C_BED_SDA);delayMicroseconds(DELAY);}


void   I2C_SegmentBED::setHigh(unsigned char pin) {
 // digitalWrite(pin,1);
 // return;
     noInterrupts();
    //if (_pullup) 
      pinMode(pin, INPUT_PULLUP);
   // else
   //   pinMode(pin, INPUT);

     interrupts();
}
bool  I2C_SegmentBED::i2c_init(unsigned char _sda,unsigned char _scl,unsigned char _addr) {
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

bool  I2C_SegmentBED::i2c_start(unsigned char addr) {
  setLow(I2C_BED_SDA);
  delayMicroseconds(DELAY);
  setLow(I2C_BED_SCL);
  return i2c_write(addr);
}

// Try to start transfer until an ACK is returned
/*  i2c_start_wait(unsigned char addr) {
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
bool  I2C_SegmentBED::i2c_rep_start(unsigned char addr) {
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
bool  I2C_SegmentBED::i2c_write(unsigned char value) {
  for (unsigned char curr = 0X80; curr != 0; curr >>= 1) {
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
  unsigned char ack = digitalRead(I2C_BED_SDA);
  setLow(I2C_BED_SCL);
  delayMicroseconds(DELAY);  
  setLow(I2C_BED_SDA);
  return ack == 0;
}

// Read one byte. If <last> is true, we send a NAK after having received 
// the byte in order to terminate the read sequence. 
unsigned char  I2C_SegmentBED::i2c_read(bool last) {
  unsigned char b = 0;
  setHigh(I2C_BED_SDA);
  for (unsigned char i = 0; i < 8; i++) {
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
////////////////////////////
 
unsigned short I2C_SegmentBED::BD_Add_OddEven(unsigned short byte)
{
	unsigned char i;
	unsigned char n; 
	unsigned short r; 
	n =0;
  for(i=0;i<10;i++)
	{
	  if(((byte >>i)&0x01) == 0x01)
		{
		   n++;
		}	
	}
	if((n&0x01) == 0x01)
	{
		r = byte | 0x400; 
	}
	else
	{
	  r = byte | 0x00;  
	}
	return r;
}

#define BYTE_CHECK_OK     0x01
#define BYTE_CHECK_ERR    0x00
/******************************************************************************************

********************************************************************************************/
unsigned short I2C_SegmentBED::BD_Check_OddEven(unsigned short byte)
{
	unsigned char i;
	unsigned char n;  
	unsigned char r;  
	n =0;
   for(i=0;i<10;i++) 
	{
	  if(((byte >>i)&0x01) == 0x01)
		{
		   n++;  
		}	
	}
	if((byte>>10) == (n&0x01)) 
	{
		r = BYTE_CHECK_OK;  
	}
	else
	{
	  r = BYTE_CHECK_ERR;  
	}
	return r;
}	

void   I2C_SegmentBED::BD_setLow(unsigned char pin) {
      noInterrupts();
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
      interrupts();
}

void    I2C_SegmentBED::BD_setHigh(unsigned char pin) {
     noInterrupts();
     pinMode(pin, INPUT_PULLUP);
     interrupts();
}

void  I2C_SegmentBED::BD_set_force_High(unsigned char pin) {
      noInterrupts();
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
     interrupts();
}


bool I2C_SegmentBED::BD_I2C_start(void)
{
  BD_setHigh(I2C_BED_SCL);
  BD_setHigh(I2C_BED_SDA);
  delayMicroseconds(DELAY);
  BD_setLow(I2C_BED_SDA);
  delayMicroseconds(DELAY);
  BD_setLow(I2C_BED_SCL);
  delayMicroseconds(DELAY*2);

}
void  I2C_SegmentBED::BD_i2c_stop(void) {
  delayMicroseconds(DELAY*2);
  BD_setLow(I2C_BED_SDA);
  delayMicroseconds(DELAY);
  BD_setHigh(I2C_BED_SCL);
  delayMicroseconds(DELAY);
  BD_setHigh(I2C_BED_SDA);
  delayMicroseconds(DELAY);
}

unsigned short I2C_SegmentBED::BD_i2c_read(void)
{
   
  BD_I2C_start();
  //// read
  BD_setHigh(I2C_BED_SDA);
  BD_setHigh(I2C_BED_SCL);
  delayMicroseconds(DELAY*2);
  BD_setLow(I2C_BED_SCL);
  ///
  delayMicroseconds(DELAY);
  unsigned short b = 0;
  BD_setHigh(I2C_BED_SDA);
  for (unsigned char i = 0; i <= 10; i++) {
    b <<= 1;
    delayMicroseconds(DELAY);
    BD_setHigh(I2C_BED_SCL);
    delayMicroseconds(DELAY);
    if (digitalRead(I2C_BED_SDA)) b |= 1;
    BD_setLow(I2C_BED_SCL);
  }
  BD_i2c_stop();
  return b;

}

void I2C_SegmentBED::BD_i2c_write(unsigned int addr)
{
 
  BD_I2C_start();
  //// write
  BD_setLow(I2C_BED_SDA);
  BD_set_force_High(I2C_BED_SCL);
  delayMicroseconds(DELAY);
  BD_setLow(I2C_BED_SCL);
  addr=BD_Add_OddEven(addr);
  ///write address
  delayMicroseconds(DELAY);
  for (int i=10; i >=0; i--) 
  {
    if ((addr>>i)&0x01) {BD_set_force_High(I2C_BED_SDA);} else  BD_setLow(I2C_BED_SDA); 
    //if (addr &curr) {set_force_High(I2C_BED_SDA);} else  setLow(I2C_BED_SDA); 
    BD_set_force_High(I2C_BED_SCL);
    delayMicroseconds(DELAY);
    BD_setLow(I2C_BED_SCL);
    delayMicroseconds(DELAY);
  }
  ////////////
  BD_i2c_stop();
  

}