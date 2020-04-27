#include "../includes/chip.hpp"

int main() {
    std::string filepath = "./roms/PONG";
    Chip* chip = new Chip();
    bool loaded = chip->loadROM(filepath);
    if (!loaded)
        exit(ROM_LOAD_ERR);

    chip->debug_dumpMem();

    while (true) {
        chip->play();
    }

    return 0;
}