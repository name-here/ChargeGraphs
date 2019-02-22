#include <cstdio>
#include "buttons.h"


void Button::moveResize( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight ){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
}


ToggleButton::ToggleButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight ){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
	onColor = 0xff996614;
	offColor = 0xffffaa22;
	pressedColor = 0xff66440d;
	borderColor = 0xff000000;
}
ToggleButton::ToggleButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32 setOnColor, Uint32 setOffColor, Uint32 setPressedColor, Uint32 setBorderColor ){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
	onColor = setOnColor;
	offColor = setOffColor;
	pressedColor = setPressedColor;
	borderColor = setBorderColor;
}

void ToggleButton::draw( Uint32*& pixels, unsigned int windowWidth, unsigned int mouseX, unsigned int mouseY, bool mousePressed, bool active ){
	justSet = false;
	if( active   &&   mouseX >= x && mouseX <= x+width  &&  mouseY >= y && mouseY <= y+height ){
		if( mousePressed ){
			if( held ){
				justPressed = false;
			}
			else{
				held = true;
				justPressed = true;
			}
			usedColor = pressedColor;
		}
		else{
			if( held ){
				held = false;
				justSet = true;
				isPressed = !isPressed;
			}
		if( isPressed ){ usedColor = onColor; }
		else{ usedColor = offColor; }
		}
	}
	else{
		held = false;
		if( isPressed ){ usedColor = onColor; }
		else{ usedColor = offColor; }
	}
	for( unsigned int setX = x; setX < x + width; setX ++ ){
		pixels[ ( windowWidth * y ) + setX ] = borderColor;
		pixels[ ( windowWidth * (y + 1) ) + setX ] = borderColor;
		pixels[ ( windowWidth * (y + height - 1) ) + setX ] = borderColor;
		pixels[ ( windowWidth * (y + height - 2) ) + setX ] = borderColor;
	}
	for( unsigned int setY = y + 2; setY < y + height - 2; setY ++ ){
		pixels[ ( windowWidth * setY ) + x ] = borderColor;
		pixels[ ( windowWidth * setY ) + x + 1 ] = borderColor;
		pixels[ ( windowWidth * setY ) + x + width - 1 ] = borderColor;
		pixels[ ( windowWidth * setY ) + x + width - 2 ] = borderColor;
	}
	for( unsigned int setY = y + 2; setY < y + height - 2; setY ++ ){
		for( unsigned int setX = x + 2; setX < x + width - 2; setX ++ ){
			pixels[ ( windowWidth * setY ) + setX ] = usedColor;
		}
	}
}


PushButton::PushButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight ){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
	offColor = 0xffaaff22;
	hoverColor = 0xff88cc1b;
	pressedColor = 0xff44660d;
	borderColor = 0xff000000;
}
PushButton::PushButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32 setOffColor, Uint32 setHoverColor, Uint32 setPressedColor, Uint32 setBorderColor ){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
	offColor = setOffColor;
	hoverColor = setHoverColor;
	pressedColor = setPressedColor;
	borderColor = setBorderColor;
}

void PushButton::draw( Uint32*& pixels, unsigned int windowWidth, unsigned int mouseX, unsigned int mouseY, bool mousePressed, bool active ){
	released = false;
	if( active   &&   mouseX >= x && mouseX <= x+width  &&  mouseY >= y && mouseY <= y+height ){
		if( mousePressed ){
			if( held ){ pressed = false; }
			else{
				held = true;
				pressed = true;
			}
			usedColor = pressedColor;
		}
		else{
			if( held ){
				held = false;
				pressed = false;
				released = true;
			}
			usedColor = hoverColor;
		}
	}
	else{
		held = false;
		usedColor = offColor;
	}
	for( unsigned int setX = x; setX < x + width; setX ++ ){
		pixels[ ( windowWidth * y ) + setX ] = borderColor;
		pixels[ ( windowWidth * (y + 1) ) + setX ] = borderColor;
		pixels[ ( windowWidth * (y + height - 1) ) + setX ] = borderColor;
		pixels[ ( windowWidth * (y + height - 2) ) + setX ] = borderColor;
	}
	for( unsigned int setY = y + 2; setY < y + height - 2; setY ++ ){
		pixels[ ( windowWidth * setY ) + x ] = borderColor;
		pixels[ ( windowWidth * setY ) + x + 1 ] = borderColor;
		pixels[ ( windowWidth * setY ) + x + width - 1 ] = borderColor;
		pixels[ ( windowWidth * setY ) + x + width - 2 ] = borderColor;
	}
	for( unsigned int setY = y + 2; setY < y + height - 2; setY ++ ){
		for( unsigned int setX = x + 2; setX < x + width - 2; setX ++ ){
			pixels[ ( windowWidth * setY ) + setX ] = usedColor;
		}
	}
}


