#include <SDL2/SDL.h>
//#include <cstdint>
//#include <cmath>
//#include <cstdio>
#include "colors.h"
#include "particles.h"
#include "buttons.h"


void set( unsigned int x, unsigned int y, Color color );
void circle( double posX, double posY, double radius, double borderWidth, double dotSize, Uint32 fill, Uint32 border, Uint32 dotColor );
void line( int x1, int y1, int x2, int y2, Uint32 color );
inline double square( double num ){return num * num;}
inline double cube( double num ){return num * num * num;}
inline double dist2DSq( double x1, double y1, double x2, double y2 ){
	return square( x2 - x1 ) + square( y2 - y1 );
}
inline double dist2D( double x1, double y1, double x2, double y2 ){
	return sqrt(  dist2DSq( x1, y1, x2, y2 )  );
}

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

unsigned int displayMode = 0;
unsigned int colorMode = 2;
unsigned int startSystem = 2;
unsigned int gridNum = 10;

ParticleSystem partSys;

bool moving;
bool selected;
bool deSelect = false;
double offsetX, offsetY;


ToggleButton viewButton;//Toggles 2D/3D
ToggleButton pauseButton;//Toggles pausing simulation
PushButton resetButton;//Resets when released
ToggleButton showVecButton;//Toggles showing vectors for field and particles
ToggleButton chargeButton;//Toggles +/- charge on selected particle
ToggleButton lockedButton;//Toggles whether selected particle is locked in place
ToggleButton addLockedButton;
//ToggleButton exertsForce;//Toggles whether selected particle exerts forces on others

void setupParticles(){
	if( startSystem == 1 ){
		partSys.addParticle( 1, 0.25 * windowWidth / shortDim, 0.25 * windowHeight / shortDim, 0, 0, 0b01000000 );
		partSys.addParticle( 1, 0.75 * windowWidth / shortDim, 0.25 * windowHeight / shortDim, 0, 0, 0b01000000 );
		partSys.addParticle( 1, 0.25 * windowWidth / shortDim, 0.75 * windowHeight / shortDim, 0, 0, 0b01000000 );
		partSys.addParticle( 1, 0.75 * windowWidth / shortDim, 0.75 * windowHeight / shortDim, 0, 0, 0b01000000 );
		partSys.addParticle( 2, 0.4 * windowWidth / shortDim, 0.5 * windowHeight / shortDim, 0, 0, 0b11000000 );
	}
	else if( startSystem == 2 ){
		partSys.addParticle( 1, 0.25 * windowWidth / shortDim, 0.5 * windowHeight/shortDim, 0, 0, 0b01000000 );
		partSys.addParticle( 1, 0.75 * windowWidth / shortDim, 0.5 * windowHeight/shortDim, 0, 0, 0b01000000 );
		partSys.addParticle( -1, 0.5 * windowWidth / shortDim, 0.5 * windowHeight/shortDim, 0.005 * windowWidth / shortDim, 0.01 * windowHeight / shortDim, 0b11000000 );
	}
	else if( startSystem == 3){
		for( int i = 0; i < 100; i ++){
			partSys.addParticle( 1, 0.25 * windowWidth / shortDim, 0.5 * windowHeight/shortDim, 0, 0, 0b01000000 );
		}
	}
}

void setup(){
	viewButton = ToggleButton( windowWidth*34/40, windowHeight*137/150, windowWidth/8, windowHeight/15, 0xff0077ff, 0xff00ff77, 0xff007777, 0xff000000 );
	viewButton.isPressed = true;
	pauseButton = ToggleButton( windowWidth*9/20, windowHeight/50, windowWidth/10, windowHeight/15, 0xffffffff, 0xffffffff, 0xffcccccc, 0xff000000 );
	resetButton = PushButton( windowWidth/40, windowHeight*137/150, windowWidth/8, windowHeight/15, 0xff990000, 0xffff0000, 0xff000000, 0xff000000 );
	showVecButton = ToggleButton( windowWidth*9/20, windowHeight*137/150, windowWidth/10, windowHeight/15, 0xff2a7d9b, 0xff44ccff, 0xff38a7ce, 0xff000000 );
	addLockedButton = ToggleButton( windowWidth*39/40 - shortDim/15, windowHeight/25 + shortDim*7/50, shortDim/15, shortDim/15, 0xff555555, 0xffdddddd, 0xffaaaaaa, 0xff000000 );
	chargeButton = ToggleButton( windowWidth/40, windowHeight/50, windowWidth*3/32, windowHeight/15, 0xffff0000, 0xff00ff00, 0xff888800, 0xff000000 );
	lockedButton = ToggleButton( windowWidth/40, windowHeight*16/150, shortDim/15, shortDim/15, 0xff555555, 0xffdddddd, 0xffaaaaaa, 0xff000000 );
	//exertsForce = ToggleButton( windowWidth/40, windowHeight*16/150, shortDim/15, shortDim/15, 0xff555555, 0xffdddddd, 0xffaaaaaa, 0xff000000 );

	setupParticles();
}


void loop(){
	sclMouseX = mouseX*1.0/shortDim;
	sclMouseY = mouseY*1.0/shortDim;

	if( !pauseButton.isPressed ){
		partSys.simulate();
	}

	if( displayMode == 0 ){
		if( moving ){
			partSys.particles.back().x = sclMouseX + offsetX;
			partSys.particles.back().y = sclMouseY + offsetY;
		}
	}

	partSys.draw(pixels, windowWidth, windowHeight, displayMode, colorMode);

	pauseButton.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
	if( pauseButton.isPressed ){
		for( unsigned int y = windowHeight*3/100; y < windowHeight*23/300; y ++ ){
			for( unsigned int x = windowWidth/2 - windowHeight*3/100; x < windowWidth/2 + windowHeight*3/100 - abs((int)y - (int)windowHeight*16/300)*18/7; x ++ ){

					pixels[ (y * windowWidth) + x ] = 0xff000000;
			}
		}
	}
	else{
		for( unsigned int y = windowHeight*3/100; y < windowHeight*23/300; y ++ ){
			for( unsigned int x = windowWidth/2 - windowHeight*5/200; x < windowWidth/2 - windowHeight/100; x ++ ){
				pixels[ (y * windowWidth) + x ] = 0xff000000;
			}
			for( unsigned int x = windowWidth/2 + windowHeight/100; x < windowWidth/2 + windowHeight*5/200; x ++ ){
				pixels[ (y * windowWidth) + x ] = 0xff000000;
			}
		}
	}

	if( displayMode == 0 ){
		for( unsigned int setX = windowWidth*39/40 - shortDim*2/25; setX < windowWidth*39/40; setX ++ ){
			pixels[ ( windowWidth * (windowHeight/50) ) + setX ] = 0xff000000;
			pixels[ ( windowWidth * (windowHeight/50 + 1) ) + setX ] = 0xff000000;
			pixels[ ( windowWidth * (windowHeight/50 + shortDim*7/50 - 1) ) + setX ] = 0xff000000;
			pixels[ ( windowWidth * (windowHeight/50 + shortDim*7/50) ) + setX ] = 0xff000000;
		}
		for( unsigned int setY = windowHeight/50; setY < windowHeight/50 + shortDim*7/50; setY ++ ){
			pixels[ ( windowWidth * setY ) + windowWidth*39/40 - shortDim*2/25 - 1 ] = 0xff000000;
			pixels[ ( windowWidth * setY ) + windowWidth*39/40 - shortDim*2/25 ] = 0xff000000;
			pixels[ ( windowWidth * setY ) + windowWidth*39/40 ] = 0xff000000;
			pixels[ ( windowWidth * setY ) + windowWidth*39/40 - 1 ] = 0xff000000;
		}
		circle( windowWidth - shortDim/25.0 - windowWidth/40, shortDim/25.0 + windowHeight/50, shortDim/50.0, shortDim/100, addLockedButton.isPressed * shortDim/150, 0xff00ff00, 0xff004400, 0xff000000 );
		circle( windowWidth - shortDim/25.0 - windowWidth/40, shortDim/10.0 + windowHeight/50, shortDim/50.0, shortDim/100, addLockedButton.isPressed * shortDim/150, 0xffff0000, 0xff440000, 0xff000000 );

		showVecButton.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );

		addLockedButton.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
		//exertsForce.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );

		if( selected ){
			chargeButton.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
			lockedButton.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
			if( chargeButton.justSet ){
				if( chargeButton.isPressed ){
					partSys.particles.back().charge = -1 * abs(partSys.particles.back().charge);
				}
				else{
					partSys.particles.back().charge = abs(partSys.particles.back().charge);
				}
			}

			if( lockedButton.justSet ){
				if( lockedButton.isPressed ){
					partSys.particles.back().properties &= 0b01111111;
					partSys.particles.back().velX = 0;
					partSys.particles.back().velY = 0;
				}
				else{
					partSys.particles.back().properties |= 0b10000000;
				}
			}

			/*if( exertsForce.justSet ){
				if( exertsForce.isPressed ){
					partSys.particles.back().properties |= 0b01000000;
				}
				else{
					partSys.particles.back().properties &= 0b10111111;
				}
			}*/

			if( chargeButton.justPressed || pauseButton.justPressed || showVecButton.justPressed || lockedButton.justPressed || addLockedButton.justPressed /*|| exertsForce.justPressed*/ ){ deSelect = false; }
		}
	}


	resetButton.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
	if( resetButton.released ){
		for( auto i = partSys.particles.begin(); i != partSys.particles.end(); ){
			selected = false;
			moving = false;
			i = partSys.particles.erase( i );
		}
		setupParticles();
	}

	viewButton.draw( pixels, windowWidth, mouseX, mouseY, mouseIsPressed, !moving );
	if(  viewButton.justSet  ){
		if( viewButton.isPressed ){
			displayMode = 0;
			lockedButton.isPressed = false;
			//exertsForce.isPressed = true;
		}
		else{
			displayMode = 1;
			colorMode = 2;
		}
	}
	if( viewButton.isPressed && !moving ){
		for( auto i = partSys.particles.begin(); i != partSys.particles.end(); ){
			if( (*i).x < 0 || (*i).x > (windowWidth*1.0 / shortDim)  ||  (*i).y < 0 || (*i).y > (windowHeight*1.0 / shortDim) ){
				if( (*i).properties & 0b00010000 ){
					selected = false;
					moving = false;
				}
				i = partSys.particles.erase( i );
			}
			else{
				++i;
			}
		}
	}

	if( displayMode == 0 ){
		partSys.drawDots(pixels, windowWidth, windowHeight);

		if( showVecButton.isPressed ){
			double intermediate;
			double forceX = 0, forceY = 0;

			for( unsigned int loopX = 1; loopX <= gridNum; loopX ++ ){
				double vecX = (double)loopX * windowWidth / shortDim / (gridNum + 1);
				for( unsigned int loopY = 1; loopY <= gridNum; loopY ++ ){
					double vecY = (double)loopY * windowHeight / shortDim / (gridNum + 1);
					forceX = 0; forceY = 0;
					for( auto i = partSys.particles.begin(); i != partSys.particles.end(); ++i ){
						intermediate = (-1 * (*i).charge) / cube( dist2D(vecX, vecY, (*i).x, (*i).y) );
						forceX += intermediate * ( (*i).x - vecX );
						forceY += intermediate * ( (*i).y - vecY );
					}
					{//to make temp temporary
						double temp = sqrt(forceX*forceX + forceY*forceY);
						if( temp > 600.0/(gridNum + 1) ){
							//Color color;
							//color.setHue(  );
							forceX *= 600.0 / (gridNum + 1) / temp;
							forceY *= 600.0 / (gridNum + 1) / temp;
						}
					}
					line( (int)( vecX * shortDim ), (int)( vecY * shortDim ), (int)abs( (vecX + forceX/1000) * shortDim ), (int)abs( (vecY + forceY/1000) * shortDim ), 0xff00ff00 );
				}
			}

			for( auto i = partSys.particles.begin(); i != partSys.particles.end(); ++i ){
				if( (*i).properties & 0b10000000 ){
					forceX = 0; forceY = 0;
					for( auto iSub = partSys.particles.begin(); iSub != partSys.particles.end(); ++iSub ){
						if( (*iSub).id != (*i).id  &&  (*iSub).properties & 0b01000000 ){
							intermediate = (-1 * (*iSub).charge) / cube( dist2D((*i).x, (*i).y, (*iSub).x, (*iSub).y) );
							forceX += intermediate * ( (*iSub).x - (*i).x ) * (*i).charge;
							forceY += intermediate * ( (*iSub).y - (*i).y ) * (*i).charge;
						}
					}
					{//to make temp temporary
						double temp = sqrt(forceX*forceX + forceY*forceY);
						if( temp > 600.0/(gridNum + 1) ){
							//Color color;
							//color.setHue(  );
							forceX *= 600.0 / (gridNum + 1) / temp;
							forceY *= 600.0 / (gridNum + 1) / temp;
						}
					}
					line( (int)( (*i).x * shortDim ), (int)( (*i).y * shortDim ), (int)abs( ((*i).x + forceX/1000) * shortDim ), (int)abs( ((*i).y + forceY/1000) * shortDim ), 0xff0000ff );
				}
			}
		}
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
				lockedButton.isPressed = !( (*i).properties & 0b10000000 );
				//exertsForce.isPressed = (*i).properties & 0b01000000;
				Particle swap = partSys.particles.back();
				partSys.particles.back() = *i;
				*i = swap;
				break;
			}
		}
		if( !moving  &&  mouseX >= windowWidth*39/40 - shortDim*3/50  &&  mouseX <= windowWidth*39/40 - shortDim/50  &&
		                 mouseY >= shortDim/50 + windowHeight/50  &&  mouseY <= shortDim*3/25 + windowHeight/50 ){
			deSelect = false;
			if( mouseY <= shortDim*3/50 + windowHeight/50 ){
				if(  square( (double)mouseX - windowWidth*39/40 + shortDim/25 ) + square( (double)mouseY - shortDim/25 - windowHeight/50 ) < square( shortDim / 50 ) ){
					partSys.particles.back().properties &= 0b11100111;
					partSys.addParticle( 1, windowWidth/2, windowHeight, 0, 0, 0b00011000 );
					chargeButton.isPressed = false;
					moving = true;
					offsetY = ( -(double)mouseY + shortDim/25.0 + windowHeight/50 ) / shortDim;
				}
			}
			else if( mouseY >= shortDim*2/25 + windowHeight/50  &&
			        square( (double)mouseX - windowWidth*39/40 + shortDim/25 ) + square( (double)mouseY - shortDim/10 - windowHeight/50 ) < square( shortDim / 50 ) ){
				partSys.particles.back().properties &= 0b11100111;
				partSys.addParticle( -1, windowWidth/2, windowHeight, 0, 0, 0b00011000 );
				chargeButton.isPressed = true;
				moving = true;
				offsetY = ( -(double)mouseY + shortDim/10.0 + windowHeight/50 ) / shortDim;
			}
			if( moving ){
				partSys.particles.back().properties +=  (!addLockedButton.isPressed<<7) /*+ (exertsForce.isPressed<<6)*/;
				lockedButton.isPressed = addLockedButton.isPressed;
				selected = true;
				offsetX = ( -(double)mouseX - shortDim/25.0 + windowWidth*39/40 ) / shortDim;
			}
		}
	}
}

void mouseReleased(){
	moving = false;
	partSys.particles.back().properties &= 0b11110111;
	partSys.particles.back().properties |= 0b01000000;
	if( deSelect ){
		deSelect = false;
		selected = false;
		partSys.particles.back().properties &= 0b11101111;
	}
}

void windowResized(){
	viewButton.moveResize( windowWidth*34/40, windowHeight*137/150, windowWidth/8, windowHeight/15 );
	pauseButton.moveResize( windowWidth*9/20, windowHeight/50, windowWidth/10, windowHeight/15 );
	resetButton.moveResize( windowWidth/40, windowHeight*137/150, windowWidth/8, windowHeight/15 );
	showVecButton.moveResize( windowWidth*9/20, windowHeight*137/150, windowWidth/10, windowHeight/15 );
	addLockedButton.moveResize( windowWidth*39/40 - shortDim/15, windowHeight/25 + shortDim*7/50, shortDim/15, shortDim/15 );
	chargeButton.moveResize( windowWidth*5/80 + shortDim/15, windowHeight/50, windowWidth*3/32, windowHeight/15 );
	lockedButton.moveResize( windowWidth/40, windowHeight/50, shortDim/15, shortDim/15 );
	//exertsForce.moveResize( windowWidth/40, windowHeight*16/150, shortDim/15, shortDim/15 );

	
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
		windowWidth = DM.w*9/10;
		windowHeight = DM.h*9/10;
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
				if( resetButton.held ){
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

void circle( double posX, double posY, double radius, double borderWidth, double dotSize, Uint32 fill, Uint32 border, Uint32 dotColor ){
	Uint32 color;
	for( int x = -(int)radius; x <= (int)radius; x ++ ){
		for( int y = -(int)radius; y <= (int)radius; y ++ ){
			if( x+posX >= 0 && x+posX < windowWidth  &&  y+posY >= 0 && y+posY < windowHeight   &&   x*x + y*y < radius*radius ){
				if( x*x + y*y < square( radius - borderWidth ) ){
					if( dotSize > 0  &&  x*x + y*y < square( dotSize ) ){
						color = dotColor;
					}
					else{
						color = fill;
					}
				}
				else{ color = border; }
				pixels[ ( (int)floor(y+posY) * windowWidth) + (int)floor(x+posX) ] = color;
			}
		}
	}
}
void line( int x1, int y1, int x2, int y2, Uint32 color ){
	int setX, setY;
	if( x2 == x1 ){ x2 ++; }
	if( y2 == y1 ){ y2 ++; }
	char addByX, addByY;
	if( x1 < x2 ){ addByX = 1; }
	else{ addByX = -1; }
	if( y1 < y2 ){ addByY = 1; }
	else{ addByY = -1; }

	for(setX = x1; setX != x2; setX += addByX){
		setY = y1 + (y2 - y1) * (setX - x1) / (x2 - x1);
		if( (unsigned int)setX < windowWidth  &&  (unsigned int)setY < windowHeight  &&  (unsigned int)((setY*windowWidth)+setX) < windowWidth*windowHeight ){
			pixels[  ( setY * windowWidth ) + setX  ] = color;
		}
	}
	for(setY = y1; setY != y2; setY += addByY){
		setX = x1 + (x2 - x1) * (setY - y1) / (y2 - y1);
		if( (unsigned int)setX < windowWidth  &&  (unsigned int)setY < windowHeight  &&  (unsigned int)((setY*windowWidth)+setX) < windowWidth*windowHeight ){
			pixels[ ( setY * windowWidth ) + setX ] = color;
		}
	}
}


void set( unsigned int x, unsigned int y, Color color ){
	if( x < windowWidth  &&  y < windowHeight  &&  (y*windowWidth)+x < windowWidth*windowHeight ){
		pixels[ ( y * windowWidth ) + x ] = ( 255 << 24 ) + ( color.r << 16 )  + ( color.g << 8 )  +  color.b;
	}
	else{ printf( "Tried to draw pixel out of bounds at (%i, %i)\n", x, y ); }
}


