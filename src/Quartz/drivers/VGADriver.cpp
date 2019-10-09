#include "VGADriver.h"

VGADriver::VGADriver()
	:
	misc_port(0x3c2),
	crtc_index_port(0x3d4), crtc_data_port(0x3d5),
	sequencer_index_port(0x3c4), sequencer_data_port(0x3c5),
	graphics_controller_index_port(0x3ce), graphics_controller_data_port(0x3cf),
	attribute_controller_index_port(0x3c0), attribute_controller_read_port(0x3c1),
	attribute_controller_write_port(0x3c0), attribute_controller_reset_port(0x3da)
{
}

VGADriver::~VGADriver()
{
}

bool VGADriver::isModeSupported(VideoMode mode)
{
	return mode.width == 320 && mode.height == 200;
}

bool VGADriver::setMode(VideoMode mode)
{
	if (!isModeSupported(mode))
	{
		return false;
	}

	m_current_video_mode = mode;

	unsigned char g_320x200x256[] =
	{
		/* MISC */
		0x63,
		/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x0E,
		/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
		0xFF,
		/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
		0xFF,
		/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x41, 0x00, 0x0F, 0x00,	0x00
	};

	writeRegisters(g_320x200x256);
	return true;
}

void VGADriver::putPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
	putPixel(x, y, RGBToColorIndex(r, g, b));
}

void VGADriver::putPixel(uint32_t x, uint32_t y, uint8_t color_index)
{
	if (m_current_video_mode.width == 0)
	{
		qlib::printf("VideoMode not set!");
		return;
	}

	uint8_t* pixel_address = getFrameBufferSegment() + m_current_video_mode.width * y + x;
	*pixel_address = color_index;
}

uint8_t VGADriver::RGBToColorIndex(uint8_t r, uint8_t g, uint8_t b)
{
	if (r == 0x00 && g == 0x00 && b == 0x00) return 0x00; // black
	if (r == 0x00 && g == 0x00 && b == 0xA8) return 0x01; // blue
	if (r == 0x00 && g == 0xA8 && b == 0x00) return 0x02; // green
	if (r == 0xA8 && g == 0x00 && b == 0x00) return 0x04; // red
	if (r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F; // white
	return 0x00;
}

uint8_t* VGADriver::getFrameBufferSegment()
{
	graphics_controller_index_port.write(0x06);
	uint8_t segmentNumber = graphics_controller_data_port.read() & (3 << 2);

	switch (segmentNumber)
	{
	default:
	case 0 << 2: return (uint8_t*)0x00000;
	case 1 << 2: return (uint8_t*)0xA0000;
	case 2 << 2: return (uint8_t*)0xB0000;
	case 3 << 2: return (uint8_t*)0xB8000;
	}
}

void VGADriver::writeRegisters(uint8_t* registers)
{
	//  misc
	misc_port.write(*(registers++));

	// sequencer
	for (uint8_t i = 0; i < 5; i++)
	{
		sequencer_index_port.write(i);
		sequencer_data_port.write(*(registers++));
	}

	// cathode ray tube controller
	crtc_index_port.write(0x03);
	crtc_data_port.write(crtc_data_port.read() | 0x80);
	crtc_index_port.write(0x11);
	crtc_data_port.write(crtc_data_port.read() & ~0x80);

	registers[0x03] = registers[0x03] | 0x80;
	registers[0x11] = registers[0x11] & ~0x80;

	for (uint8_t i = 0; i < 25; i++)
	{
		crtc_index_port.write(i);
		crtc_data_port.write(*(registers++));
	}

	// graphics controller
	for (uint8_t i = 0; i < 9; i++)
	{
		graphics_controller_index_port.write(i);
		graphics_controller_data_port.write(*(registers++));
	}

	// attribute controller
	for (uint8_t i = 0; i < 21; i++)
	{
		attribute_controller_reset_port.read();
		attribute_controller_index_port.write(i);
		attribute_controller_write_port.write(*(registers++));
	}

	attribute_controller_reset_port.read();
	attribute_controller_index_port.write(0x20);
}
