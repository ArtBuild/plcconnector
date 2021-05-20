#define DIGITAL_START_PIN 22
#define DIGITAL_END_PIN 100
#define PCF8574_START_PIN_I2C 100
#define PCF8574_END_PIN_I2C 164

#define DIGITAL_I2C_START_ADDRESS 0x10
#define PCF8574_I2C_START_ADDRESS 0x20

#define MAX_PIN_CNT 164

#define MODBUSIP_PORT 	  502
#define MODBUSIP_MAXFRAME 200

#define TCP_KEEP_ALIVE 1

#define REG_FLAG_REINIT_CONFIG 0
#define REG_PIN_COUNT 1

#define RELAY_ON LOW
#define RELAY_OFF HIGH
//Type Objects

enum objetsType {
	RELAY = 0x01,
	SERVO = 0x02,
	LED = 0x03,
	SENSOR_UP = 0x04,
	SENSOR_DOWN = 0x05,
	MOTOR = 0x06,
	ARROW = 0x07,
	TRAFFIC_LIGHT = 0x08,
	RAILWAY_TRAFFIC_LIGHT = 0x09,
	TRAIN = 0x10,
};

#define MAX_REGS     32
#define MAX_FRAME   128
//#define USE_HOLDING_REGISTERS_ONLY

typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned long u_long;


//Function Codes
enum funcCode {
	MB_FC_READ_COILS = 0x01, // Read Coils (Output) Status 0xxxx
	MB_FC_READ_INPUT_STAT = 0x02, // Read Input Status (Discrete Inputs) 1xxxx
	MB_FC_READ_REGS = 0x03, // Read Holding Registers 4xxxx
	MB_FC_READ_INPUT_REGS = 0x04, // Read Input Registers 3xxxx
	MB_FC_WRITE_COIL = 0x05, // Write Single Coil (Output) 0xxxx
	MB_FC_WRITE_REG = 0x06, // Preset Single Register 4xxxx
	MB_FC_WRITE_COILS = 0x0F, // Write Multiple Coils (Outputs) 0xxxx
	MB_FC_WRITE_REGS = 0x10, // Write block of contiguous registers 4xxxx
	MB_FC_WRITE_INPUT_REG = 0x30, // Write DIGITAL Register
	MB_FC_READ_OUTPUT_REG = 0x31, // Read DIGITALRegister
	MB_FC_SET_MODE_PINS = 0x50, // Reconfig PinMode in Arduino
};

//Exception Codes
enum ExceptCode {
	MB_EX_ILLEGAL_FUNCTION = 0x01, // Function Code not Supported
	MB_EX_ILLEGAL_ADDRESS = 0x02, // Output Address not exists
	MB_EX_ILLEGAL_VALUE = 0x03, // Output Value not in Range
	MB_EX_SLAVE_FAILURE = 0x04, // Slave Deive Fails to process request
};

//Reply Types
enum replyType {
	MB_REPLY_OFF = 0x01,
	MB_REPLY_ECHO = 0x02,
	MB_REPLY_NORMAL = 0x03,
};

typedef struct PinMode {
	u_short pin;
	byte mode;
} PinMode;

typedef struct InputRegister {
	u_short address;
	byte value;
	struct InputRegister* next;
} InputRegister;

typedef struct OutputRegister {
	u_short address;
	byte value;
	struct OutputRegister* next;
} OutputRegister;