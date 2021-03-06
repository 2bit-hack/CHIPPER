#include "../includes/chip.hpp"

// initialize or reset the CHIP-8 system
Chip::Chip() {
    m_memory.clear();
    m_memory.resize(4096);

    loadFont();

    // setting up rng for opcode 0xCXNN
    srand(time(NULL));

    m_programCounter = 0x0200; // 512 bytes
    m_indexRegister = 0;

    m_registers.clear();
    m_registers.resize(16);

    m_stack.clear();
    m_stack.resize(16);

    m_stackPointer = 0;

    m_keys.clear();
    m_keys.resize(16);

    m_frameBuffer.clear();
    m_frameBuffer.resize(64 * 32);

    m_delayTimer = 0;
    m_soundTimer = 0;

    m_drawFlag = false;
}

// same as the constructor, but for reseting
void Chip::reset() {
    m_memory.clear();
    m_memory.resize(4096);

    loadFont();

    // setting up rng for opcode 0xCXNN
    srand(time(NULL));

    m_programCounter = 0x0200; // 512 bytes
    m_indexRegister = 0;

    m_registers.clear();
    m_registers.resize(16);

    m_stack.clear();
    m_stack.resize(16);

    m_stackPointer = 0;

    m_keys.clear();
    m_keys.resize(16);

    m_frameBuffer.clear();
    m_frameBuffer.resize(64 * 32);

    m_delayTimer = 0;
    m_soundTimer = 0;

    m_drawFlag = false;
}

// dumps contents of memory to stdout
// useful for quickly checking opcodes in a certain ROM
void Chip::debug_dumpMem() {
    for (int i = 0x0000; i <= 0x0FFF; i++) {
        std::cout << std::dec << i << " : " << std::hex << (int)m_memory[i]
                  << " ";
        if (i % 8 == 0)
            std::cout << "\n";
    }
    std::cout << "\n";
}

// prints out useful information regarding the state of the machine
void Chip::debug_instructions(Opcode opcode) {
    std::cout << "Opcode: " << std::hex << opcode << std::dec << "\n";
    std::cout << "PC: " << std::hex << m_programCounter << std::dec << "\n";
    std::cout << "I:  " << std::hex << m_indexRegister << std::dec << "\n";
    std::cout << "SP: " << std::hex << m_stackPointer << std::dec << "\n\n";
}

// load CHIP-8 fontset into memory
// memory from 0 to 512 is basically empty, so we use the first 80 bytes to
// store this
void Chip::loadFont() {
    // font taken from
    // http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
    Byte chip8_fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    for (int i = 0x0000; i < 0x0050; i++) {
        m_memory[i] = chip8_fontset[i];
    }
}

// load a ROM into CHIP-8 memory
// we start loading the ROM from the 512th byte onwards
bool Chip::loadROM(std::string filepath) {
    // reads rom as a binary file
    size_t romSize;
    std::ifstream rom(filepath,
                      std::ios::in | std::ios::binary | std::ios::ate);
    if (rom.is_open()) {
        romSize = rom.tellg();
        rom.seekg(0, std::ios::beg);
        // hacky conversion of vector<unsigned char> to char*
        rom.read((char*)&m_memory[0x0200], romSize);
        rom.close();
        return true;
    } else {
        std::cerr << "Failed to load ROM\n";
        return false;
    }
}

// fetch, decode, execute
void Chip::play() {

    // utility variables
    unsigned short randomNumber; // really only needs 1 byte, but unsigned short
                                 // for consistency
    unsigned short N;
    unsigned short NN;
    unsigned short NNN;
    unsigned short X;
    unsigned short Y;

    // fetch
    // get two bytes from memory according to the program counter
    Opcode opcode = (Opcode)((m_memory[m_programCounter] << 8) |
                             m_memory[m_programCounter + 1]);

    // debug_instructions(opcode);

    // decode and execute
    // get first nibble (half a byte, same as a single hex char)
    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x00FF) {
        case 0x00E0:
            // 00E0
            // Clears the screen

            for (int i = 0; i < 64 * 32; i++) {
                m_frameBuffer[i] = false;
            }
            m_drawFlag = true;
            m_programCounter += 2;

            break;
        case 0x00EE:
            // 00EE
            // Returns from a subroutine

            m_stackPointer--;
            m_programCounter = m_stack[m_stackPointer];
            m_programCounter += 2;

            break;
        default:
            std::cerr << "Illegal opcode encountered! " << std::hex
                      << (int)opcode << std::dec << "\n";
            exit(ILLEGAL_OPCODE_ERR);
            break;
        }
        break;
    case 0x1000:
        // 1NNN
        // Jumps to address NNN

        NNN = opcode & 0x0FFF;
        m_programCounter = NNN;

        break;
    case 0x2000:
        // 2NNN
        // Calls subroutine at NNN

        NNN = opcode & 0x0FFF;
        m_stack[m_stackPointer] = m_programCounter;
        m_stackPointer++;
        m_programCounter = NNN;

        break;
    case 0x3000:
        // 3XNN
        // Skips the next instruction if VX equals NN (Usually the next
        // instruction is a jump to skip a code block)

        X = (opcode & 0x0F00) >> 8;
        NN = opcode & 0x00FF;
        if (m_registers[X] == NN)
            m_programCounter += 4;
        else
            m_programCounter += 2;

        break;
    case 0x4000:
        // 4XNN
        // Skips the next instruction if VX doesn't equal NN (Usually the
        // next instruction is a jump to skip a code block)

        X = (opcode & 0x0F00) >> 8;
        NN = opcode & 0x00FF;
        if (m_registers[X] != NN)
            m_programCounter += 4;
        else
            m_programCounter += 2;

        break;
    case 0x5000:
        // 5XY0
        // Skips the next instruction if VX equals VY (Usually the next
        // instruction is a jump to skip a code block)

        X = (opcode & 0x0F00) >> 8;
        Y = (opcode & 0x00F0) >> 4;
        if (m_registers[X] == m_registers[Y])
            m_programCounter += 4;
        else
            m_programCounter += 2;

        break;
    case 0x6000:
        // 6XNN
        // Sets VX to NN

        X = (opcode & 0x0F00) >> 8;
        NN = opcode & 0x00FF;
        m_registers[X] = NN;
        m_programCounter += 2;

        break;
    case 0x7000:
        // 7XNN
        // Adds NN to VX (Carry flag is not changed)

        X = (opcode & 0x0F00) >> 8;
        NN = opcode & 0x00FF;
        m_registers[X] += NN;
        m_programCounter += 2;

        break;
    case 0x8000:
        switch (opcode & 0x000F) {
        case 0x0000:
            // 8XY0
            // Sets VX to the value of VY

            X = (opcode & 0x0F00) >> 8;
            Y = (opcode & 0x00F0) >> 4;
            m_registers[X] = m_registers[Y];
            m_programCounter += 2;

            break;
        case 0x0001:
            // 8XY1
            // Sets VX to VX or VY (Bitwise OR operation)

            X = (opcode & 0x0F00) >> 8;
            Y = (opcode & 0x00F0) >> 4;
            m_registers[X] = m_registers[X] | m_registers[Y];
            m_registers[0x000F] = 0;
            m_programCounter += 2;

            break;
        case 0x0002:
            // 8XY2
            // Sets VX to VX and VY (Bitwise AND operation)

            X = (opcode & 0x0F00) >> 8;
            Y = (opcode & 0x00F0) >> 4;
            m_registers[X] = m_registers[X] & m_registers[Y];
            m_registers[0x000F] = 0;
            m_programCounter += 2;

            break;
        case 0x0003:
            // 8XY3
            // Sets VX to VX xor VY

            X = (opcode & 0x0F00) >> 8;
            Y = (opcode & 0x00F0) >> 4;
            m_registers[X] = m_registers[X] ^ m_registers[Y];
            m_registers[0x000F] = 0;
            m_programCounter += 2;

            break;
        case 0x0004:
            // 8XY4
            // Adds VY to VX
            // VF is set to 1 when there's a carry, and to 0
            // when there isn't

            X = (opcode & 0x0F00) >> 8;
            Y = (opcode & 0x00F0) >> 4;
            if (m_registers[X] + m_registers[Y] > 0x00FF)
                m_registers[0x000F] = 1;
            else
                m_registers[0x000F] = 0;
            m_registers[X] = (Byte)(m_registers[X] + m_registers[Y]);
            m_programCounter += 2;

            break;
        case 0x0005:
            // 8XY5
            // VY is subtracted from VX
            // VF is set to 0 when there's a
            // borrow, and 1 when there isn't

            X = (opcode & 0x0F00) >> 8;
            Y = (opcode & 0x00F0) >> 4;
            if (m_registers[X] < m_registers[Y])
                m_registers[0x000F] = 0;
            else
                m_registers[0x000F] = 1;
            m_registers[X] = (Byte)(m_registers[X] - m_registers[Y]);
            m_programCounter += 2;

            break;
        case 0x0006:
            // 8XY6
            // Stores the least significant bit of VX in VF and then shifts
            // VX to the right by 1

            X = (opcode & 0x0F00) >> 8;
            // Y is never used for some reason
            Y = (opcode & 0x00F0) >> 4;
            m_registers[0x000F] = m_registers[X] & 0x0001;
            m_registers[X] = (Byte)m_registers[X] >> 1;
            m_programCounter += 2;

            break;
        case 0x0007:
            // 8XY7
            // Sets VX to VY minus VX
            // VF is set to 0 when there's a borrow,
            // and 1 when there isn't

            X = (opcode & 0x0F00) >> 8;
            Y = (opcode & 0x00F0) >> 4;
            if (m_registers[Y] < m_registers[X])
                m_registers[0x000F] = 0;
            else
                m_registers[0x000F] = 1;
            m_registers[X] = (Byte)(m_registers[Y] - m_registers[X]);
            m_programCounter += 2;

            break;
        case 0x000E:
            // 8XYE
            // Stores the most significant bit of VX in VF and then shifts VX
            // to the left by 1

            X = (opcode & 0x0F00) >> 8;
            // Again, Y is not used
            Y = (opcode & 0x00F0) >> 4;
            m_registers[0x000F] = (m_registers[X] & 0x0080) >> 7;
            m_registers[X] = (Byte)(m_registers[X] << 1);
            m_programCounter += 2;

            break;
        default:
            std::cerr << "Illegal opcode encountered! " << std::hex
                      << (int)opcode << std::dec << "\n";
            exit(ILLEGAL_OPCODE_ERR);
            break;
        }
        break;
    case 0x9000:
        // 9XY0
        // Skips the next instruction if VX doesn't equal VY (Usually the
        // next instruction is a jump to skip a code block)

        X = (opcode & 0x0F00) >> 8;
        Y = (opcode & 0x00F0) >> 4;
        if (m_registers[X] != m_registers[Y])
            m_programCounter += 4;
        else
            m_programCounter += 2;

        break;
    case 0xA000:
        // ANNN
        // Sets I to the address NNN

        NNN = opcode & 0x0FFF;
        m_indexRegister = NNN;
        m_programCounter += 2;

        break;
    case 0xB000:
        // BNNN
        // Jumps to the address NNN plus V0

        NNN = opcode & 0x0FFF;
        m_programCounter = NNN + m_registers[0x0000];

        break;
    case 0xC000:
        // CXNN
        // Sets VX to the result of a bitwise and operation on a random
        // number (Typically: 0 to 255) and NN

        X = (opcode & 0x0F00) >> 8;
        NN = opcode & 0x00FF;
        // usually, rand() should be avoided
        // might change to more "true" rng later
        randomNumber = rand() % 256;
        m_registers[X] = randomNumber & NN;
        m_programCounter += 2;

        break;
    case 0xD000: {
        // DXYN
        // Draws a sprite at coordinate (VX, VY) that has a width of 8
        // pixels and a height of N pixels. Each row of 8 pixels is read as
        // bit-coded starting from memory location I; I value doesn’t change
        // after the execution of this instruction. As described above, VF
        // is set to 1 if any screen pixels are flipped from set to unset
        // when the sprite is drawn, and to 0 if that doesn’t happen

        X = (opcode & 0x0F00) >> 8;
        Y = (opcode & 0x00F0) >> 4;
        N = opcode & 0x000F;
        Byte x = m_registers[X];
        Byte y = m_registers[Y];
        Byte height = N;
        Byte width = 8;
        Byte pixelRow;
        // if no collisions, this will remain 0
        m_registers[0x000F] = 0;
        for (int i = 0; i < height; i++) {
            pixelRow = m_memory[m_indexRegister + i];
            for (int j = 0; j < width; j++) {
                // get each pixel in pixelRow
                // 128b10 = 10000000b2
                // we right shift from 0 to 7 to get each bit
                if ((pixelRow & (0x0080 >> j)) != 0) {
                    // finding the x,y coordinate in the 1D framebuffer
                    // row major
                    int mapXYto1DArr = (x + j + ((y + i) * 64)) % 0x0800;
                    if (m_frameBuffer[mapXYto1DArr]) {
                        // xor mode drawing
                        // if that pixel is 1, then collision occurs
                        // (both pixels are 1)
                        // and the pixel is xor'd to 0
                        // we set VF to 1
                        m_registers[0x000F] = 1;
                    }
                    m_frameBuffer[mapXYto1DArr] =
                        (bool)(m_frameBuffer[mapXYto1DArr] ^ 1);
                }
            }
        }
        m_drawFlag = true;
        m_programCounter += 2;

    }

    break;
    case 0xE000:
        switch (opcode & 0x000F) {
        case 0x000E:
            // EX9E
            // Skips the next instruction if the key stored in VX is
            // pressed. (Usually the next instruction is a jump to skip
            // a code block)

            X = (opcode & 0x0F00) >> 8;
            if (m_keys[m_registers[X]])
                m_programCounter += 4;
            else
                m_programCounter += 2;

            break;
        case 0x0001:
            // EXA1
            // Skips the next instruction if the key stored in VX isn't
            // pressed. (Usually the next instruction is a jump to skip
            // a code block)

            X = (opcode & 0x0F00) >> 8;
            if (!m_keys[m_registers[X]])
                m_programCounter += 4;
            else
                m_programCounter += 2;

            break;
        default:
            std::cerr << "Illegal opcode encountered! " << std::hex
                      << (int)opcode << std::dec << "\n";
            exit(ILLEGAL_OPCODE_ERR);
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0x000F) {
        case 0x0007:
            // FX07
            // Sets VX to the value of the delay timer

            X = (opcode & 0x0F00) >> 8;
            m_registers[X] = m_delayTimer;
            m_programCounter += 2;

            break;
        case 0x000A: {
            // FX0A
            // A key press is awaited, and then stored in VX. (Blocking
            // Operation. All instruction halted until next key event)

            X = (opcode & 0x0F00) >> 8;
            bool isKeyPressed = false;
            for (int i = 0; i < 16; i++) {
                if (m_keys[i]) {
                    m_registers[X] = (Byte)i;
                    isKeyPressed = true;
                }
            }
            if (isKeyPressed)
                m_programCounter += 2;
        }

        break;
        case 0x0005:
            switch (opcode & 0x00F0) {
            case 0x0010:
                // FX15
                // Sets the delay timer to VX

                X = (opcode & 0x0F00) >> 8;
                m_delayTimer = m_registers[X];
                m_programCounter += 2;

                break;
            case 0x0050:
                // FX55
                // Stores V0 to VX (including VX) in memory starting at
                // address I. The offset from I is increased by 1 for each
                // value written, but I itself is left unmodified

                X = (opcode & 0x0F00) >> 8;
                for (int i = 0; i <= (int)X; i++) {
                    m_memory[m_indexRegister + i] = m_registers[i];
                }
                m_programCounter += 2;

                break;
            case 0x0060:
                // FX65
                // Fills V0 to VX (including VX) with values from memory
                // starting at address I. The offset from I is increased by
                // 1 for each value written, but I itself is left unmodified

                X = (opcode & 0x0F00) >> 8;
                for (int i = 0; i <= (int)X; i++) {
                    m_registers[i] = m_memory[m_indexRegister + i];
                }
                m_programCounter += 2;

                break;
            default:
                std::cerr << "Illegal opcode encountered! " << std::hex
                          << (int)opcode << std::dec << "\n";
                exit(ILLEGAL_OPCODE_ERR);
                break;
            }
            break;
        case 0x0008:
            // FX18
            // Sets the sound timer to VX

            X = (opcode & 0x0F00) >> 8;
            m_soundTimer = m_registers[X];
            m_programCounter += 2;

            break;
        case 0x000E:
            // FX1E
            // Adds VX to I. VF is set to 1 when there is a range overflow
            // (I+VX>0xFFF), and to 0 when there isn't

            X = (opcode & 0x0F00) >> 8;
            m_registers[0x000F] = 0;
            if (m_indexRegister + m_registers[X] > 0x0FFF)
                m_registers[0x000F] = 1;
            m_indexRegister =
                (unsigned short)(m_indexRegister + m_registers[X]);
            m_programCounter += 2;

            break;
        case 0x0009:
            // FX29
            // Sets I to the location of the sprite for the character in VX.
            // Characters 0-F (in hexadecimal) are represented by a 4x5 font

            X = (opcode & 0x0F00) >> 8;
            // fonts are stored in memory from 0x0000 - 0x0080
            // each character is stored in 5 bytes
            m_indexRegister = m_registers[X] * 5;
            m_programCounter += 2;

            break;
        case 0x0003: {
            // FX33
            // Stores the binary-coded decimal representation of VX, with the
            // most significant of three digits at the address in I, the middle
            // digit at I plus 1, and the least significant digit at I plus 2.
            // (In other words, take the decimal representation of VX, place the
            // hundreds digit in memory at location in I, the tens digit at
            // location I+1, and the ones digit at location I+2.)

            X = (opcode & 0x0F00) >> 8;
            int VX = m_registers[X];
            m_memory[m_indexRegister + 2] = VX % 10;
            VX /= 10;
            m_memory[m_indexRegister + 1] = VX % 10;
            VX /= 10;
            m_memory[m_indexRegister] = VX % 10;
            m_programCounter += 2;
        }

        break;
        default:
            std::cerr << "Illegal opcode encountered! " << std::hex
                      << (int)opcode << std::dec << "\n";
            exit(ILLEGAL_OPCODE_ERR);
            break;
        }
        break;
    default:
        std::cerr << "Illegal opcode encountered! " << std::hex << (int)opcode
                  << std::dec << "\n";
        exit(ILLEGAL_OPCODE_ERR);
        break;
    }

    if (m_delayTimer > 0)
        m_delayTimer--;

    if (m_soundTimer > 0) {
        m_soundTimer--;
    }
}
