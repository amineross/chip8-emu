#include "emu.cpp"
#include "machine.cpp"


int main(int argc, char * argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] <<  " <SCALE> <DELAY> <ROM/PATH>\n";
        return 1;
    }
    int scale = std::stoi(argv[1]);
    int delay = std::stoi(argv[2]);

    char const* rom_filename = argv[3];

    Machine machine("CHIP-8 Emu", SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale, SCREEN_WIDTH, SCREEN_HEIGHT);

    Emu emulator;
    emulator.loadROM(rom_filename);


    int videoPitch = sizeof(emulator.screen[0]) * SCREEN_WIDTH;

    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    while (!quit)
    {
        quit = machine.processInput(emulator.get_keys());

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > delay)
        {
            lastCycleTime = currentTime;
            emulator.execute();
            machine.Update(emulator.screen, videoPitch);
        }
    }
    return 0;
}

