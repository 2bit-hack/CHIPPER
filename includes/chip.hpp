#ifndef CHIP_HPP
#define CHIP_HPP

#define ROM_LOAD_ERR -1

using Byte = unsigned char;
using Opcode = unsigned short;

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Chip {
  public:
    std::vector<Byte> m_memory;
    int m_programCounter;
    std::vector<Byte> m_registers;
    std::vector<Byte> m_stack;
    int m_stackPointer;
    std::vector<bool> m_keys;
    std::vector<bool> m_frameBuffer;
    int m_delayTimer;
    int m_soundTimer;

    Chip();

    void loadROM(std::string filepath);
    void play();
};

#endif