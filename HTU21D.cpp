/**
 * @author Alan Lai & Nelson Diaz revision by Johnny Garcia
 * Last revision 02/10/2016
 * 
 * @section LICENSE
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 43):
 * <alanhlai91@gmail.com & jpgmar@outlook.com> wrote this file. As long as you 
 * retain this notice you can do whatever you want with this stuff. If we meet 
 * some day, and you think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 *
 * @section DESCRIPTION
 *
 * Honeywell HTU21D Humidity and Temperature sensor.
 *
 * Datasheet, specs, and information:
 * https://www.sparkfun.com/products/12064
 */
 
// **Includes
#include "HTU21D.h"
#include "mbed.h"
//Serial pc(USBTX,USBRX);

HTU21D::HTU21D(PinName sda, PinName scl){
	_i2c = new I2C(sda, scl);
	_i2c->frequency(I2C_FREQUENCY);
}

float HTU21D::readTempC(void){
	char cmd = TRIGGER_TEMP_MEASURE_HOLD_MASTER; //tx: transfer 1 byte 
	char rx[2]; //rx: receive 2 bytes.
	
	_i2c->write(HTU21D_I2C_ADDRESS_PLUS_WRITE, &cmd, 1);
	wait_ms(1);
	//pc.printf("sent: %x\n\r", cmd);
	
	//Reads triegered measure
	_i2c->read(HTU21D_I2C_ADDRESS_PLUS_READ, rx, 2);
	wait_ms(1);
	//pc.printf("received: %x : %d", rx[0], rx[1]);

	// Algorithm from datasheet to compute temperature.
   unsigned int rawTemp = ((unsigned int) rx[0] << 8) | (unsigned int) rx[1];
	 rawTemp &= 0xFFFC; //Zero out the status bits but keep them in place
	
	float realTempC = rawTemp / (float)65536;	//2^16 = 65536
	realTempC = (175.85 * realTempC) - 46.85;		//From page 15
	//pc.printf("Temperature: %3.2f\r\n", realTemp);
	
	return realTempC;
}

float HTU21D::readTempF(void){
    float realTempF = readTempC();
    realTempF = (realTempF * 1.8) + 32;   
    return realTempF;
}

float HTU21D::readTempK(void){
    float realTempK = readTempC();
    realTempK += 274;
    return realTempK;
}

float HTU21D::read_humid(void){
	char cmd = TRIGGER_HUMD_MEASURE_HOLD_MASTER; //tx: transfer 1 byte 
	char rx[2]; //rx: receive 2 bytes.
	
	_i2c->write(HTU21D_I2C_ADDRESS_PLUS_WRITE, &cmd, 1);
	wait_ms(1);
	//pc.printf("sent: %x\n\r", cmd);
	
	//Read triegered measure
	_i2c->read(HTU21D_I2C_ADDRESS_PLUS_READ, rx, 2);
	wait_ms(1);
	//pc.printf("received: %x : %d", rx[0], rx[1]);
	
	//Algorithm from datasheet.
  unsigned int rawHumd = ((unsigned int) rx[0] << 8) | (unsigned int) rx[1];
	rawHumd &= 0xFFFC; //Zero out the status bits but keep them in place
	
	float realHumd = rawHumd / (float)65536;	//2^16 = 65536
	realHumd = (125* realHumd) - 6;		//From page 15
	//pc.printf("Humidity: %3.2f\r\n", realHumd);
	return realHumd;
}

void HTU21D::reset(void){
	char cmd = SOFT_RESET;
	
	_i2c->write(HTU21D_I2C_ADDRESS_PLUS_WRITE, &cmd, 1);
	wait_ms(1);
}

void HTU21D::changeResolution(char newRes){
	char UserReg = readUsrReg();	//Read the current register state
	
	newRes &=~ 0x8E; //mask &= 0b10000001 Turns off all bits but resolution bits
	UserReg &= 0x8E; //mask &= 0b01111110 Turns off resolution bits
	UserReg |= newRes; //Mask in the requested resolution bits
	
	char cmd[2] = {WRITE_USER_REG, UserReg};
	
	_i2c->write(HTU21D_I2C_ADDRESS_PLUS_WRITE, cmd, 2);
	wait_ms(1);	
	//pc.printf("sent command: %x and and user register: %x\n\r", cmd[0], cmd[1]);
}

char HTU21D::readUsrReg(void){
	char cmd = READ_USER_REG;
	char rx;
	
	_i2c->write(HTU21D_I2C_ADDRESS_PLUS_WRITE, &cmd, 1);
	wait_ms(1);
	//pc.printf("sent: %x\n\r", cmd);
	
	//Read user register
	_i2c->read(HTU21D_I2C_ADDRESS_PLUS_READ, &rx, 1);
	wait_ms(1);
	//pc.printf("received: %x : %d", rx[0], rx[1]);
	
	return rx;
}
