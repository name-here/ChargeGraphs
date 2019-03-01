#include "colors.h"

Color::Color(){
	r = 0;
	g = 0;
	b = 0;
}

void Color::setHue(int hue){
	if( hue <= 255 ){
		r = 255;
		g = hue;
		b = 0;
	}
	else if( hue <= 255*2 ){
		r = 256*2 - 1 - hue;
		g = 255;
		b = 0;
	}
	else if( hue <= 255*3 ){
		r = 0;
		g = 255;
		b = hue - 255*2;
	}
	else if( hue <= 255*4 ){
		r = 0;
		g = 255*4 - hue;
		b = 255;
	}
	else if( hue <= 255*5 ){
		r = hue - 255*4;
		g = 0;
		b = 255;
	}
	else{
		r = 255;
		g = 0;
		b = 255*6 - hue;
	}
}