#ifndef CHIP_HPP
#define CHIP_HPP

#define ROM_LOAD_ERR -1

using byte = unsigned char;

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Chip {

  public:
    std::vector<byte> m_memory;
    std::vector<byte> m_registers;
    std::vector<byte> m_stack;
    int m_stackPointer;
    std::vector<bool> m_keys;
    std::vector<bool> m_frameBuffer;
    int m_delayTimer;
    int m_soundTimer;

    Chip();

    void loadROM(std::string filepath);
    void run();
};

#endif