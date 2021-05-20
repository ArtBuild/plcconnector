#include "PLCconnect.h"


void PLCconnectClass::writeEEPROM(int address, uint8_t mode)
{
	EEPROM.update(address, mode);
}

uint8_t PLCconnectClass::readEEPROM(int address)
{
	return EEPROM.read(address);
}

void PLCconnectClass::configIP(uint8_t* mac) {
	mb.config(mac);
}

void PLCconnectClass::configIP(uint8_t* mac, IPAddress ip) {
	mb.config(mac, ip);
}

void PLCconnectClass::configIP(uint8_t* mac, IPAddress ip, IPAddress dns) {
	mb.config(mac, ip, dns);
}

void PLCconnectClass::configIP(uint8_t* mac, IPAddress ip, IPAddress dns, IPAddress gateway) {
	mb.config(mac, ip, dns, gateway);
}

void PLCconnectClass::configIP(uint8_t* mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet) {
	mb.config(mac, ip, dns, gateway, subnet);
}

void PLCconnectClass::initPins()
{
	u_short pin, addr;
	byte mode, p_;
	Serial.print("initPins:\n");
	//for PLC INPUT
	for (int pin = PCF8574_START_PIN_I2C; pin < PCF8574_END_PIN_I2C; pin++) {
		addr = (pin - PCF8574_START_PIN_I2C) / 7;
		if (addr_in[addr] == 1) {
			Serial.print("Pin plc input ");Serial.print(pin);Serial.println(": OUTPUT");
			p_ = (pin - PCF8574_START_PIN_I2C) % 7;
			this->mb.add_input_reg(pin, 0);
		}
	}
	//for PLC OUTPUT
	for (int pin = DIGITAL_START_PIN; pin < DIGITAL_END_PIN; pin++) {								
		if (pin < 50) {
			Serial.print("Pin plc output ");Serial.print(pin);Serial.println(": INPUT_PULLUP");
			pinMode(pin, INPUT_PULLUP);
			this->mb.add_output_reg(pin, 0);
		}
		else
		{
			addr = ((pin - DIGITAL_START_PIN) / 28)-1;
			if (addr_out[addr] == 1) {
				Serial.print("Pin plc output ");Serial.print(pin);Serial.print(": INPUT_PULLUP (addr:");Serial.print(addr+0x10);Serial.println(")");
				this->mb.add_output_reg(pin, 0);
				//TODO
			}
		}
	}
		
	Serial.println(" finished initPins:\n");
}

void PLCconnectClass::writeInputReg()
{
	//Serial.println(" writeInputReg");
	u_short pin, addr;
	byte mode, p_;
	for (int pin = PCF8574_START_PIN_I2C; pin < PCF8574_END_PIN_I2C; pin++) {
		addr = (pin - PCF8574_START_PIN_I2C) / 7;
		if (addr_in[addr] > 0) {
			p_ = (pin - PCF8574_START_PIN_I2C) % 7;
			if (this->mb.input_reg(pin) == 1)
				val_in[addr] |= bit(p_);
			else
				val_in[addr] &= ~bit(p_);
			Wire.beginTransmission((int)addr + PCF8574_I2C_START_ADDRESS);
			Wire.write(val_in[addr]);
			Wire.endTransmission();
			//Serial.print("Pin Input "); Serial.print(pin); Serial.print(" ("); Serial.print(p_); Serial.print(")");  Serial.print(" write "); Serial.println(val_in[addr]);
		}
	}

}

uint8_t PLCconnectClass::readPin(int pin)
{
	byte val1 = digitalRead(pin);
	delay(1);
	byte val2 = digitalRead(pin);
	if (val1 == val2)
	return val1;
	delay(1);
	byte val3 = digitalRead(pin);
	if (val1 == val3)
	return val1;
	else
	return val2;
}

void PLCconnectClass::readOutputReg() {
	//Serial.println(" readOutputReg");
	u_short pin, addr;
	byte mode, val;
	for (int pin = DIGITAL_START_PIN; pin < DIGITAL_END_PIN; pin++) {
		if (pin < 50) {
			val = (readPin(pin) + 1) % 2;
			//if (val == 1) {
			//	Serial.print("Pin "); Serial.print(pin); Serial.print(" read "); Serial.println(val);
			//}
			this->mb.output_reg(pin, val);
		}
		else
		{
			addr = ((pin - DIGITAL_START_PIN) / 28) - 1;
			if (addr_out[addr] == 1) {
				//TODO get val
				this->mb.output_reg(pin, val);
				//Serial.print("Pin "); Serial.print(pin); Serial.print(" read "); Serial.println(val);
			}
		}
								
	}
}

void PLCconnectClass::run()
{
	this->readOutputReg();
	this->mb.task();
	this->writeInputReg();
}

void PLCconnectClass::init(ModbusIP mb, int* _addr_in, int* _addr_out)
{
	addr_in = _addr_in;
	addr_out = _addr_out;
	//Wire.begin();
	val_in = (byte*)malloc(9 * sizeof(byte));
	val_out = (byte*)malloc(3 * sizeof(byte));
	memset(val_in, 0xff, 9);
	memset(val_out, 0xff, 3);
	for (int i = 0; i < 1024; i++) {
					writeEEPROM(i, 0xff);
	}
	this->mb = mb;	
	initPins();
}


PLCconnectClass PLCconnect;