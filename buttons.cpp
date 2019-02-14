#include <cstdio>
#include "buttons.h"


ToggleButton::ToggleButton(){
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	pixels = nullptr;
	windowWidth = 0;
}
ToggleButton::ToggleButton(unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32*& setPixels, unsigned int setWindowWidth){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
	pixels = setPixels;
	windowWidth = setWindowWidth;
}

void ToggleButton::draw( unsigned int mouseX, unsigned int mouseY, bool mousePressed ){
	if( mouseX >= x && mouseX <= x+width  &&  mouseY >= y && mouseY <= y+height ){
		if( mousePressed ){
			held = true;
			usedColor = pressedColor;
		}
		else{
			if( held ){
				held = false;
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
	windowWidth = 0;
}
PushButton::PushButton(unsigned int setX, unsigned int setY, unsigned int setWidth, unsigned int setHeight, Uint32*& setPixels, unsigned int setWindowWidth){
	x = setX;
	y = setY;
	width = setWidth;
	height = setHeight;
	pixels = setPixels;
	windowWidth = setWindowWidth;
}

void PushButton::draw( unsigned int mouseX, unsigned int mouseY, bool mousePressed ){
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
				held = false;
				released = true;
			}
			usedColor = hoverColor;
		}
	}
	else{
		usedColor = offColor;
	}
	for( unsigned int setY = y; setY < y+height; setY ++ ){
		for( unsigned int setX = x; setX<x+width; setX ++ ){
			pixels[ (windowWidth * setY) + setX] = usedColor;
		}
	}
}


