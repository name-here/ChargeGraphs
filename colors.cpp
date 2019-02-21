#include "colors.h"

Color::Color(){
	r = 0;
	g = 0;
	b = 0;
}

void Color::setHue(uint8_t hue){
	if( hue <= 255/6 ){
		r = 255;
		g = hue * 6;
		b = 0;
	}
	else if( hue <= 255*2/6 ){
		r = ( 255*2/6 - hue )*6;
		g = 255;
		b = 0;
	}
	else if( hue <= 255*3/6 ){
		r = 0;
		g = 255;
		b = ( hue - 255*2/6 )*6;
	}
	else if( hue <= 255*4/6 ){
		r = 0;
		g = ( 255*4/6 - hue )*6;
		b = 255;
	}
	else if( hue <= 255*5/6 ){
		r = ( hue - 255*4/6 )*6;
		g = 0;
		b = 255;
	}
	else{
		r = 255;
		g = 0;
		b = ( 255 - hue )*6;
	}
}