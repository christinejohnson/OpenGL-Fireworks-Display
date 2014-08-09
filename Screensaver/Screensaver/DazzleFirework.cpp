#include "stdafx.h"

#include "DazzleFirework.h"

//Creates a dazzleFirework which subclasses a firework with:
//the id of the texture to use in the firework
DazzleFirework::DazzleFirework(GLuint texture)
: Firework(texture) {

	//load default values
	loadDefaultValues();
}

//Creates a dazzleFirework which subclasses a firework with:
//the name of the texture to use in the firework
DazzleFirework::DazzleFirework(char * particleTexture)
: Firework(particleTexture) {

	//load default values
	loadDefaultValues();
}

//loads the default values for the dazzle firework
void DazzleFirework::loadDefaultValues() {

	Firework::loadDefaultValues();

	MaxDazzleParticles = 20;
	DazzleLifeSpan = 0.2f;
	DazzlePosVar = Vector3(0.4f, 0.4f, 0);
	DazzleColour = RGBA(1, 1, 0, 1);
	_timeSinceDazzleStarted = 0;
	MaxDazzleTime = 0.8f;
}

//Updates the dazzle firework
void DazzleFirework::update(float dt) {

	//update the normal firework
	Firework::update(dt);

	if (State == kExploding) {

		//if it hasn't already passed the max time, increment the time since the firework started dazzling
		if (_timeSinceDazzleStarted < MaxDazzleTime)
			_timeSinceDazzleStarted += dt;
	}
}

//Explodes the normal firework, then begins work on 
void DazzleFirework::explode() {

	//explode the normal firework first
	Firework::explode();

	//now set up the dazzle system
	ShouldRegenParticles = true;
	ParticleLife = DazzleLifeSpan;
	OriginVariance = DazzlePosVar;
	StartColour = DazzleColour;
	StartColourVariance = RGBA(0, 0, 0, 0);
	EndColour = RGBA(-1, -1, -1, 0);
	EndColourVariance = RGBA(0, 0, 0, 0);
	Gravity = Vector3(0, 0, 0);
	StartScale = 0.75f;
	EndScale = 0;

	//increase the max amount of particles possible to allow for dazzle particles to spawn
	MaxParticles = ExplodeParticleCount + MaxDazzleParticles;
}

//Replenishes particles, spawning new particles while the number of active particles is below the max amount of particles allowed.
//if the system is currently exploding and its a dazzle particle that's being spawned, adjust its life based off
//takes the time since the last update call
void DazzleFirework::replenishParticles(float dt) {

	//Don't allow replenishment when the firework has finished
	if (State == kFinished)
		return;

	//allow normal replenishing when not exploding
	if (State != kExploding)
		Firework::replenishParticles(dt);

	else {
		//if exploding, only allow dazzle replenishment if it hasn't passed the max time
		if (_timeSinceDazzleStarted < MaxDazzleTime)
			Firework::replenishParticles(dt);
	}
}