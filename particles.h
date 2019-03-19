#include <vector>

struct Color;

struct Particle{
	unsigned int id;
	double charge;
	double x; double y;
	double velX; double velY;
	unsigned char properties;
	//particleProperties is a series of switches for properties of particle, in order:
	//feels force | exerts force | TODO: mark location? (2D only?) | selected | being moved | unused | unused | unused
};

class ParticleSystem{
	inline double getPotential( double x, double y );
	Color getColor( double potential, int colorMode );
public:
	std::vector<Particle> particles;
	unsigned int detail = 1;

	ParticleSystem();
	void draw( Uint32*& pixels, unsigned int windowWidth, unsigned int windowHeight, unsigned int displayMode, unsigned int colorMode );
	void drawDots( Uint32*& pixels, unsigned int windowWidth, unsigned int windowHeight );
	void simulate();
	void addParticle( double setCharge, double setParticleX, double setParticleY, double setParticleVelX, double setParticleVelY, char setProperties );//make inline?
};


