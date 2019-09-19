#ifndef QZ_INTERRUPT_H
#define QZ_INTERRUPT_H

#include <qlib/assert.h>
#include <qlib/io.h>
#include <qlib/memory.h>
#include <qlib/types.h>

#include "Quartz/Port.h"
#include "Quartz/GlobalDescriptorTable.h"
#include "InterruptHandler.h"
#include "interrupt_list.h"

/*
 * TODO: If in the future there is no need for multiple managers
		transform the class into a singleton => no activate / deactivate
		just Create() and eventually Destroy()
 */
class InterruptManager
{
protected:

	static InterruptManager* activeInterruptManager;

	struct GateDescriptor
	{
		uint16_t handler_address_low;
		uint16_t gdt_code_segment_selector;
		uint8_t reserved;
		uint8_t access_rights;
		uint16_t handler_address_high;

	} __attribute__((packed));

	static GateDescriptor interrupt_descriptor_table[256];

	struct InterruptDescriptorTablePointer
	{
		uint16_t size;
		uint32_t base;
	} __attribute__((packed));


	static void setInterruptDescriptorTableEntry(
		uint8_t interrupt_number,
		uint16_t gdt_code_segment_selector,
		void (*handler)(),
		uint8_t descriptor_privilege_level,
		uint8_t descriptor_type
	);

	Port8BitSlow pic_master_command;
	Port8BitSlow pic_master_data;
	Port8BitSlow pic_slave_command;
	Port8BitSlow pic_slave_data;

	InterruptHandler* handlers[256];

public:

	InterruptManager(GlobalDescriptorTable* gdt);
	~InterruptManager();

	static InterruptManager* Get() 
	{
		ASSERT(activeInterruptManager != NULL)
		return activeInterruptManager;
	};

	void activate();
	void deactivate();

	void addHandler(InterruptHandler* handler);

    static uint32_t handleInterrupt(uint8_t interrupt, uint32_t stack_ptr);
    uint32_t doHandleInterrupt(uint8_t interrupt, uint32_t stack_ptr);

	// Defined in interrupts.s
	static void ignoreInterruptRequest();
	static void handleInterruptRequest0x00();
	static void handleInterruptRequest0x01();
	static void handleInterruptRequest0x0C();

	static void handleException0x13();
};

#endif