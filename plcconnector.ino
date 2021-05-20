// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       PLCconnector.ino
    Created:	08.05.2020 15:26:14
    Author:     DESKTOP-HOG71UM\home
*/

// Define User Types below here or use a .h file
//
#include "PLCconnect.h"

// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts

PLCconnectClass plc;

ModbusIP mb = ModbusIP();

void setup()
{
	Serial.begin(115200);
	uint8_t mac[] = { 0x3C, 0x18, 0xA0, 0x06, 0xB0, 0x60 };
	//IPAddress  ip(10, 0, 170, 119);
	//IPAddress  dns(10, 0, 170, 1);
	//IPAddress  gateway(10, 0, 170, 1);
	IPAddress  ip(10, 0, 170, 63);
	IPAddress  dns(10, 0, 170, 1);
	IPAddress  gateway(10, 0, 170, 1);
	IPAddress  subnet(255, 255, 255, 0);
	mb.config(mac, ip, dns, gateway, subnet);
	byte error, address; //variable for error and I2C address
	int nDevices;
	nDevices = 0;
	int *addr_in, *addr_out;
	addr_in = (int*)malloc(9 * sizeof(int));
	addr_out = (int*)malloc(3 * sizeof(int));
	memset(addr_in, 0, 9);
	memset(addr_out, 0, 3);
	Wire.begin();
	for (address = 0x20; address < 0x28; address++)
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (error == 0)
		{
			addr_in[address - 0x20] = 1;
			Serial.print("I2C device found at address 0x"); Serial.print(address, HEX); Serial.println("  !");
		}					
	}	
	for (address = 0x10; address < 0x12; address++)
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (error == 0)
		{
			addr_out[address - 0x10] = 1;
			Serial.print("I2C device found at address 0x");Serial.print(address, HEX);Serial.println("  !");
		}
	}
	plc.init(mb, addr_in, addr_out);

}

// Add the main program code into the continuous loop() function
void loop()
{					
	plc.run();	
}
