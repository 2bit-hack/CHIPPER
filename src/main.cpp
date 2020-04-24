#include "../includes/chip.hpp"

int main() {
    std::string filepath = "./roms/PONG";
    Chip* chip = new Chip();
    chip->loadROM(filepath);

    // It works!
    // chip->m_memory[0] = 0xab;
    // chip->m_memory[1] = 0xcd;
    // chip->play();

    return 0;
}