#include <cstdint>

struct Color{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	Color();
	void setHue(int hue);
};