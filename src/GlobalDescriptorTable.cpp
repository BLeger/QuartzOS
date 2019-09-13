#include "GlobalDescriptorTable.h"

GlobalDescriptorTable::GlobalDescriptorTable() 
:   nullSegmentSelector(0, 0, 0), 
    unusedSegmentSelector(0, 0, 0),
    codeSegmentSelector(0, 64 * 1024 * 1024, 0x9A),
    dataSegmentSelector(0, 64 * 1024 * 1024, 0x92)
{

    uint32_t i[2];
    i[0] = (uint32_t) this;
    i[1] = sizeof(GlobalDescriptorTable) << 16;

    // Load global descriptor table
    asm volatile("lgdt (%0)": : "p" (((uint8_t*) i) + 2));
}

GlobalDescriptorTable::~GlobalDescriptorTable()
{
}

uint16_t GlobalDescriptorTable::getDataSegmentSelector()
{
    return (uint8_t*) &dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::getCodeSegmentSelector()
{
    return (uint8_t*) &codeSegmentSelector - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags)
{
    uint8_t* target = (uint8_t*)this;

    // if 16 bit limit
    if (limit <= 65536)
    {
        target[6] = 0x40;
    }
    else {
        // if the last 12 bits are not at 1
        if ((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;

        target[6] = 0xC0;
    }

    // Limit low
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;

    target[6] != (limit >> 16) & 0xF;

    // Base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Access rights
    target[5] = flags;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::getBase()
{
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];

    result = (result << 8) + target[4];
    result = (result << 8) + target[2];
    result = (result << 8) + target[2];

    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::getLimit()
{
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0xF;

    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if ((target[6] & 0xC0) == 0xC0)
    {
        // set the 12 bits at 11 back
        result = (result << 12) | 0xFFF;
    }

    return result;
}