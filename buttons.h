#include <SDL2/SDL.h>


class ToggleButton{
public:
	unsigned int x, y, width, height;
	bool pressed = false;//button is active if (toggled on)
	bool held = false;//button is being pressed by mouse right now (mouse pressed and on button)
	Uint32* pixels;
	unsigned int windowWidth;

	Uint32 offColor = 0xffaa22ff;//These store colors (Uint32 used for convient color setting)
	Uint32 onColor = 0x996614ff;
	Uint32 pressedColor = 0x66440dff;
	Uint32 borderColor = 0x000000ff;
	Uint32 usedColor;
	ToggleButton();
	ToggleButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32*& setPixels, unsigned int setWindowWidth );
	void draw( unsigned int mouseX, unsigned int mouseY, bool mousePressed );
};

class PushButton{
public:
	unsigned int x, y, width, height;
	bool pressed = false;//button was just pressed this frame
	bool held = false;//button is being pressed by mouse right now (mouse pressed and on button)
	bool released = false;//button was just released this frame
	Uint32* pixels;
	unsigned int windowWidth;

	Uint32 offColor = 0xaaff22ff;//These store colors (Uint32 used for convient color setting)
	Uint32 hoverColor = 0x88cc1bff;//Probably should be removed (maybe only used for non-toggle buttons)
	Uint32 pressedColor = 0x44660dff;
	Uint32 borderColor = 0x000000ff;
	Uint32 usedColor;
	PushButton();
	PushButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32*& setPixels, unsigned int setWindowWidth );
	void draw( unsigned int mouseX, unsigned int mouseY, bool mousePressed );
};


