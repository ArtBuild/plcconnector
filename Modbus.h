/*
	Modbus.h - Header for Modbus Base Library
	Copyright (C) 2014 Andrй Sarmento Barbosa
*/
#include "Arduino.h"
#include <EEPROM.h>
#include "GloabalConst.h"
//#include <FastLED.h>



class Modbus {
private:
	InputRegister* _input_head;
	InputRegister* _input_last;
	OutputRegister* _output_head;
	OutputRegister* _output_last;

	void read_output_registers(byte* frame);
	void write_inputs_registers(byte* frame, u_short pinsCount);
	void set_pinMode_register(byte* frame, u_short pinsCount);
	void exceptionResponse(byte fcode, byte excode);

	InputRegister* search_input_register(u_short addr);
	OutputRegister* search_output_register(u_short addr);

protected:
	byte *_frame;
	u_short  _len;
	byte  _reply;
	u_short _outputRegCnt;
	u_short _pinsCnt;
	void receive_PDU(byte* frame);

public:
	PinMode* pinMode;
	u_short* pinLeds;
	u_short* pinMotor;
	u_short pinsCount;
	bool fReinitPins;

	Modbus();

	void free_all_regs();
	void free_all_input_regs();
	void free_all_output_regs();

	void add_input_reg(u_short pin, byte value);
	void add_output_reg(u_short pin, byte value);

	bool input_reg(u_short pin, byte value);
	bool output_reg(u_short pin, byte value);

	byte input_reg(u_short pin);
	byte output_reg(u_short pin);
};
