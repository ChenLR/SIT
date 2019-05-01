#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"


void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_address_direction(uint8_t address, uint8_t direction);
void i2c_transmit(uint8_t byte);
uint8_t i2c_receive_ack(void);
uint8_t i2c_receive_nack(void);
void i2c_write(uint8_t address, uint8_t data);
void i2c_read(uint8_t address, uint8_t* data);
void i2c_write_bytes(uint8_t address, uint8_t* data, uint8_t num);
void i2c_read_bytes(uint8_t address, uint8_t* data, uint8_t num);

#endif
