#include <SDL2/SDL.h>


class Button{
	public:
		unsigned int x, y, width, height;
		bool held = false;//button is being pressed by mouse right now (mouse pressed and on button)

		Uint32 offColor;//These store colors (Uint32 used for convient color setting)
		Uint32 pressedColor;
		Uint32 borderColor;
		Uint32 usedColor;

		virtual ~Button() = default;
		virtual void draw( Uint32*& pixels, unsigned int windowWidth, unsigned int mouseX, unsigned int mouseY, bool mousePressed, bool active ) = 0;
		virtual void moveResize( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight );
};


class ToggleButton: public Button{
public:
	bool justPressed = false;//button was just pressed last frame
	bool isPressed = false;//button is active (toggled on)

	Uint32 onColor;// = 0xff996614;//These store colors (Uint32 used for convient color setting)
	/*Uint32 offColor = 0xffffaa22;
	Uint32 pressedColor = 0xff66440d;
	Uint32 borderColor = 0xff000000;
	Uint32 usedColor;*/

	ToggleButton(){};
	ToggleButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight );
	ToggleButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32 setOnColor, Uint32 setOffColor, Uint32 setPressedColor, Uint32 setBorderColor );
	void draw( Uint32*& pixels, unsigned int windowWidth, unsigned int mouseX, unsigned int mouseY, bool mousePressed, bool active );
};

class PushButton: public Button{
public:
	bool pressed = false;//button was just pressed this frame
	bool held = false;//button is being pressed by mouse right now (mouse pressed and on button)
	bool released = false;//button was just released this frame

	//Uint32 offColor = 0xffaaff22;//These store colors (Uint32 used for convient color setting)
	Uint32 hoverColor;// = 0xff88cc1b;
	/*Uint32 pressedColor = 0xff44660d;
	Uint32 borderColor = 0xff000000;
	Uint32 usedColor;*/

	PushButton(){};
	PushButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight );
	PushButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32 setOffColor, Uint32 setHoverColor, Uint32 setPressedColor, Uint32 setBorderColor );
	void draw( Uint32*& pixels, unsigned int windowWidth, unsigned int mouseX, unsigned int mouseY, bool mousePressed, bool active );
};


