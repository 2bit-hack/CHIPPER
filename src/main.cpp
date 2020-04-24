#include "../includes/chip.hpp"

int main() {
    std::string filepath = "./roms/PONG";
    Chip* chip = new Chip();
    chip->loadROM(filepath);

    return 0;
}