#include "../includes/chip.hpp"

// initialize or reset the CHIP-8 system
Chip::Chip() {
    m_memory.clear();
    m_memory.resize(4096);

    m_programCounter = 0;
    m_indexRegister = 0;

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

// dumps contents of memory from 512 bytes onwards to stdout
// useful for quickly checking opcodes in a certain ROM
void Chip::debug_dumpMem() {
    for (int i = 512; i < 4096; i++) {
        std::cout << std::dec << i << " : " << std::hex << (int)m_memory[i]
                  << " ";
        if (i % 8 == 0)
            std::cout << "\n";
    }
    std::cout << "\n";
}

// load a ROM into CHIP-8 memory
bool Chip::loadROM(std::string filepath) {
    // reads rom as a binary file
    size_t romSize;
    std::ifstream rom(filepath,
                      std::ios::in | std::ios::binary | std::ios::ate);
    if (rom.is_open()) {
        romSize = rom.tellg();
        rom.seekg(0, std::ios::beg);
        // hacky conversion of vector<unsigned char> to char*
        rom.read((char*)&m_memory[512], romSize);
        rom.close();
        return true;
    } else {
        std::cerr << "Failed to load ROM\n";
        // TODO: cleanup if necessary
        return false;
    }
}

// fetch, decode, execute
void Chip::play() {
    // fetch
    // get two bytes from memory according to the program counter
    Opcode opcode = (Opcode)((m_memory[m_programCounter] << 8) |
                             m_memory[m_programCounter + 1]);

    // decode and execute
    // get first nibble (half a byte, same as a single hex char)
    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x00FF) {
        case 0x00E0:
            // 00E0
            // Clears the screen
            break;
        case 0x00EE:
            // 00EE
            // Returns from a subroutine
            break;
        default:
            std::cerr << "Illegal opcode encountered!\n";
        }
        break;
    case 0x1000:
        // 1NNN
        // Jumps to address NNN
        break;
    case 0x2000:
        // 2NNN
        // Calls subroutine at NNN
        break;
    case 0x3000:
        // 3XNN
        // Skips the next instruction if VX equals NN (Usually the next
        // instruction is a jump to skip a code block)
        break;
    case 0x4000:
        // 4XNN
        // Skips the next instruction if VX doesn't equal NN (Usually the
        // next instruction is a jump to skip a code block)
        break;
    case 0x5000:
        // 5XY0
        // Skips the next instruction if VX equals VY (Usually the next
        // instruction is a jump to skip a code block)
        break;
    case 0x6000:
        // 6XNN
        // Sets VX to NN
        break;
    case 0x7000:
        // 7XNN
        // Adds NN to VX (Carry flag is not changed)
        break;
    case 0x8000:
        switch (opcode & 0x000F) {
        case 0x0000:
            // 8XY0
            // Sets VX to the value of VY
            break;
        case 0x0001:
            // 8XY1
            // Sets VX to VX or VY (Bitwise OR operation)
            break;
        case 0x0002:
            // 8XY2
            // Sets VX to VX and VY (Bitwise AND operation)
            break;
        case 0x0003:
            // 8XY3
            // Sets VX to VX xor VY
            break;
        case 0x0004:
            // 8XY4
            // Adds VY to VX
            // VF is set to 1 when there's a carry, and to 0
            // when there isn't
            break;
        case 0x0005:
            // 8XY5
            // VY is subtracted from VX
            // VF is set to 0 when there's a
            // borrow, and 1 when there isn't
            break;
        case 0x0006:
            // 8XY6
            // Stores the least significant bit of VX in VF and then shifts
            // VX to the right by 1
            break;
        case 0x0007:
            // 8XY7
            // Sets VX to VY minus VX
            // VF is set to 0 when there's a borrow,
            // and 1 when there isn't
            break;
        case 0x000E:
            // 8XYE
            // Stores the most significant bit of VX in VF and then shifts VX
            // to the left by 1
            break;
        default:
            std::cerr << "Illegal opcode encountered!\n";
        }
        break;
    case 0x9000:
        // 9XY0
        // Skips the next instruction if VX doesn't equal VY (Usually the
        // next instruction is a jump to skip a code block)
        break;
    case 0xA000:
        // ANNN
        // Sets I to the address NNN
        break;
    case 0xB000:
        // BNNN
        // Jumps to the address NNN plus V0
        break;
    case 0xC000:
        // CXNN
        // Sets VX to the result of a bitwise and operation on a random
        // number (Typically: 0 to 255) and NN
        break;
    case 0xD000:
        // DXYN
        // Draws a sprite at coordinate (VX, VY) that has a width of 8
        // pixels and a height of N pixels. Each row of 8 pixels is read as
        // bit-coded starting from memory location I; I value doesn’t change
        // after the execution of this instruction. As described above, VF
        // is set to 1 if any screen pixels are flipped from set to unset
        // when the sprite is drawn, and to 0 if that doesn’t happen
        break;
    case 0xE000:
        switch (opcode & 0x000F) {
        case 0x000E:
            // EX9E
            // Skips the next instruction if the key stored in VX is
            // pressed. (Usually the next instruction is a jump to skip
            // a code block)
            break;
        case 0x0001:
            // EXA1
            // Skips the next instruction if the key stored in VX isn't
            // pressed. (Usually the next instruction is a jump to skip
            // a code block)
            break;
        default:
            std::cerr << "Illegal opcode encountered!\n";
        }
        break;
    case 0xF000:
        switch (opcode & 0x000F) {
        case 0x0007:
            // FX07
            // Sets VX to the value of the delay timer
            break;
        case 0x000A:
            // FX0A
            // A key press is awaited, and then stored in VX. (Blocking
            // Operation. All instruction halted until next key event)
            break;
        case 0x0005:
            switch (opcode & 0x00F0) {
            case 0x0010:
                // FX15
                // Sets the delay timer to VX
                break;
            case 0x0050:
                // FX55
                // Stores V0 to VX (including VX) in memory starting at
                // address I. The offset from I is increased by 1 for each
                // value written, but I itself is left unmodified
                break;
            case 0x0060:
                // FX65
                // Fills V0 to VX (including VX) with values from memory
                // starting at address I. The offset from I is increased by
                // 1 for each value written, but I itself is left unmodified
                break;
            default:
                std::cerr << "Illegal opcode encountered!\n";
            }
            break;
        case 0x0008:
            // FX18
            // Sets the sound timer to VX
            break;
        case 0x000E:
            // FX1E
            // Adds VX to I. VF is set to 1 when there is a range overflow
            // (I+VX>0xFFF), and to 0 when there isn't
            break;
        case 0x0009:
            // FX29
            // Sets I to the location of the sprite for the character in VX.
            // Characters 0-F (in hexadecimal) are represented by a 4x5 font
            break;
        case 0x0003:
            // FX33
            // Stores the binary-coded decimal representation of VX, with the
            // most significant of three digits at the address in I, the middle
            // digit at I plus 1, and the least significant digit at I plus 2.
            // (In other words, take the decimal representation of VX, place the
            // hundreds digit in memory at location in I, the tens digit at
            // location I+1, and the ones digit at location I+2.)
            break;
        default:
            std::cerr << "Illegal opcode encountered!\n";
        }
        break;
    default:
        std::cerr << "Illegal opcode encountered!\n";
    }
}