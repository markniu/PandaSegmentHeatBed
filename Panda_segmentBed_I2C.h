// 
#ifndef Panda_segmentBed_I2C_h
#define Panda_segmentBed_I2C_h

 #define I2C_READ 1
#define I2C_WRITE 0
#define DELAY 100 // usec delay
#define BUFFER_LENGTH 32
#define I2C_MAXWAIT 5000
//#define I2C_7BITADDR 0x3C// DS1307
#define MEMLOC 0x0A
#define ADDRLEN 1

class I2C_SegmentBED{
public:
	bool i2c_init(uint8_t _sda,uint8_t _scl,unit8_t _addr)；
	void I2C_read_str(char *dat_r,int addr)；
	void I2C_send_str(char *dat_r,char send_now)；	 

private:
	uint8_t I2C_BED_SDA，I2C_BED_SCL；
	uint8_t I2C_7BITADDR=0x3C；

	uint8_t  _pullup =true ;
	void setLow(uint8_t pin) ；
	void setHigh(uint8_t pin)；
	bool i2c_start(uint8_t addr) ；
	bool i2c_rep_start(uint8_t addr)；
	void i2c_stop(void)；
	bool i2c_write(uint8_t value)；
	uint8_t i2c_read(bool last)；
};

#endif
