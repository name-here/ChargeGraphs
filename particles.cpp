#include <SDL2/SDL.h>//for Uint32
#include <cmath>
#include <cstdio>
#include "colors.h"
#include "particles.h"


void set( Uint32*& pixels, unsigned int windowWidth, unsigned int windowHeight, unsigned int x, unsigned int y, Color color );
inline double square( double num ){return num * num;}
inline double cube( double num ){return num * num * num;}
inline double dist2DSq( double x1, double y1, double x2, double y2 ){
	return square( x2 - x1 ) + square( y2 - y1 );
}
inline double dist2D( double x1, double y1, double x2, double y2 ){
	return sqrt(  dist2DSq( x1, y1, x2, y2 )  );
}


ParticleSystem::ParticleSystem(){

}

void ParticleSystem::draw( Uint32*& pixels, unsigned int windowWidth, unsigned int windowHeight, unsigned int displayMode, unsigned int colorMode ){
	unsigned int shortDim;
	if( windowWidth <= windowHeight){ shortDim = windowWidth; }
	else{ shortDim = windowHeight; }
	double x, y;
	double normX, normY;
	unsigned int setX, setY;
	double potential;
	double intermediate;
	double fieldX = 0, fieldY = 0;

	Color color;

	if( displayMode == 0 ){
		memset(pixels, 255, ( sizeof(Uint32) ) * windowWidth * windowHeight);
	}
	else{
		if( displayMode == 1 ){
			memset(pixels, 127, ( sizeof(Uint32) ) * windowWidth * windowHeight);
		}

		for( unsigned int screenX = windowWidth-1; screenX < windowWidth; screenX -- ){
			for( unsigned int screenY = windowHeight-1; screenY < windowWidth; screenY -- ){
				for(unsigned int subX = 0; subX < detail; subX ++ ){
					for( unsigned int subY = 0; subY < detail; subY ++ ){
						x = ( screenX*1.0 + subX*1.0 / detail)  /  (windowWidth - 1);
						y = ( screenY*1.0 + subY*1.0 / detail)  /  (windowHeight - 1);

						normX = x*windowWidth/shortDim;
						normY = y*windowHeight/shortDim;

						if( displayMode == 1 ){
							potential = getPotential( normX, normY );
							setX = (unsigned int) (  ( (x * windowWidth) + ((1 - y) * windowHeight) ) * (windowWidth - 1) / (windowWidth + windowHeight)  );
							setY = (unsigned int) (  windowHeight * (0.375 + x/8 + y/8 - potential/100)  );
							if( setY < windowHeight  &&  setY > 0  &&  pixels[ (setY * windowWidth) + setX ] == 0x7f7f7f7f ){
								color = getColor( potential, colorMode );
								pixels[ (setY * windowWidth) + setX ]  =  ( 255 << 24 ) + ( color.r << 16 )  + ( color.g << 8 )  +  color.b;
							}
						}
						else if( displayMode == 2 ){
							if( subX == 0  &&  subY == 0 ){
								set( pixels, windowWidth, windowHeight, screenX, screenY, getColor( getPotential( normX, normY ), colorMode ) );
							}
						}
						else if( displayMode == 3 ){
							Color color;
							fieldX = 0; fieldY = 0;
							for( auto i = particles.begin(); i!=particles.end(); ++i ){
								intermediate = 1 / cube( dist2D(normX, normY, (*i).x, (*i).y) );
								fieldX += intermediate * ( (*i).x - normX );
								fieldY += intermediate * ( (*i).y - normY );
							}
							double colorX = fieldX + 128;
							if( colorX < 0 ){ colorX = 0; }
							if( colorX > 255 ){colorX = 255;}
							double colorY = fieldY + 128;
							if( colorY < 0 ){ colorY = 0; }
							if( colorY > 255 ){colorY = 255;}
							color.r = 0;
							color.g = (int)colorX;
							color.b = (int)colorY;
							set( pixels, windowWidth, windowHeight, screenX, screenY, color );
						}
					}
				}
			}
		}
	}
}

void ParticleSystem::drawDots( Uint32*& pixels, unsigned int windowWidth, unsigned int windowHeight ){
	unsigned int shortDim;
	if( windowWidth <= windowHeight){ shortDim = windowWidth; }
	else{ shortDim = windowHeight; }
	Uint32 color;
	Uint32 fill;
	Uint32 border;
	for( auto i = particles.begin(); i!=particles.end(); ++i ){
		if( (*i).charge >= 0 ){
			fill = 0xff00ff00;
			if( (*i).properties & 0b00010000 ){
				border = 0xff004400;
			}
			else{
				border = 0xff008800;
			}
		}
		else{
			fill = 0xffff0000;
			if( (*i).properties & 0b00010000 ){
				border = 0xff440000;
			}
			else{
				border = 0xff880000;
			}
		}
		for( int x = -(int)floor( abs( (*i).charge * shortDim / 50 ) ); x <= (int)floor( abs( (*i).charge * shortDim / 50 ) ); x ++ ){
			for( int y = -(int)floor( abs( (*i).charge * shortDim / 50 ) ); y <= (int)floor( abs( (*i).charge * shortDim / 50 ) ); y ++ ){
				if( x*x + y*y < square((*i).charge * shortDim / 50) ){
					unsigned int setX = (int)( x  +  (*i).x * shortDim );
					unsigned int setY = (int)( y  +  (*i).y * shortDim );
					if( setX < windowWidth  &&  setY < windowHeight ){
						if( x*x + y*y < square( abs((*i).charge) * shortDim / 50 - shortDim/100 ) ){
							if( !((*i).properties & 0b10000000)  &&  x*x + y*y < square( shortDim / 150 ) ){
								color = 0xff000000;
							}
							else{ color = fill; }
						}
						else{ color = border; }
						pixels[ ( setY * windowWidth) + setX ] = color;
					}
				}
			}
		}
	}
}


void ParticleSystem::simulate(){
	double intermediate;
	double fieldX = 0, fieldY = 0;
	for( auto i = particles.begin(); i!=particles.end(); ++i ){
		if( (*i).properties & 0b10000000  &&  !( (*i).properties & 0b00001000 ) ){
			fieldX = 0; fieldY = 0;
			for( auto iSub = particles.begin(); iSub!=particles.end(); ++iSub ){
				if( (*iSub).id != (*i).id  &&  (*iSub).properties & 0b01000000 ){
					intermediate = (-1 * (*iSub).charge) / cube( dist2D((*i).x, (*i).y, (*iSub).x, (*iSub).y) );
					fieldX += intermediate * ( (*iSub).x - (*i).x );
					fieldY += intermediate * ( (*iSub).y - (*i).y );
				}
			}
			(*i).velX += (*i).charge * fieldX / 10000;
			(*i).velY += (*i).charge * fieldY / 10000;
			(*i).x += (*i).velX;
			(*i).y += (*i).velY;
		}
	}
}

Color ParticleSystem::getColor( double potential, int colorMode ){
	Color color;
	int shade = ((int)( potential * 128 ) + 256)*6;
	if( colorMode == 0 ){
		shade /= 10;
		if( shade>255 ) { shade = 255; }
		if( shade<0 ) { shade = 0; }
	}
	else if( colorMode == 1){
		shade = (int)(potential*255) % 256;
		shade = shade > -4 && shade < 4;
		shade *= 255;
		int intermediate = abs(  ( (int)( potential * 128 / 2 ) + 256 )  /  255  *  50  );
		if(intermediate<0){intermediate = 0;}
		if(intermediate + shade>255){intermediate = shade - 255;}
		shade += intermediate;
	}
	else if( colorMode == 2 ){
		shade %= 255*6;
		if( shade < 0 ){ shade += 255*6; }
		color.setHue( shade );
		
	}
	if( colorMode < 2 ){
		color.r = shade;
		color.g = shade;
		color.b = shade;
	}
	return color;
}

inline double ParticleSystem::getPotential( double x, double y ){
	double potential = 0;
	for( auto i = particles.begin(); i!=particles.end(); ++i ){
		if( (*i).properties & 0b01000000 ){
			potential += (*i).charge / dist2D( x, y, (*i).x, (*i).y );
		}
	}
	return potential;
}

void ParticleSystem::addParticle( double setCharge, double setParticleX, double setParticleY, double setParticleVelX, double setParticleVelY, char setProperties ){
	Particle particle;
	particle.charge = setCharge;
	particle.x = setParticleX;// * windowWidth / shortDim;
	particle.y = setParticleY;// * windowHeight / shortDim;
	particle.velX = setParticleVelX;
	particle.velY = setParticleVelY;
	particle.properties = setProperties;
	particle.id = particles.size();
	particles.push_back( particle );
}


void set( Uint32*& pixels, unsigned int windowWidth, unsigned int windowHeight, unsigned int x, unsigned int y, Color color ){
	if( x < windowWidth  &&  y < windowHeight  &&  (y*windowWidth)+x < windowWidth*windowHeight){
		pixels[ (y * windowWidth) + x ]  =  ( 255 << 24 ) + ( color.r << 16 )  + ( color.g << 8 )  +  color.b;
	}
	else{ printf( "Tried to draw pixel out of bounds at (%i, %i)\n", x, y ); }
}


