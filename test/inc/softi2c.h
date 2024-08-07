#ifndef __SOFTI2C_H__
#define __SOFTI2C_H__
#include<linux/gpio.h>
#include<linux/spinlock.h>
#include"I2c_Pins.h"
#define I2C_WRITE_ADDR(addr)    ((addr<<1)|0)
#define I2C_READ_ADDR(addr)    ((addr<<1)|1)
extern void get_i2c_lock(I2c_Pins *pins);
extern void free_i2c_lock(I2c_Pins *pins);
#define ACK 1
#define NACK 0
extern void I2c_Start(I2c_Pins* pins);
extern void I2c_Stop(I2c_Pins* pins);
extern void I2c_WrByte(I2c_Pins* pins,unsigned char dat);
extern unsigned char I2c_RdByte(I2c_Pins* pins,int ack);
#endif