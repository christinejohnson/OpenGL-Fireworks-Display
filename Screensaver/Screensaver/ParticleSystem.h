#ifndef ParticleSystemH
#define ParticleSystemH

#include "CoordSystems.h"
#include <gl\glut.h>
#include <gl\GL.h>
#include <vector>

#include "ColourSystems.h"

using namespace std;

//prototype
class ParticleSystem;

class Particle {

	//===================================================================================
	//Variables
public:
	Vector3 Position;	//The position the particle is at

	float Age;	//The length of time this particle has been active for
	float MaxAge;	//The max length of time this particle can be alive for

	float Scale;	//The current scale of the particle
	float EndScale;	//The target end scale of the particle

	RGBA Colour;	//The particles colour
	RGBA EndColour;	//The colour this particle will finish at

	Vector3 MoveVector;	//The movement a particle will make per frame

	bool IsFake;	//Flag for weither this particle is part of a trail

private:
	float _startScale;	//The original scale
	RGBA _startColour;	//The original colour

	//===================================================================================
	//Methods
public:
	Particle::Particle();
	void update(float dt);
};

class ParticleSystem {

	//===================================================================================
	//Variables
public:
	Vector3 Origin;	//Position all particles will start off at
	Vector3 OriginVariance;	//The variance in the start position of the particles

	float ParticleLife;	//The amount of time a particle lasts for
	float ParticleLifeVariance;	//The variance in the amount of time a particle is alive for

	float ParticleSize;	//The size of the particle
	float StartScale;	//The start scale of a particle
	float EndScale;	//The end scale of a particle
	float StartScaleVariance;	//The variance in the scale of the particle at the start
	float EndScaleVariance;	//The variance in the scale of the particle at the end

	RGBA StartColour;	//The colour particles start off with
	RGBA EndColour;	//The colour particles end on. Use RGBA(-1, -1, -1, -1) to make end same as start
					//where you can change eg the RGB or the A components to real values for selective copying
	RGBA StartColourVariance;	//The variation in colour particles start off with
	RGBA EndColourVariance;	//The variation in colour particles end with

	float Angle;	//The angle which the particle should move out from, degrees
	float AngleVariance;	//The variance in angle, degrees
	float MoveSpeed;	//The speed at which a particle moves at
	float MoveVariance;	//The variance in a particle's speed

	Vector3 Gravity;	//The force that is applied to the particles every step
	float MaxParticles;	//The max amount of particles that can be active at once
	float EmissionRate;	//The time that needs to expire before a particle can be spawned. 0 means particles spawn instantly
	bool ShouldRegenParticles;	//Flag for weither the system should replace particles that expire

	GLenum BlendFactor;	//how RGBA blending factors are computed when drawing

protected:
	GLuint _texture;	//Texture to draw particles with
	vector<Particle*> _activeParticles;	//The list of active particles
	int _totalActiveParticles;	//Number of active particles
	float _emissionCountdown;	//Time to be decremented so that when 0, a particle can be spawned

	//===================================================================================
	//Methods
public:
	ParticleSystem::ParticleSystem(GLuint texture, int maxParticles);
	ParticleSystem::ParticleSystem(char * particleTexture, int maxParticles);
	virtual void loadDefaultValues();
	virtual void update(float dt);
	virtual void spawnParticle();
	virtual void draw();

protected:
	virtual void replenishParticles(float dt);
	virtual void removeParticle(Particle * particleToRemove);
};

#endif