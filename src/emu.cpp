#include "emu.hpp"

Emu::Emu() 
    {
        pc = START_ADDR;
        std::fill(ram.begin(), ram.end(), 0);
        std::fill(screen.begin(), screen.end(), false);
        std::fill(v_reg.begin(), v_reg.end(), false);
        i_reg = 0;
        sp = 0;
        std::fill(stack->begin(), stack->end(), 0);
        std::fill(keys.begin(), keys.end(), false);
        delayTimer = 0;
        soundTimer = 0;
    }

void Emu::push(uint16_t val)
{
    stack->push_back(val);
    sp++;
}

void Emu::pop()
{
    stack->pop_back();
    sp--;
}

void Emu::loadFontset()
{
    for(int i = 0; i<80; i++)
    {
        ram[i] = fontset[i];
    }
}

void Emu::reset()
{
    pc = START_ADDR;
    std::fill(ram.begin(), ram.end(), 0);
    std::fill(screen.begin(), screen.end(), false);
    std::fill(v_reg.begin(), v_reg.end(), false);
    i_reg = 0;
    sp = 0;
    std::fill(stack->begin(), stack->end(), 0);
    std::fill(keys.begin(), keys.end(), false);
    delayTimer = 0;
    soundTimer = 0;
}

void Emu::tick()
{
    uint16_t op = fetch();
}

uint16_t Emu::fetch()
{
    uint16_t higher_byte = ram[pc];
    uint16_t lower_byte = ram[pc+1];
    uint16_t op = (higher_byte << 8) | lower_byte;
    pc += 2;
    return op;
}

void Emu::tick_timers()
{
    if (delayTimer > 0)
    {
        delayTimer -= 1;
    }
    if (soundTimer > 0)
    {
        if (soundTimer == 1)
        {
            //BEEP
        }
        soundTimer -= 1;
    }
}

/*++++++++++++++++++++++++++++++++++++OPCODES FUNCTIONS**************************/


int main()
{
    Emu emulator;
    emulator.loadFontset();
    return 0;
}