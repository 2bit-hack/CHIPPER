#include "../includes/chip.hpp"

// initialize or reset the CHIP-8 system
Chip::Chip() {
    m_memory.clear();
    m_memory.resize(4096);

    m_registers.clear();
    m_registers.resize(16);

    m_stack.clear();
    m_stack.resize(16);

    m_stackPointer = 0;

    m_keys.clear();
    m_keys.resize(16);

    m_frameBuffer.clear();
    m_frameBuffer.resize(32 * 64);

    m_delayTimer = 0;
    m_soundTimer = 0;
}

// load a ROM into CHIP-8 memory
void Chip::loadROM(std::string filepath) {
    // reads rom as a binary file
    // set read pointer at the end to get size in bytes
    // and store in romSize
    size_t romSize;
    std::ifstream rom(filepath,
                      std::ios::in | std::ios::binary | std::ios::ate);
    if (rom.is_open()) {
        romSize = rom.tellg();
        rom.seekg(0, std::ios::beg);
        // hacky conversion of vector<unsigned char> to char*
        rom.read((char*)&m_memory[0], romSize);
        std::cout << "File size: " << romSize << "\n";
        rom.close();
    } else {
        std::cerr << "Failed to load ROM\n";
        exit(ROM_LOAD_ERR);
    }
}