#include "InterruptManager.h"

#include "../lib/io.h"

InterruptManager::GateDescriptor InterruptManager::interrupt_descriptor_table[256];
InterruptManager* InterruptManager::activeInterruptManager = NULL;

void InterruptManager::setInterruptDescriptorTableEntry(uint8_t interrupt_number, uint16_t gdt_code_segment_selector, void(*handler)(), uint8_t descriptor_privilege_level, uint8_t descriptor_type)
{
	const uint8_t IDT_DESC_PRESENT = 0x80;

	interrupt_descriptor_table[interrupt_number].handler_address_low = ((uint32_t)handler) & 0xFFFF;
	interrupt_descriptor_table[interrupt_number].handler_address_high = (((uint32_t)handler) >> 16) & 0xFFFF;
	interrupt_descriptor_table[interrupt_number].gdt_code_segment_selector = gdt_code_segment_selector;
	interrupt_descriptor_table[interrupt_number].access_rights = IDT_DESC_PRESENT | descriptor_type | ((descriptor_privilege_level & 3) << 5);
	interrupt_descriptor_table[interrupt_number].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
	: pic_master_command(0x20), pic_master_data(0x21),
	  pic_slave_command(0xA0), pic_slave_data(0xA1)
{
	uint16_t code_segment = gdt->getCodeSegmentSelector();
	constexpr uint8_t IDT_INTERRUPT_GATE = 0xE;

	// Set all interrupts to ignore
	for (int i = 0; i < 256; i++)
	{
		handlers[i] = NULL;
		setInterruptDescriptorTableEntry(i, code_segment, &ignoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
	}

	//setInterruptDescriptorTableEntry(0x13, code_segment, &handleException0x13, 0, IDT_INTERRUPT_GATE);

	setInterruptDescriptorTableEntry(TIMER_INTERRUPT, code_segment, &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	setInterruptDescriptorTableEntry(KEYBOARD_INTERRUPT, code_segment, &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

	pic_master_command.write(0x11);
	pic_slave_command.write(0x11);

	pic_master_data.write(0x20);
	pic_slave_data.write(0x28);

	pic_master_data.write(0x04);
	pic_slave_data.write(0x02);

	pic_master_data.write(0x01);
	pic_slave_data.write(0x01);

	pic_master_data.write(0x00);
	pic_slave_data.write(0x00);

	InterruptDescriptorTablePointer idt;
	idt.size = 256 * sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t)interrupt_descriptor_table;

	// Load the interrupt descriptor table
	asm volatile("lidt %0" :  : "m" (idt));
}

InterruptManager::~InterruptManager()
{
}

void InterruptManager::activate()
{
	// If another InterruptManager is active, deactivate it before activating the new one
	if (activeInterruptManager != NULL) {
		activeInterruptManager->deactivate();
	}

	activeInterruptManager = this;
	// Start interrupts
	__asm("sti");
}

void InterruptManager::deactivate()
{
	if (activeInterruptManager != this)
		return;

	activeInterruptManager = NULL;
	__asm("cli");
}

void InterruptManager::addHandler(InterruptHandler* handler)
{
	if (handler == NULL)
		return;

	handlers[handler->getInterruptNumber()] = handler;
}

uint32_t InterruptManager::handleInterrupt(uint8_t interrupt, uint32_t stack_ptr)
{
	// Get the active InterruptManager
	auto* manager = InterruptManager::Get();
	if (manager == NULL)
		return 0;

	return manager->doHandleInterrupt(interrupt, stack_ptr);
}

uint32_t InterruptManager::doHandleInterrupt(uint8_t interrupt, uint32_t stack_ptr)
{
	if (handlers[interrupt] != NULL) {
		stack_ptr = handlers[interrupt]->handleInterrupt(stack_ptr);
	}
	else {
		if (interrupt != TIMER_INTERRUPT)
			qlib::printf("Interrupt has not handler");
	}

	// Only hardware interrupts require an answer
	if (0x20 <= interrupt && interrupt < 0x30)
	{
		pic_master_command.write(0x20);
		// if the interrupt came from slave pic
		if (0x28 <= interrupt)
		{
			pic_slave_command.write(0x20);
		}
	}

	return stack_ptr;
}
