#include "../includes/chip.hpp"

// used to initialize and reset the system
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