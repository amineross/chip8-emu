#include <cstdlib>
#include <stdint.h>
#include <array>
#include <vector>
#include "lib.cpp"

#define START_ADDR 0x200
#define RAM_SIZE 4096
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 32
#define NUM_REGS 16
#define STACK_SIZE 16
#define NUM_KEYS 16

class Emu
{

    uint16_t pc;
    std::array<uint8_t, RAM_SIZE> ram;

    std::array<uint8_t, NUM_REGS> v_reg;
    uint16_t i_reg;

    std::array<bool, SCREEN_HEIGHT*SCREEN_WIDTH> screen;

    std::vector<uint16_t> stack[STACK_SIZE];
    uint16_t sp;

    std::array<bool, NUM_KEYS> keys;
    uint8_t delayTimer;
    uint8_t soundTimer;

    //func
    uint16_t fetch();
    void execute();

    public:
    Emu();
    void push(uint16_t val);
    void pop();
    void loadFontset();
    void reset();
    void tick();
    void tick_timers();

    //INSTRUCTIONS 💀
    void OP_00E0();
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XNN();
    void OP_4XNN();
    void OP_5XY0();
    void OP_6XNN();
    void OP_7XNN();
    void OP_8XY0();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();
    void OP_9XY0();
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXNN();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();


};