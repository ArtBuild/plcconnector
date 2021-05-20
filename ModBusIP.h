/*
	ModbusIP.h - Header for Modbus IP Library
	Copyright (C) 2015 Andr? Sarmento Barbosa
*/
//#include <Arduino.h>
#include "Modbus.h"
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>

#ifndef MODBUSIP_H
#define MODBUSIP_H


class ModbusIP : public Modbus {
private:
	EthernetServer _server;
	byte _MBAP[7];

public:

	ModbusIP();
	void config(uint8_t *mac);
	void config(uint8_t *mac, IPAddress ip);
	void config(uint8_t *mac, IPAddress ip, IPAddress dns);
	void config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway);
	void config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet);
	void task();
	void wire_task();

};

#endif //MODBUSIP_H

