#include <SDL2/SDL.h>


class ToggleButton{
public:
	unsigned int x, y, width, height;
	bool justPressed = false;//button was just pressed last frame
	bool isPressed = false;//button is active (toggled on)
	bool held = false;//button is being pressed by mouse right now (mouse pressed and on button)

	Uint32 offColor = 0xffffaa22;//These store colors (Uint32 used for convient color setting)
	Uint32 onColor = 0xff996614;
	Uint32 pressedColor = 0xff66440d;
	Uint32 borderColor = 0xff000000;
	Uint32 usedColor;
	ToggleButton();
	ToggleButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight );
	void draw( Uint32*& pixels, unsigned int windowWidth, unsigned int mouseX, unsigned int mouseY, bool mousePressed, bool active );
};

class PushButton{
public:
	unsigned int x, y, width, height;
	bool pressed = false;//button was just pressed this frame
	bool held = false;//button is being pressed by mouse right now (mouse pressed and on button)
	bool released = false;//button was just released this frame

	Uint32 offColor = 0xffaaff22;//These store colors (Uint32 used for convient color setting)
	Uint32 hoverColor = 0xff88cc1b;//Probably should be removed (maybe only used for non-toggle buttons)
	Uint32 pressedColor = 0xff44660d;
	Uint32 borderColor = 0xff000000;
	Uint32 usedColor;
	PushButton();
	PushButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight );
	void draw( Uint32*& pixels, unsigned int windowWidth, unsigned int mouseX, unsigned int mouseY, bool mousePressed, bool active );
};


