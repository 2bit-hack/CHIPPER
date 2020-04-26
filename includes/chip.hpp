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
    // 4KB memory, first 512 bytes are reserved
    // ROM is loaded after that
    std::vector<Byte> m_memory;
    int m_programCounter;
    // stores a 12 bit address pointer
    int m_indexRegister;
    // V0 - VF
    std::vector<Byte> m_registers;
    // possibly 12 - 16 levels of pushing
    std::vector<Byte> m_stack;
    int m_stackPointer;
    // 16 keys
    std::vector<bool> m_keys;
    // 32 rows, 64 columns
    std::vector<bool> m_frameBuffer;
    // count down at 60hz unless they are 0
    int m_delayTimer;
    int m_soundTimer;

    // not part of chip8, useful for performance reasons
    // only draw when this flag is set
    bool m_drawFlag;

    Chip();

    void debug_dumpMem();

    void loadFont();
    bool loadROM(std::string filepath);
    void play();
};

#endif