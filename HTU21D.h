/**
 * @author Alan Lai & Nelson Diaz modified by Johnny Garcia
 * The Georgia Institute of Technology 
 * ECE 4180 Embeded Systems
 * Professor Hamblen
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
 *
 * https://www.sparkfun.com/products/12064
 */
 
 #ifndef HTU21D_H
 #define HTU21D_H
 
 // **Includes.
 #include "mbed.h"
 
 // **Address & Commands.
 #define HTU21D_I2C_ADDRESS 						0X40
 #define HTU21D_I2C_ADDRESS_PLUS_WRITE 	((HTU21D_I2C_ADDRESS <<1) | (0X00)) //0x80
 #define HTU21D_I2C_ADDRESS_PLUS_READ		((HTU21D_I2C_ADDRESS <<1) | (0X01)) //0x81
 
 #define TRIGGER_TEMP_MEASURE_HOLD_MASTER  			0xE3
 #define TRIGGER_HUMD_MEASURE_HOLD_MASTER  			0xE5
 #define TRIGGER_TEMP_MEASURE__NO_HOLD_MASTER  	0xF3
 #define TRIGGER_HUMD_MEASURE_NO_HOLD_MASTER  	0xF5
 
 #define WRITE_USER_REG	0XE6
 #define READ_USER_REG 	0XE7
 #define SOFT_RESET			0XFE
 
 //**Parameters
 #define I2C_FREQUENCY 400000 //400kHz, as specified by the datasheet.
 
 // ****Honeywell HTU21D digital humidity and temperature sensor.****
 class HTU21D{
	 
	 public:
		 /**
     * Constructor.
     *
     * @param sda mbed pin to use for SDA line of I2C interface.
     * @param scl mbed pin to use for SCL line of I2C interface.
     */
		HTU21D(PinName sda, PinName scl); 
		
		//Read the temperature, input void,outputs a float in Celcius
		float readTempC(void);
	 
		//Read the temperature, input void,outputs a float in Fahrenheit
		float readTempF(void);
	 
		//Read the temperature, input void,outputs a float in Kelvin
		float readTempK(void);
		
		//Read the humidity, input void, outputs and float.
    float read_humid(void);
	 
		//Soft reset
	  void reset(void);
		
		//Change user register
		void changeResolution(char);
	 
	  private:
		I2C* _i2c;
	 
		//Read user register
		char readUsrReg(void);
	 
		/**
     * Write to EEPROM or RAM on the device.
     *
     * @param EepromOrRam 0x80 -> Writing to EEPROM
     * @param address Address to write to.
     * @param data Data to write.
     */
		void write (int EepromOrRam, int address, int data);
		
		/**
     * Read EEPROM or RAM on the device.
     *
     * @param EepromOrRam 0x81 -> Reading from EEPROM
     * @param address Address to read from.
     * @return The contents of the memory address.
     */
    int read(int EepromOrRam, int address);

 };
 #endif // HTU21D_H
