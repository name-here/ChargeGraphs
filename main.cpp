#include <SDL2/SDL.h>
#include <cstdint>
#include <cmath>
#include <cstdio>
#include <vector>
#include "buttons.h"

struct Color{ uint8_t r; uint8_t g; uint8_t b; };

Color getColor( double potential, int colorMode );
Color hueRGB(uint8_t hue);
void set( unsigned int x, unsigned int y, Color color );
inline double square( double num ){return num * num;}
inline double cube( double num ){return num * num * num;}
inline double dist2DSq( double x1, double y1, double x2, double y2 ){
	return square( x2 - x1 ) + square( y2 - y1 );
}
inline double dist2D( double x1, double y1, double x2, double y2 ){
	return sqrt(  dist2DSq( x1, y1, x2, y2 )  );
}
void addParticle( double setCharge, double setParticleX, double setParticleY, double setParticleVelX, double setParticleVelY, char setProperties );//make inline?

unsigned int windowWidth;//600
unsigned int windowHeight;//420
unsigned int shortDim;
Uint32* pixels;
unsigned int frameCount = 0;

unsigned int mouseX = windowWidth/2;
unsigned int mouseY = windowHeight/2;
double sclMouseX;
double sclMouseY;
bool mousePressed;

unsigned int detail = 1;
double x, y;
double normX, normY;
unsigned int setX, setY;

double potential;
double intermediate;
double xPull = 0, yPull = 0;

int displayMode = 1;
int colorMode = 2;
int systemNum = 2;
int shade;

std::vector<double> particleCharge;
std::vector<double> particleX;//position of particles
std::vector<double> particleY;
std::vector<double> particleVelX;//velocity of particles
std::vector<double> particleVelY;
std::vector<char> particleProperties;
//particleProperties is a series of switches for properties of particle, in order:
//feels force | exerts force | unused | unused | unused | unused | unused | unused

PushButton button1;
ToggleButton button2;

void setup(){
	button1 = PushButton(10, 10, 50, 30, pixels);
	button2 = ToggleButton(70, 10, 50, 30, pixels);
	button2.pressed = true;

	addParticle( 1, 0.25, 0.5, 0, 0, 0b11000000 );
	addParticle( -1, 0.75, 0.5, 0, 0, 0b11000000 );
}


void loop(){
	sclMouseX = mouseX*1.0/shortDim;
	sclMouseY = mouseY*1.0/shortDim;

	//if( displayMode == 1 ){
		memset(pixels, 127, ( sizeof(Uint32) ) * windowWidth * windowHeight);
	//}

	for( unsigned int screenX = windowWidth-1; screenX < windowWidth; screenX -- ){
		for( unsigned int screenY = windowHeight-1; screenY < windowWidth; screenY -- ){
			for(unsigned int subX = 0; subX < detail; subX ++ ){
				for( unsigned int subY = 0; subY < detail; subY ++ ){
					x = ( screenX*1.0 + subX*1.0 / detail)  /  (windowWidth - 1);
					y = ( screenY*1.0 + subY*1.0 / detail)  /  (windowHeight - 1);

					normX = x*windowWidth/shortDim;
					normY = y*windowHeight/shortDim;

					if( systemNum == 0 ){
						potential = -0.5 / dist2D( normX, normY, 0.5, 0.5 )  +  1 / dist2D( normX, normY, sclMouseX, sclMouseY ) - 0.25 / dist2D( normX, normY, 0, 1 );
						//shade = (int)( potential * 128 ) + 256;
					}
					else if( systemNum == 1 ){
						potential = 4 / dist2D( normX, normY, 0.25, 0.5 ) - 1 / dist2D( normX, normY, 0.75, 0.5 );
						//shade = (int)( potential * 128 ) + 256; + (sclMouseY - 0.5)*20*255;
					}
					else if( systemNum == 2 ){
						potential = (1 - 2*sclMouseY) / dist2D( normX, normY, 0.25, 0.75 ) + 1 / dist2D( normX, normY, 0.75, 0.25 );
					}

					if( displayMode == 0 ){
						if( pixels[ (screenY * windowWidth) + screenX ] == 0x7f7f7f7f ){
							set( screenX, screenY, getColor( potential, colorMode ) );
						}
					}
					else if( displayMode == 1 ){
						setX = (unsigned int) (  ( (x * windowWidth) + ((1 - y) * windowHeight) ) * (windowWidth - 1) / (windowWidth + windowHeight)  );
						setY = (unsigned int) (  windowHeight * (0.375 + x/8 + y/8) - potential*4  );
						if( setY < windowHeight  &&  setY > 0  &&  pixels[ (setY * windowWidth) + setX ] == 0x7f7f7f7f ){
							set( setX, setY, getColor( potential, colorMode ) );
						}
					}
					else if( displayMode == 2 ){
						Color color;
						intermediate = 1 / cube( dist2D(normX, normY, sclMouseX, sclMouseY) );
						xPull = intermediate * (sclMouseX - normX);
						yPull = intermediate * (sclMouseY - normY);
						color.r = 0;
						color.g = abs((int)(yPull)%255);
						color.b = abs((int)(xPull)%255);
						set( screenX, screenY, color );
					}
				}
			}
		}
	}
	/*if( (frameCount % 30) == 0 ){
		printf("xPull: %f\n", xPull);
	}*/
	button1.draw(mouseX, mouseY, mousePressed, windowWidth);
	button2.draw(mouseX, mouseY, mousePressed, windowWidth);
	if( button1.released ){
		systemNum ++;
		if( systemNum > 2 ){ systemNum = 0; }
	}
	if( button2.pressed ){
		displayMode = 2;
		colorMode = 2;
	}
	else{
		displayMode = 0;
		colorMode = 0;
	}

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
							mousePressed = true;
					}
					else if( event.type == SDL_MOUSEBUTTONUP ){
							mousePressed = false;
					}
					else if( event.type == SDL_WINDOWEVENT ){
						if( event.window.event == SDL_WINDOWEVENT_RESIZED ){
							windowWidth = event.window.data1;
							windowHeight = event.window.data2;
							//SDL_GetWindowSize(window, &windowWidth, &windowHeight);
							if( windowWidth <= windowHeight){ shortDim = windowWidth; }
							else{ shortDim = windowHeight; }
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


Color getColor( double potential, int colorMode ){
	Color color;
	shade = (int)( potential * 128 ) + 256;
	if( colorMode == 0 ){
		shade /= 5;
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
		shade %= 255;
		if( shade < 0 ){ shade += 255; }
		color = hueRGB(shade);
		
	}
	if( colorMode < 2 ){
		color.r = shade;
		color.g = shade;
		color.b = shade;
	}
	return color;
}

Color hueRGB(uint8_t hue){
	Color color;
	if( hue <= 255/6 ){
		color.r = 255;
		color.g = hue * 6;
		color.b = 0;
	}
	else if( hue <= 255*2/6 ){
		color.r = ( 255*2/6 - hue )*6;
		color.g = 255;
		color.b = 0;
	}
	else if( hue <= 255*3/6 ){
		color.r = 0;
		color.g = 255;
		color.b = ( hue - 255*2/6 )*6;
	}
	else if( hue <= 255*4/6 ){
		color.r = 0;
		color.g = ( 255*4/6 - hue )*6;
		color.b = 255;
	}
	else if( hue <= 255*5/6 ){
		color.r = ( hue - 255*4/6 )*6;
		color.g = 0;
		color.b = 255;
	}
	else{
		color.r = 255;
		color.g = 0;
		color.b = ( 255 - hue )*6;
	}
	return color;
}

void addParticle( double setCharge, double setParticleX, double setParticleY, double setParticleVelX, double setParticleVelY, char setProperties ){
	particleCharge.push_back( setCharge );
	particleX.push_back( setParticleX );
	particleY.push_back( setParticleY );
	particleVelX.push_back( setParticleVelX );
	particleVelY.push_back( setParticleVelY );
	particleProperties.push_back( setProperties );
}

void set( unsigned int x, unsigned int y, Color color ){
	if( x < windowWidth  &&  y < windowHeight  &&  (y*windowWidth)+x < windowWidth*windowHeight){
		pixels[ (y*windowWidth)+x ] = ( 255 << 24 ) + ( color.r << 16 )  + ( color.g << 8 )  +  color.b;
	}
	else{ printf( "Tried to draw pixel out of bounds at (%i, %i)\n", x, y ); }
}


