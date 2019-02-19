#include <SDL2/SDL.h>
//#include <cstdint>
//#include <cmath>
//#include <cstdio>
#include "colors.h"
#include "particles.h"
#include "buttons.h"


void set( unsigned int x, unsigned int y, Color color );
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
double offsetX, offsetY;


ToggleButton button1;//Changes positive/negative charges placed on click
ToggleButton button2;//Toggles 2D/3D

void setup(){
	button1 = ToggleButton(windowWidth - 60, 10, 50, 30);
	button2 = ToggleButton(10, 10, 50, 30);
	button2.offColor = 0xff00ff00;
	button2.onColor = 0xffff0000;
	button2.pressedColor = 0xff888800;

	partSys.addParticle( 1, 0.25, 0.25, 0, 0, 0b01000000 );
	partSys.addParticle( 1, 0.75, 0.25, 0, 0, 0b01000000 );
	partSys.addParticle( 1, 0.25, 0.75, 0, 0, 0b01000000 );
	partSys.addParticle( 1, 0.75, 0.75, 0, 0, 0b01000000 );
	partSys.addParticle( -1, 0.5, 0.5, 0, 0, 0b11000000 );
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
	else{
		partSys.simulate();
	}

	partSys.draw(pixels, windowWidth, windowHeight, displayMode, colorMode);

	button1.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed );
	if( button1.isPressed ){
		displayMode = 0;
		int toDelete = 0;
		for( auto i = partSys.particles.begin(); i != partSys.particles.end(); ++i ){
			if( (*i).x < 0 || (*i).x > (windowWidth*1.0 / shortDim)  ||  (*i).y < 0 || (*i).y > (windowHeight*1.0 / shortDim) ){
				//partSys.particles.erase( partSys.particles.begin() );
			}
			toDelete ++;
		}
	}
	else{
		displayMode = 1;
		colorMode = 2;
	}
	if( displayMode == 0 ){
		button2.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed );
		if( selected ){
			if( button2.isPressed ){
				partSys.particles.back().charge = -1 * abs(partSys.particles.back().charge);
			}
			else{
				partSys.particles.back().charge = abs(partSys.particles.back().charge);
			}
		}
	}

}

void mousePressed(){
	if( displayMode == 0 ){
		if( !moving ){
			for( auto i = partSys.particles.begin(); i != partSys.particles.end(); ++i ){
				if(  square( sclMouseX - (*i).x ) + square( sclMouseY - (*i).y ) < square( (*i).charge / 50 )  ){
					printf("ran");
					moving = true;
					selected = true;
					offsetX = (*i).x - sclMouseX;
					offsetY = (*i).y - sclMouseY;
					button2.isPressed = ( (*i).charge < 0 );
					Particle swap = partSys.particles.back();
					partSys.particles.back() = *i;
					*i = swap;
					break;
				}
			}
		}
	}
	else{
		selected = false;
	}
}

void mouseReleased(){
	moving = false;
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
		windowWidth = DM.w*3/4;
		windowHeight = DM.h*3/4;
		if( windowWidth <= windowHeight){ shortDim = windowWidth; }
		else{ shortDim = windowHeight; }
		pixels = new Uint32[ windowWidth * windowHeight ];

		window = SDL_CreateWindow( "Charge Graphs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE );//used to end with "SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI" instead of "0".  For resizable, should be SDL_WINDOW_RESIZABLE.
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );

		}
		else{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC );
			buffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight );
			//SDL_SetWindowFullscreen(window,  SDL_WINDOW_FULLSCREEN_DESKTOP);
			setup();
			while(!quit){
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


void set( unsigned int x, unsigned int y, Color color ){
	if( x < windowWidth  &&  y < windowHeight  &&  (y*windowWidth)+x < windowWidth*windowHeight){
		pixels[ (y*windowWidth)+x ] = ( 255 << 24 ) + ( color.r << 16 )  + ( color.g << 8 )  +  color.b;
	}
	else{ printf( "Tried to draw pixel out of bounds at (%i, %i)\n", x, y ); }
}


