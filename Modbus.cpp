/*
	Modbus.cpp - Source for Modbus Base Library
	Copyright (C) 2014 André Sarmento Barbosa
*/
#include "Modbus.h"

Modbus::Modbus() {
	_input_head = 0;
	_input_last = 0;
	_output_head = 0;
	_output_last = 0;
	_outputRegCnt = 0;
	this->fReinitPins = false;
	this->pinMode = (PinMode*)malloc(sizeof(PinMode)*(MAX_PIN_CNT + 1));
}

InputRegister* Modbus::search_input_register(u_short address) {
	//Serial.println("\nsearchRegister: ");
	//Serial.print(address);    
	InputRegister* reg = _input_head;
	if (reg == 0) return(0);
	do {
		//Serial.print(reg->address);
		//Serial.print(" ");
		if (reg->address == address) {
			//Serial.print(" find;");
			return(reg);
		}
		reg = reg->next;
	} while (reg);
	return(0);
}

OutputRegister* Modbus::search_output_register(u_short address) {
	//Serial.println("\nsearchRegister: ");
	//Serial.print(address);    
	OutputRegister* reg = _output_head;
	if (reg == 0) return(0);
	do {
		//Serial.print(reg->address);
		//Serial.print(" ");
		if (reg->address == address) {
			//Serial.print(" find;");
			return(reg);
		}
		reg = reg->next;
	} while (reg);
	return(0);
}

void Modbus::add_input_reg(u_short pin, byte value)
{
	Serial.println("add_input_reg");
	InputRegister* newreg;
	newreg = (InputRegister*)malloc(sizeof(InputRegister));
	newreg->address = pin;
	newreg->value = value;
	if (_input_head == 0) {
		_input_head = newreg;
		_input_last = _input_head;
	}
	else {
		//Assign the last register's next pointer to newreg.
		_input_last->next = newreg;
		//then make temp the last register in the list.
		_input_last = newreg;
		_input_last->next = 0;
	}
}


void Modbus::add_output_reg(u_short pin, byte value)
{
	Serial.println("add_output_reg");
	_outputRegCnt++;
	OutputRegister* newreg;
	newreg = (OutputRegister*)malloc(sizeof(OutputRegister));
	newreg->address = pin;
	newreg->value = value;
	if (_output_head == 0) {
		_output_head = newreg;
		_output_last = _output_head;
	}
	else {
		//Assign the last register's next pointer to newreg.
		_output_last->next = newreg;
		//then make temp the last register in the list.
		_output_last = newreg;
		_output_last->next = 0;
	}
}

bool Modbus::input_reg(u_short pin, byte value) {
	InputRegister* reg;
	reg = this->search_input_register(pin);
	//if found then assign the register value to the new value.
	if (reg) {
		if (reg->value != value) {
			reg->value = value;
		}
		return true;
	}
	else
		return false;
}

bool Modbus::output_reg(u_short pin, byte value) {
	OutputRegister* reg;
	reg = this->search_output_register(pin);
	//if found then assign the register value to the new value.
	if (reg) {
		if (reg->value != value) {
			//Serial.print("Output "); Serial.print(pin); Serial.print(": "); Serial.println(value);
			reg->value = value;
		}
		return true;
	}
	else
		return false;
}

byte Modbus::input_reg(u_short pin) {
	InputRegister* reg;
	reg = this->search_input_register(pin);
	if (reg)
		return(reg->value);
	else
		return(0);
}

byte Modbus::output_reg(u_short pin) {
	OutputRegister* reg;
	reg = this->search_output_register(pin);
	if (reg)
		return(reg->value);
	else
		return(0);
}

void Modbus::receive_PDU(byte* frame) {
	byte fcode = frame[0];
	u_short pinsCount_ = (u_short)frame[1] << 8 | (u_short)frame[2];
	//Serial.print(fcode);
	switch (fcode) {
	case MB_FC_WRITE_INPUT_REG:
		this->write_inputs_registers(frame, pinsCount_);
		break;

	case MB_FC_READ_OUTPUT_REG:
		this->read_output_registers(frame);
		break;

	case MB_FC_SET_MODE_PINS:
		this->set_pinMode_register(frame, pinsCount_);
		break;
	default:
		this->exceptionResponse(fcode, MB_EX_ILLEGAL_FUNCTION);
	}
}

void Modbus::exceptionResponse(byte fcode, byte excode) {
	//Clean frame buffer
	free(_frame);
	_len = 2;
	_frame = (byte*)malloc(_len);
	_frame[0] = fcode + 0x80;
	_frame[1] = excode;

	_reply = MB_REPLY_NORMAL;
}

void Modbus::write_inputs_registers(byte* frame, u_short pinsCount_) {
				u_short pin;
				u_short offset = 3;
				u_short value;
				byte metod, red, green, blue;
				//Serial.println("write_inputs_registers");
				for (int i = 0; i < pinsCount_; i++) {
								pin = frame[offset] << 8 | frame[offset + 1];
								offset+=2;
								input_reg(pin, (frame[offset + 3] + 1) % 2);
								offset += 4;
				}
				read_output_registers(frame);
}

void Modbus::read_output_registers(byte* frame) {
				u_short offset = 3;
				free(_frame);
				_len = 3 + this->_outputRegCnt * 3;
				_frame = (byte*)malloc(_len);
				if (!_frame) {
								this->exceptionResponse(MB_FC_WRITE_REGS, MB_EX_SLAVE_FAILURE);
								return;
				}
				_frame[0] = MB_FC_WRITE_INPUT_REG;
				_frame[1] = this->_outputRegCnt >> 8;
				_frame[2] = this->_outputRegCnt & 0x00FF;

				OutputRegister* reg = _output_head;
				//Serial.print("_outputRegCnt: ");
				//Serial.println(_outputRegCnt);
				for (int i = 0; i < this->_outputRegCnt; i++) {
								//Serial.print(reg->address);
								//Serial.print(" ");
								//Serial.print(reg->value);
								//Serial.print (" ");
								_frame[offset] = reg->address >> 8;
								_frame[offset + 1] = reg->address & 0x00FF;
								_frame[offset + 2] = reg->value;
								offset += 3;
								reg = reg->next;
				}
				//Serial.println(" ");
				_reply = MB_REPLY_NORMAL;
}

void Modbus::set_pinMode_register(byte* frame, u_short pinsCount_) {
				free(_frame);
				_len = 3;
				_frame = (byte*)malloc(_len);
				if (!_frame) {
				this->exceptionResponse(MB_FC_WRITE_REGS, MB_EX_SLAVE_FAILURE);
				return;
				}
				_frame[0] = MB_FC_SET_MODE_PINS;
				_frame[1] = 0;
				_frame[2] = 0;
				_reply = MB_REPLY_NORMAL;
}


void Modbus::free_all_regs() {
	this->free_all_input_regs();
	this->free_all_output_regs();
}

void Modbus::free_all_input_regs() {
	Serial.println("free_all_input_regs");
	InputRegister* reg = _input_head;
	InputRegister* reg_next;
	while (reg) {
		Serial.print((u_long)reg);
		Serial.print(" ");
		Serial.println((u_long)reg->next);
		reg_next = reg->next;
		free(reg);
		if (reg_next != 0)
			reg = reg_next;
		else
			break;
	}
}

void Modbus::free_all_output_regs() {
	Serial.println("free_all_output_regs");
	OutputRegister* reg = _output_head;
	OutputRegister* reg_next;
	while (reg) {
		Serial.print((u_long)reg);
		Serial.print(" ");
		Serial.println((u_long)reg->next);
		reg_next = reg->next;
		free(reg);
		if (reg_next != 0)
			reg = reg_next;
		else
			break;
	}
}


