#include <cstdlib>
#include <stdint.h>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include "lib.cpp"

#define START_ADDR 0x200
#define RAM_SIZE 4096
#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 64
#define NUM_REGS 16
#define STACK_SIZE 16
#define NUM_KEYS 16

class Emu
{

    uint16_t pc;
    std::array<uint8_t, RAM_SIZE> ram;

    std::array<uint8_t, NUM_REGS> v_reg;
    uint16_t i_reg;

    

    std::array<uint16_t, STACK_SIZE> stack;
    uint16_t sp;

    std::array<bool, NUM_KEYS> keys;
    uint8_t delayTimer;
    uint8_t soundTimer;

    uint16_t opcode;

    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

    //Operation Tables
    typedef void (Emu::*Chip8Func)();
	Chip8Func table[0xF + 1];
	Chip8Func table0[0xE + 1];
	Chip8Func table8[0xE + 1];
	Chip8Func tableE[0xE + 1];
	Chip8Func tableF[0x65 + 1];

    //func
    uint16_t fetch();
    

    public:
    Emu();
    uint32_t screen[SCREEN_HEIGHT*SCREEN_WIDTH];
    // void push(uint16_t val);
    // uint16_t pop();
    void execute();
    // void loadFontset();
    void reset();
    void tick();
    void tick_timers();

    //ptr table functions
    void Table0();

	void Table8();

	void TableE();

	void TableF();

	void OP_NULL();

    //INSTRUCTIONS
    void OP_00E0();
    void OP_00EE();
    void OP_1nnn();
    void OP_2nnn();
    void OP_3xnn();
    void OP_4xnn();
    void OP_5xy0();
    void OP_6xnn();
    void OP_7xnn();
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();
    void OP_9xy0();
    void OP_Annn();
    void OP_Bnnn();
    void OP_Cxnn();
    void OP_Dxyn();
    void OP_Ex9E();
    void OP_ExA1();
    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();

    //front-end
    // std::array<bool, SCREEN_HEIGHT*SCREEN_WIDTH> get_screen();
    std::array<bool, NUM_KEYS> get_keys();
    void keyPress(uint8_t index, bool pressed);
    void loadROM(char const * filename);

};