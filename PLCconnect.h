#include "ModbusIP.h"

class PLCconnectClass
{
protected:
	byte f_reinit_config;
	byte pinCount, ledCount, motorCount;
	int* addr_in, *addr_out;
	byte* val_in, *val_out;

	void initPins();
	void writeInputReg();
	void readOutputReg();

	void writeEEPROM(int address, uint8_t mode);
	uint8_t readEEPROM(int address);
	uint8_t readPin(int pin);


public:
	ModbusIP mb;
	void init(ModbusIP mb, int *addr_in, int *addr_out);
	void run();
	void configIP(uint8_t* mac);
	void configIP(uint8_t* mac, IPAddress ip);
	void configIP(uint8_t* mac, IPAddress ip, IPAddress dns);
	void configIP(uint8_t* mac, IPAddress ip, IPAddress dns, IPAddress gateway);
	void configIP(uint8_t* mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet);

};

extern PLCconnectClass PLCconnect;

