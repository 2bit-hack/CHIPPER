#include "../includes/chip.hpp"

int main() {
    std::string filepath = "./roms/PONG";
    Chip* chip = new Chip();
    bool loaded = chip->loadROM(filepath);
    if (!loaded)
        exit(ROM_LOAD_ERR);

    // It works!
    // chip->m_memory[0] = 0xab;
    // chip->m_memory[1] = 0xcd;
    // chip->play();

    chip->debug_dumpMem();

    return 0;
}