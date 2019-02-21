#include <SDL2/SDL.h>
//#include <cstdint>
//#include <cmath>
//#include <cstdio>
#include "colors.h"
#include "particles.h"
#include "buttons.h"


void set( unsigned int x, unsigned int y, Color color );
void circle( double posX, double posY, double radius, double borderWidth, Uint32 fill, Uint32 border );
inline double square( double num ){return num * num;}
/*inline double cube( double num ){return num * num * num;}
inline double dist2DSq( double x1, double y1, double x2, double y2 ){
	return square( x2 - x1 ) + square( y2 - y1 );
}
inline double dist2D( double x1, double y1, double x2, double y2 ){
	return sqrt(  dist2DSq( x1, y1, x2, y2 )  );
}*/

unsigned int windowWidth;//600
unsigned int windowHeight;//420
unsigned int shortDim;
Uint32* pixels;
unsigned int frameCount = 0;
Uint32 lastTime;
Uint32 frameRate;

unsigned int mouseX = windowWidth/2;
unsigned int mouseY = windowHeight/2;
double sclMouseX;
double sclMouseY;
bool mouseIsPressed;

int displayMode = 1;
int colorMode = 2;

ParticleSystem partSys;

bool moving;
bool selected;
bool deSelect = false;
double offsetX, offsetY;


ToggleButton button1;//Changes positive/negative charges placed on click
ToggleButton pause;
PushButton button2;
ToggleButton chargeButton;//Toggles 2D/3D

void setup(){
	button1 = ToggleButton(windowWidth - 60, windowHeight - 40, 50, 30);
	pause = ToggleButton( windowWidth/2 - 25, 10, 50, 30 );
	pause.offColor = 0xffffffff;
	pause.onColor = 0xffffffff;
	pause.pressedColor = 0xffcccccc;
	button2 = PushButton(10, windowHeight - 40, 50, 30 );
	chargeButton = ToggleButton(10, 10, 50, 30);
	chargeButton.offColor = 0xff00ff00;
	chargeButton.onColor = 0xffff0000;
	chargeButton.pressedColor = 0xff888800;

	partSys.addParticle( 1, 0.25 * windowWidth / shortDim, 0.25 * windowHeight / shortDim, 0, 0, 0b01000000 );
	partSys.addParticle( 1, 0.75 * windowWidth / shortDim, 0.25 * windowHeight / shortDim, 0, 0, 0b01000000 );
	partSys.addParticle( 1, 0.25 * windowWidth / shortDim, 0.75 * windowHeight / shortDim, 0, 0, 0b01000000 );
	partSys.addParticle( 1, 0.75 * windowWidth / shortDim, 0.75 * windowHeight / shortDim, 0, 0, 0b01000000 );
	partSys.addParticle( 2, 0.4 * windowWidth / shortDim, 0.5 * windowHeight / shortDim, 0, 0, 0b11000000 );
}


void loop(){
	sclMouseX = mouseX*1.0/shortDim;
	sclMouseY = mouseY*1.0/shortDim;

	//partSys.particles.back().x = sclMouseX;
	//partSys.particles.back().y = sclMouseY;

	if( displayMode == 0 ){
		if( moving ){
			partSys.particles.back().x = sclMouseX + offsetX;
			partSys.particles.back().y = sclMouseY + offsetY;
		}
	}
	if( !pause.isPressed ){
		partSys.simulate();
	}

	partSys.draw(pixels, windowWidth, windowHeight, displayMode, colorMode);

	if( displayMode == 0 ){
		for( unsigned int setX = windowWidth - shortDim*4/50 - 11; setX < windowWidth - 10; setX ++ ){
			pixels[ ( windowWidth * 10 ) + setX ] = 0xff000000;
			pixels[ ( windowWidth * 11 ) + setX ] = 0xff000000;
			pixels[ ( windowWidth * (9 + shortDim*7/50) ) + setX ] = 0xff000000;
			pixels[ ( windowWidth * (10 + shortDim*7/50) ) + setX ] = 0xff000000;
		}
		for( unsigned int setY = 11; setY < shortDim*7/50 + 11; setY ++ ){
			pixels[ ( windowWidth * setY ) + windowWidth - shortDim*2/25 - 11 ] = 0xff000000;
			pixels[ ( windowWidth * setY ) + windowWidth - shortDim*2/25 - 10 ] = 0xff000000;
			pixels[ ( windowWidth * setY ) + windowWidth - 11 ] = 0xff000000;
			pixels[ ( windowWidth * setY ) + windowWidth - 12 ] = 0xff000000;
		}
		circle( windowWidth - shortDim/25.0 - 11, shortDim/25.0 + 10, shortDim/50.0, shortDim/100, 0xff00ff00, 0xff004400);
		circle( windowWidth - shortDim/25.0 - 11, shortDim/10.0 + 10, shortDim/50.0, shortDim/100, 0xffff0000, 0xff440000);

		if( selected ){
			chargeButton.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
			if( chargeButton.justPressed ){ deSelect = false; }
			if( chargeButton.isPressed ){
				partSys.particles.back().charge = -1 * abs(partSys.particles.back().charge);
			}
			else{
				partSys.particles.back().charge = abs(partSys.particles.back().charge);
			}
		}
	}

	button1.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
	pause.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
	if( pause.isPressed ){
		for( unsigned int y = 15; y < 35; y ++ ){
			for( unsigned int x = windowWidth/2 - 10; x < windowWidth/2 + 10; x ++ ){
				if( (unsigned int)abs( (int)y - 25 ) < 10 - (x - windowWidth/2 + 10)/2 ){
					pixels[ (y * windowWidth) + x ] = 0xff000000;
				}
			}
		}
	}
	else{
		for( int num = 0; num < 2; num ++ ){
			for( unsigned int y = 15; y < 35; y ++ ){
				for( unsigned int x = windowWidth/2 - 10; x < windowWidth/2 - 3; x ++ ){
					pixels[ (y * windowWidth) + x ] = 0xff000000;
				}
				for( unsigned int x = windowWidth/2 +3; x < windowWidth/2 + 10; x ++ ){
					pixels[ (y * windowWidth) + x ] = 0xff000000;
				}
			}
		}
	}
	button2.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
	if( button1.isPressed ){
		displayMode = 0;
		if( !moving ){
			for( auto i = partSys.particles.begin(); i != partSys.particles.end(); ++i ){
				if( (*i).x < 0 || (*i).x > (windowWidth*1.0 / shortDim)  ||  (*i).y < 0 || (*i).y > (windowHeight*1.0 / shortDim) ){
					if( (*i).properties & 0b00010000 ){
						selected = false;
						moving = false;
					}
					partSys.particles.erase( i );
					--i;
				}
			}
		}
	}
	else{
		displayMode = 1;
		colorMode = 2;
	}

	if( displayMode == 0 ){
		partSys.drawDots(pixels, windowWidth, windowHeight);
	}
}

void mousePressed(){
	if( displayMode == 0 && !moving/*sanity check (unnecessary?)*/ ){
		deSelect = true;
		for( auto i = partSys.particles.begin(); i != partSys.particles.end(); ++i ){
			if(  square( sclMouseX - (*i).x ) + square( sclMouseY - (*i).y ) < square( (*i).charge / 50 )  ){
				moving = true;
				selected = true;
				deSelect = false;
				partSys.particles.back().properties &= 0b11100111;
				(*i).properties |= 0b00011000;
				offsetX = (*i).x - sclMouseX;
				offsetY = (*i).y - sclMouseY;
				chargeButton.isPressed = ( (*i).charge < 0 );
				Particle swap = partSys.particles.back();
				partSys.particles.back() = *i;
				*i = swap;
				break;
			}
		}
		if( !moving  &&  mouseX >= windowWidth - shortDim*3/50 - 11  &&  mouseX <= windowWidth - shortDim/50 - 11  &&
		                 mouseY >= shortDim/50 + 10  &&  mouseY <= shortDim*3/25 +10 ){
			deSelect = false;
			if( mouseY <= shortDim*3/50 + 10 ){
				if(  square( mouseX - windowWidth + shortDim/25 + 11 ) + square( mouseY - shortDim/25 - 10 ) < square( shortDim / 50 ) || true  ){
					partSys.particles.back().properties &= 0b11100111;
					partSys.addParticle( 1, windowWidth/2, windowHeight, 0, 0, 0b11011000 );
					chargeButton.isPressed = false;
					moving = true;
					selected = true;
					offsetX = (-(double)mouseX - shortDim/25.0 + windowWidth - 11) / shortDim;
					offsetY = (-(double)mouseY + shortDim/25.0 + 10) / shortDim;
				}
			}
			else if( mouseY >= shortDim*2/25 + 10){//  &&
			        //square( mouseX - windowWidth + shortDim/25 + 11 ) + square( mouseY - shortDim/10 - 10 ) < square( shortDim / 50 ) ){
				partSys.particles.back().properties &= 0b11100111;
				partSys.addParticle( -1, windowWidth/2, windowHeight, 0, 0, 0b11011000 );
				chargeButton.isPressed = true;
				moving = true;
				selected = true;
				offsetX = (-(double)mouseX - shortDim/25.0 + windowWidth - 11) / shortDim;
				offsetY = (-(double)mouseY + shortDim/25.0 + 10) / shortDim;
			}
		}
	}
}

void mouseReleased(){
	moving = false;
	partSys.particles.back().properties &= 0b11110111;
	if( deSelect ){
		deSelect = false;
		selected = false;
		partSys.particles.back().properties &= 0b11101111;
	}
}

void windowResized(){
	button1.x = windowWidth - 60;
	button1.y = windowHeight - 40;
	pause.x = windowWidth/2 - 25;
	button2.y = windowHeight - 40;
}


int main(){
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_DisplayMode DM;
	SDL_Texture* buffer = nullptr;
	bool quit = false;
    SDL_Event event;
	if(SDL_Init( SDL_INIT_VIDEO ) < 0){
		printf( "SDL could not initialize.  SDL_Error: %s\n", SDL_GetError() );
	}
	else{
		SDL_GetCurrentDisplayMode( 0, &DM );
		windowWidth = 250;//DM.w*3/4;
		windowHeight = 250;//DM.h*3/4;
		if( windowWidth <= windowHeight){ shortDim = windowWidth; }
		else{ shortDim = windowHeight; }
		pixels = new Uint32[ windowWidth * windowHeight ];

		window = SDL_CreateWindow( "Charge Graphs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE );
		//last parameter above once ended with "SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI" instead of current value.  For resizable, should be SDL_WINDOW_RESIZABLE.
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );

		}
		else{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC );
			buffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight );
			//SDL_SetWindowFullscreen(window,  SDL_WINDOW_FULLSCREEN_DESKTOP);
			setup();
			while(!quit){
				if( SDL_GetTicks() != lastTime  ){
				frameRate = 1000 / ( SDL_GetTicks() - lastTime );
				}
				lastTime = SDL_GetTicks();
				if( button2.held ){
					printf("Frame Rate:%i\n", frameRate);
				}
				loop();
				SDL_UpdateTexture( buffer, NULL, pixels, windowWidth*sizeof(Uint32) );
				SDL_RenderClear( renderer );
				SDL_RenderCopy( renderer, buffer, NULL, NULL );
				SDL_RenderPresent( renderer );

				frameCount ++;
				while( SDL_PollEvent( &event ) != 0 ){
					if( event.type == SDL_QUIT ){
						quit = true;
						break;
					}
					else if( event.type == SDL_MOUSEMOTION ){
						mouseX = event.motion.x;   // = event.motion.x-(windowWidth/2)
						mouseY = event.motion.y;  // = -event.motion.y+(windowHeight/2)
					}
					else if( event.type == SDL_MOUSEBUTTONDOWN ){
						mouseIsPressed = true;
						mousePressed();
					}
					else if( event.type == SDL_MOUSEBUTTONUP ){
						mouseIsPressed = false;
						mouseReleased();
					}
					else if( event.type == SDL_WINDOWEVENT ){
						if( event.window.event == SDL_WINDOWEVENT_RESIZED ){
							windowWidth = event.window.data1;
							windowHeight = event.window.data2;
							if( windowWidth <= windowHeight){ shortDim = windowWidth; }
							else{ shortDim = windowHeight; }
							//SDL_GetWindowSize(window, &windowWidth, &windowHeight);
							delete pixels;
							pixels = new Uint32[ windowWidth * windowHeight ];
							SDL_DestroyTexture( buffer );
							buffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight );
							windowResized();
						}
					}
				}
			}
		}
	}
	SDL_DestroyTexture( buffer );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}

void circle( double posX, double posY, double radius, double borderWidth, Uint32 fill, Uint32 border ){
	Uint32 color;
	for( int x = -(int)radius; x <= (int)radius; x ++ ){
		for( int y = -(int)radius; y <= (int)radius; y ++ ){
			if( x+posX >= 0 && x+posX < windowWidth  &&  y+posY >= 0 && y+posY < windowHeight   &&   x*x + y*y < radius*radius ){
				if( x*x + y*y < square( radius - borderWidth ) ){ color = fill; }
				else{ color = border; }
				pixels[ ( (int)floor(y+posY) * windowWidth) + (int)floor(x+posX) ] = color;
			}
		}
	}
}


void set( unsigned int x, unsigned int y, Color color ){
	if( x < windowWidth  &&  y < windowHeight  &&  (y*windowWidth)+x < windowWidth*windowHeight){
		pixels[ (y*windowWidth)+x ] = ( 255 << 24 ) + ( color.r << 16 )  + ( color.g << 8 )  +  color.b;
	}
	else{ printf( "Tried to draw pixel out of bounds at (%i, %i)\n", x, y ); }
}


