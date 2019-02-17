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
				pressed = !pressed;
			}
		if( pressed ){ usedColor = onColor; }
		else{ usedColor = offColor; }
		}
	}
	else{
		if( pressed ){ usedColor = onColor; }
		else{ usedColor = offColor; }
	}
	if( held && !mousePressed ){
		held = false;
	}
	for( unsigned int setY = y; setY < y+height; setY ++ ){
		for( unsigned int setX = x; setX<x+width; setX ++ ){
			pixels[ (windowWidth * setY) + setX] = usedColor;
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
	for( unsigned int setY = y; setY < y+height; setY ++ ){
		for( unsigned int setX = x; setX<x+width; setX ++ ){
			pixels[ (windowWidth * setY) + setX] = usedColor;
		}
	}
}


