#include "Port.h"

Port::Port(uint16_t portNumber)
: m_portNumber(portNumber)
{}

Port::~Port()
{}

/* 8 Bit port */
Port8Bit::Port8Bit(uint16_t portNumber)
: Port(portNumber)
{}

Port8Bit::~Port8Bit()
{}

void Port8Bit::write(uint8_t data)
{
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (m_portNumber));
}

uint8_t Port8Bit::read()
{
    uint8_t data;

    __asm__ volatile("inb %1, %0" : "=a" (data) : "Nd" (m_portNumber) );

    return data;
}

/* Slow 8 Bit port */
Port8BitSlow::Port8BitSlow(uint16_t portNumber)
: Port8Bit(portNumber)
{}

Port8BitSlow::~Port8BitSlow()
{}

void Port8BitSlow::write(uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (m_portNumber));
}

/* 16 Bit Port */
Port16Bit::Port16Bit(uint16_t portNumber)
: Port(portNumber)
{}

Port16Bit::~Port16Bit()
{}

void Port16Bit::write(uint16_t data)
{
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (m_portNumber));
}

uint16_t Port16Bit::read()
{
    uint16_t data;

    __asm__ volatile("inw %1, %0" : "=a" (data) : "Nd" (m_portNumber) );

    return data;
}

/* 32 Bit Port */
Port32Bit::Port32Bit(uint16_t portNumber)
: Port(portNumber)
{}

Port32Bit::~Port32Bit()
{}

void Port32Bit::write(uint32_t data)
{
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (m_portNumber));
}

uint32_t Port32Bit::read()
{
    uint32_t data;

    __asm__ volatile("inl %1, %0" : "=a" (data) : "Nd" (m_portNumber) );

    return data;
}