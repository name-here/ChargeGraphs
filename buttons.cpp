#include <cstdio>
#include "buttons.h"


ToggleButton::ToggleButton(){
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	pixels = nullptr;
}
ToggleButton::ToggleButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32*& setPixels ){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
	pixels = setPixels;
}

void ToggleButton::draw( unsigned int mouseX, unsigned int mouseY, bool mousePressed, unsigned int windowWidth ){
	if( mouseX >= x && mouseX <= x+width  &&  mouseY >= y && mouseY <= y+height ){
		if( mousePressed ){
			held = true;
			usedColor = pressedColor;
		}
		else{
			if( held ){
				isPressed = !isPressed;
			}
		if( isPressed ){ usedColor = onColor; }
		else{ usedColor = offColor; }
		}
	}
	else{
		if( isPressed ){ usedColor = onColor; }
		else{ usedColor = offColor; }
	}
	if( held && !mousePressed ){
		held = false;
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


PushButton::PushButton(){
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	pixels = nullptr;
}
PushButton::PushButton( unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32*& setPixels ){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
	pixels = setPixels;
}

void PushButton::draw( unsigned int mouseX, unsigned int mouseY, bool mousePressed, unsigned int windowWidth ){
	if( mouseX >= x && mouseX <= x+width  &&  mouseY >= y && mouseY <= y+height ){
		released = false;
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
				released = true;
			}
			usedColor = hoverColor;
		}
	}
	else{
		usedColor = offColor;
	}
	if( held && !mousePressed ){
		held = false;
	}
	for( unsigned int setX = x; setX < x + width; setX ++ ){
		pixels[ setX ] = borderColor;
		pixels[ windowWidth + setX ] = borderColor;
		pixels[ ( windowWidth * (height - 1) ) + setX ] = borderColor;
		pixels[ ( windowWidth * (height - 2) ) + setX ] = borderColor;
	}
	for( unsigned int setY = y + 2; setY < y + height - 2; setY ++ ){
		pixels[ ( windowWidth * setY ) ] = borderColor;
		pixels[ ( windowWidth * setY ) + 1 ] = borderColor;
		pixels[ ( windowWidth * setY ) + width-1 ] = borderColor;
		pixels[ ( windowWidth * setY ) + width-2 ] = borderColor;
	}
	for( unsigned int setY = y + 2; setY < y + height - 2; setY ++ ){
		for( unsigned int setX = x + 2; setX < x + width - 2; setX ++ ){
			pixels[ ( windowWidth * setY ) + setX ] = usedColor;
		}
	}
}


