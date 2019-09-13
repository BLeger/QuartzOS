#ifndef QZ_GLOBAL_DESCRIPTOR_TABLE_H
#define QZ_GLOBAL_DESCRIPTOR_TABLE_H

#include "types.h"

class GlobalDescriptorTable 
{
    public:
    
        class SegmentDescriptor
        {
        private:
            uint16_t limit_low;
            uint16_t base_low;
            uint8_t base_high;
            uint8_t type;
            uint8_t flags_limit_high;
            uint8_t base_vhi;

        public:
            SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags);
            uint32_t getBase();
            uint32_t getLimit(); 
        } __attribute((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        uint16_t getCodeSegmentSelector();
        uint16_t getDataSegmentSelector();

};

#endif