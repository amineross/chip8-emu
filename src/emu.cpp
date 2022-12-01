#include "emu.hpp"

Emu::Emu(): randGen(std::chrono::system_clock::now().time_since_epoch().count())
    {
        pc = START_ADDR;
        memset(screen, 0, sizeof(screen));
        // std::fill(ram.begin(), ram.end(), 0);
        for(unsigned int i = 0; i<80; i++)
        {
            ram[i] = fontset[i];
        }

        // std::fill(v_reg.begin(), v_reg.end(), 0);
        // i_reg = 0;
        // sp = 0;
        // std::fill(stack.begin(), stack.end(), 0);
        // std::fill(keys.begin(), keys.end(), false);
        memset(keys, 0, sizeof(keys));
        // delayTimer = 0;
        // soundTimer = 0;

        randByte = std::uniform_int_distribution<uint8_t>(0,255u);

        // Setting up function pointer tables
		table[0x0] = &Emu::Table0;
		table[0x1] = &Emu::OP_1nnn;
		table[0x2] = &Emu::OP_2nnn;
		table[0x3] = &Emu::OP_3xnn;
		table[0x4] = &Emu::OP_4xnn;
		table[0x5] = &Emu::OP_5xy0;
		table[0x6] = &Emu::OP_6xnn;
		table[0x7] = &Emu::OP_7xnn;
		table[0x8] = &Emu::Table8;
		table[0x9] = &Emu::OP_9xy0;
		table[0xA] = &Emu::OP_Annn;
		table[0xB] = &Emu::OP_Bnnn;
		table[0xC] = &Emu::OP_Cxnn;
		table[0xD] = &Emu::OP_Dxyn;
		table[0xE] = &Emu::TableE;
		table[0xF] = &Emu::TableF;

		for (size_t i = 0; i <= 0xE; i++)
		{
			table0[i] = &Emu::OP_NULL;
			table8[i] = &Emu::OP_NULL;
			tableE[i] = &Emu::OP_NULL;
		}

		table0[0x0] = &Emu::OP_00E0;
		table0[0xE] = &Emu::OP_00EE;

		table8[0x0] = &Emu::OP_8xy0;
		table8[0x1] = &Emu::OP_8xy1;
		table8[0x2] = &Emu::OP_8xy2;
		table8[0x3] = &Emu::OP_8xy3;
		table8[0x4] = &Emu::OP_8xy4;
		table8[0x5] = &Emu::OP_8xy5;
		table8[0x6] = &Emu::OP_8xy6;
		table8[0x7] = &Emu::OP_8xy7;
		table8[0xE] = &Emu::OP_8xyE;

		tableE[0x1] = &Emu::OP_ExA1;
		tableE[0xE] = &Emu::OP_Ex9E;

		for (size_t i = 0; i <= 0x65; i++)
		{
			tableF[i] = &Emu::OP_NULL;
		}

		tableF[0x07] = &Emu::OP_Fx07;
		tableF[0x0A] = &Emu::OP_Fx0A;
		tableF[0x15] = &Emu::OP_Fx15;
		tableF[0x18] = &Emu::OP_Fx18;
		tableF[0x1E] = &Emu::OP_Fx1E;
		tableF[0x29] = &Emu::OP_Fx29;
		tableF[0x33] = &Emu::OP_Fx33;
		tableF[0x55] = &Emu::OP_Fx55;
		tableF[0x65] = &Emu::OP_Fx65;
    }


void Emu::reset()
{
    // pc = START_ADDR;
    // std::fill(ram.begin(), ram.end(), 0);
    // std::fill(v_reg.begin(), v_reg.end(), false);
    // i_reg = 0;
    // sp = 0;
    // std::fill(stack.begin(), stack.end(), 0);
    // std::fill(keys.begin(), keys.end(), false);
    // delayTimer = 0;
    // soundTimer = 0;
}

void Emu::tick()
{
    uint16_t op = fetch();
    opcode = op;
}

uint16_t Emu::fetch()
{
    uint16_t higher_byte = ram[pc];
    uint16_t lower_byte = ram[pc+1];
    uint16_t op = (higher_byte << 8) | lower_byte;
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
            std::cout << "BEEP\n";
        }
        soundTimer -= 1;
    }
}
void Emu::Table0()
	{
		((*this).*(table0[opcode & 0x000Fu]))();
	}

void Emu::Table8()
{
    ((*this).*(table8[opcode & 0x000Fu]))();
}

void Emu::TableE()
{
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

void Emu::TableF()
{
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

void Emu::OP_NULL()
{}

void Emu::execute()
{
    tick();
    //opcode = (ram[pc] << 8 | ram[pc+1]);
    pc += 2;
    //DEBUG: std::cout << "OPCODE " << std::hex << opcode << "\n";
    ((*this).*(table[(opcode & 0xF000) >> 12]))();

    tick_timers();
}


/*++++++++++++++++++++++++++++++++++++OPCODES FUNCTIONS**************************/
void Emu::OP_00E0() { memset(screen, 0, sizeof(screen)); } //clear screen

void Emu::OP_00EE() // return from subroutine (function basically)
{
    sp--;
    pc = stack[sp];
}

void Emu::OP_1nnn() // Jump
{
    uint16_t nnn = opcode & 0x0FFF;
    pc = nnn;
}

void Emu::OP_2nnn() // Call subroutine
{
    uint16_t nnn = opcode & 0x0FFF;
    stack[sp] = pc;
    sp++;
    pc = nnn;
}

void Emu::OP_3xnn() // skip if Vx = NN which implements conditionals 
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    if (v_reg[x] == nn)
    {
        pc += 2;
    }
}

void Emu::OP_4xnn()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    if (v_reg[x] != nn)
    {
        pc += 2;
    }
}

void Emu::OP_5xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (v_reg[x] == v_reg[y])
    {
        pc += 2;
    }
}

void Emu::OP_6xnn()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    v_reg[x] = nn;
}

void Emu::OP_7xnn()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    v_reg[x] += nn;
}

void Emu::OP_8xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    v_reg[x] = v_reg[y];
}

void Emu::OP_8xy1()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    v_reg[x] |= v_reg[y];
}

void Emu::OP_8xy2()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    v_reg[x] &= v_reg[y];
}

void Emu::OP_8xy3()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    v_reg[x] ^= v_reg[y];
}

void Emu::OP_8xy4()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    uint16_t sum = v_reg[x] + v_reg[y];

    if (sum > 255U)
    {
        v_reg[0xF] = 1;
    } else 
    {
        v_reg[0xF] = 0;
    }

    v_reg[x] = sum & 0xFF;
}

void Emu::OP_8xy5()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (v_reg[x] > v_reg[y])
    {
        v_reg[0xF] = 1;
    } else 
    {
        v_reg[0xF] = 0;
    }
    v_reg[x] -= v_reg[y];
}

void Emu::OP_8xy6()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    v_reg[0xF] = v_reg[x] & 0x1; // lsb in VF
    v_reg[x] >>= 1;
}

void Emu::OP_8xy7()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (v_reg[y] > v_reg[x])
    {
        v_reg[0xF] = 1;
    } else
    {
        v_reg[0xF] = 0;
    }
    v_reg[x] = v_reg[y] - v_reg[x];
}

void Emu::OP_8xyE()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    v_reg[0xF] = (v_reg[x] & 0x80) >> 7;
    v_reg[x] <<= 1;
}

void Emu::OP_9xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (v_reg[x] != v_reg[y])
    {
        pc += 2;
    }
}

void Emu::OP_Annn()
{
    uint16_t nnn = (opcode & 0x0FFF);
    i_reg = nnn;
}

void Emu::OP_Bnnn() // jump to v0 + nnn
{
    uint8_t nnn = (opcode & 0x0FFF);
    pc = v_reg[0] + nnn;
}

void Emu::OP_Cxnn()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    v_reg[x] = randByte(randGen) & nn;
}

// void Emu::OP_Dxyn() //Draw coord: (X, Y) ; width: 8bits from RAM ; height: N
// {
//     uint8_t x_coord = v_reg[(opcode & 0x0F00) >> 8] % SCREEN_WIDTH;
//     uint8_t y_coord = v_reg[(opcode & 0x00F0) >> 4] % SCREEN_HEIGHT;
//     uint8_t n_lines = (opcode & 0x000F);
//     // bool flipped = false;
//     v_reg[0xF] = 0;

//     for (unsigned int line = 0; line<n_lines; line++)
//     {
//         uint8_t spriteByte = ram[i_reg + line];

//         for (unsigned int col = 0; col<8; col++)
//         {
//             uint8_t spritePixel = spriteByte & (0x80 >> col);
//             uint32_t* screenPixel = &screen[(y_coord + line) * SCREEN_WIDTH + (x_coord + col)];
//             if (spritePixel)
//             {
//                 if (*screenPixel == 0xFFFFFFFF)
//                 {
//                     v_reg[0xF] = 1;
//                 }
//                 *screenPixel ^= 0xFFFFFFFF;
//             }
//         }
//     }
// }

void Emu::OP_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = v_reg[Vx] % SCREEN_WIDTH;
	uint8_t yPos = v_reg[Vy] % SCREEN_HEIGHT;

	v_reg[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = ram[i_reg + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &screen[(yPos + row) * SCREEN_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					v_reg[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Emu::OP_Ex9E() //Skip is keys[Vx] is pressed
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t Vx = v_reg[x];
    if (keys[Vx])
    {
        pc += 2;
    }
}

void Emu::OP_ExA1() //Skip is keys[Vx] is not pressed
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t Vx = v_reg[x];
    if (!keys[Vx])
    {
        pc += 2;
    }
}

void Emu::OP_Fx07() //Vx = DelayTimer 
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    v_reg[x] = delayTimer;
}

// void Emu::OP_Fx0A() // wait for key press (endless loop until pressed)
// {
//     uint8_t x = (opcode & 0x0F00) >> 8;
//     bool pressed = false;
//     for (int i = 0; i<NUM_KEYS; i++)
//     {
//         if (keys[i])
//         {
//             v_reg[x] = i;
//             pressed = true;
//             break;
//         }
//     }
//     if (!pressed)
//     {
//         pc -= 2;
//     }
// }

void Emu::OP_Fx0A()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    if (keys[0])
	{
		v_reg[x] = 0;
	}
	else if (keys[1])
	{
		v_reg[x] = 1;
	}
	else if (keys[2])
	{
		v_reg[x] = 2;
	}
	else if (keys[3])
	{
		v_reg[x] = 3;
	}
	else if (keys[4])
	{
		v_reg[x] = 4;
	}
	else if (keys[5])
	{
		v_reg[x] = 5;
	}
	else if (keys[6])
	{
		v_reg[x] = 6;
	}
	else if (keys[7])
	{
		v_reg[x] = 7;
	}
	else if (keys[8])
	{
		v_reg[x] = 8;
	}
	else if (keys[9])
	{
		v_reg[x] = 9;
	}
	else if (keys[10])
	{
		v_reg[x] = 10;
	}
	else if (keys[11])
	{
		v_reg[x] = 11;
	}
	else if (keys[12])
	{
		v_reg[x] = 12;
	}
	else if (keys[13])
	{
		v_reg[x] = 13;
	}
	else if (keys[14])
	{
		v_reg[x] = 14;
	}
	else if (keys[15])
	{
		v_reg[x] = 15;
	}
	else
	{
		pc -= 2;
	}
}

void Emu::OP_Fx15() // delayTimer = Vx
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    delayTimer = v_reg[x];
}

void Emu::OP_Fx18() // soundTimer = Vx
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    soundTimer = v_reg[x];
}

void Emu::OP_Fx1E() //  I_reg += Vx
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    i_reg += v_reg[x];
}

void Emu::OP_Fx29() //  I_reg = font_addr
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    i_reg = v_reg[x] * 5;
    // since our fonts are stored in the very beginning of the RAM
    /*
        *-------------------------*
        | Character | RAM Address |
        ---------------------------
        | 0         | 0*5 = 0     |
        | 1         | 1*5 = 5     |
        | 2         | 2*5 = 10    |
        | ...       | ...         |
        *-------------------------*
    */
}

void Emu::OP_Fx33() // Binary-Coded Decimal; turn Vx into a decimal number of 3 bytes
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t Vx = v_reg[x];
    ram[i_reg + 2] = Vx % 10;
    Vx /= 10;
    ram[i_reg + 1] = Vx % 10;
    Vx /= 10;
    ram[i_reg] = Vx % 10;
}

void Emu::OP_Fx55() // populate ram with v0 through Vx (inclusive) starting at i_reg addr
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint16_t i = i_reg; // starting at i
    for (int index = 0; index <= x; index++)
    {
        ram[i + index] = v_reg[index];
    }
}

void Emu::OP_Fx65()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint16_t i = i_reg; // starting at i
    for (int index = 0; index <= x; index++)
    {
        v_reg[index] = ram[i + index];
    }
}

// std::array<bool, NUM_KEYS> Emu::get_keys()
// {
//     return keys;
// }

void Emu::keyPress(uint8_t index, bool pressed)
{
    keys[index] = pressed;
}

void Emu::loadROM(char const * filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        std::streampos size = file.tellg();
        char * buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (long i = 0; i<size; i++)
        {
            ram[START_ADDR + i] = buffer[i];
        }

        delete[] buffer;
        std::cout << "ROM Loaded Successfuly\n";
    }

    std::ofstream myfile ("ram_dump", std::ios::out | std::ios::binary);

    if (myfile.is_open())
    {
        myfile.write((char*) &ram, sizeof(uint8_t) * RAM_SIZE);
        myfile.close();
    } else std::cout << "ERROR: RAM DUMP FAILED.\n";
    
}

