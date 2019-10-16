#ifndef QZ_VGA_DRIVER_H
#define QZ_VGA_DRIVER_H

#include "Quartz/kprintf.h"
#include "Driver.h"
#include "Quartz/core/Port.h"

struct VideoMode
{
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t color_depth = 0;
};

class VGADriver : public Driver
{
public:
	VGADriver();
	~VGADriver();

	virtual void activate() {};
	virtual int reset() {
		return 0;
	};
	virtual void deactivate() {};

	bool isModeSupported(VideoMode mode);
	bool setMode(VideoMode mode);

	void putPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
	void putPixel(uint32_t x, uint32_t y, uint8_t color_index);
private:
	VideoMode m_current_video_mode;

	Port8Bit misc_port;
	Port8Bit crtc_index_port;
	Port8Bit crtc_data_port;
	Port8Bit sequencer_index_port;
	Port8Bit sequencer_data_port;
	Port8Bit graphics_controller_index_port;
	Port8Bit graphics_controller_data_port;
	Port8Bit attribute_controller_index_port;
	Port8Bit attribute_controller_read_port;
	Port8Bit attribute_controller_write_port;
	Port8Bit attribute_controller_reset_port;

	uint8_t RGBToColorIndex(uint8_t r, uint8_t g, uint8_t b);
	uint8_t* getFrameBufferSegment();
	void writeRegisters(uint8_t* registers);


};

#endif // !QZ_VGA_DRIVER_H
