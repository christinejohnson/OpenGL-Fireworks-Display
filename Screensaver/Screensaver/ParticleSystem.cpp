#include "stdafx.h"

#include <algorithm>
#include <iostream>

#include "ParticleSystem.h"
#include "Texture.h"

//===================================================================================
//Particle System methods

//Create a particle system with;
//a texture id
//the max amount of particles the system should take
ParticleSystem::ParticleSystem(GLuint texture, int maxParticles) {

	//load the texture for the particles
	_texture = texture;

	MaxParticles = maxParticles;

	//set default values
	loadDefaultValues();
}

//Create a particle system with;
//a texture name
//the max amount of particles the system should take
ParticleSystem::ParticleSystem(char * particleTexture, int maxParticles) {

	//load the texture for the particles
	_texture = Texture::loadTexture(particleTexture);

	MaxParticles = maxParticles;

	//set default values
	loadDefaultValues();
}

//Loads the default values for the particle system
void ParticleSystem::loadDefaultValues() {

	StartScale = 1;
	EndScale = 1;
	StartScaleVariance = 0;
	EndScaleVariance = 0;

	Angle = 0;
	AngleVariance = 0;

	ParticleSize = 0.02f;
	ParticleLife = 1;
	ParticleLifeVariance = 0;

	MoveSpeed = 0;
	MoveVariance = 0;
	Gravity = Vector3(0, 0, 0);

	EmissionRate = 0;
	ShouldRegenParticles = true;
	_emissionCountdown = 0;
	_totalActiveParticles = 0;

	StartColour = RGBA(1, 1, 1, 1);
	StartColourVariance = RGBA(0, 0, 0, 0);
	EndColour = RGBA(0, 0, 0, 1);
	EndColourVariance = RGBA(0, 0, 0, 0);

	BlendFactor = GL_ONE;
}

//update the particle system with;
//The difference in time since the last update
void ParticleSystem::update(float dt) {

	//loop through all the active particles
	for (int i = 0; i < (int)_activeParticles.size(); i++) {

		Particle * loopedParticle = _activeParticles[i];

		//if the particle is older than what it should be..
		if (loopedParticle->Age > loopedParticle->MaxAge) {

			//remove the particle
			removeParticle(loopedParticle);
			//reduce the iterator because we've just deleted a particle and don't want to skip over the next in the loop
			i--;

			continue;
		}

		//updating them
		_activeParticles[i]->update(dt);

		//affect the particle's velocity by the system's gravity
		_activeParticles[i]->MoveVector = _activeParticles[i]->MoveVector + (Gravity * dt);
	}

	//if the system is allowed to regen particles...
	if (ShouldRegenParticles == true) {

		//reduce the emission count by the dt so we know when we can next spawn a particle
		if (_emissionCountdown > 0)
			_emissionCountdown -= dt;

		//else if the emission countdown has been passed then we can spawn more
		else
			replenishParticles(dt);
	}
}

//Replenishes particles, spawning new particles while the number of active particles is below the max amount of particles allowed with;
//the time since the last update call
void ParticleSystem::replenishParticles(float dt) {

	int numParticlesToSpawn;
	//work out the number of particles that can be spawned in the length of time it took since the last update if the system is spawning faster than the systems FPS
	if (EmissionRate < dt)
		numParticlesToSpawn = dt / EmissionRate;
	else
		numParticlesToSpawn = 1;

	//if the system can spawn more particles and the cooldown for spawning particles has been passed, then spawn a new particle
	while ((numParticlesToSpawn > 0 || EmissionRate == 0) && _totalActiveParticles < MaxParticles) {

		//decrement the num of particles to spawn count
		numParticlesToSpawn--;
		spawnParticle();
	}

	//reset the emission count
	_emissionCountdown = EmissionRate;
}

//Spawn a particle according to the particle system's vars
void ParticleSystem::spawnParticle() {

	//create a new particle
	Particle * newParticle = new Particle();
	_activeParticles.push_back(newParticle);

	_totalActiveParticles++;

	//set particle variables by applying a random variance to the related variance

	//working out position with variance
	Vector3 randomOffset;
	randomOffset.random(2);
	randomOffset.z = 0;
	newParticle->Position = (Origin - OriginVariance) + (OriginVariance * randomOffset);

	//working out the various properties with variance
	newParticle->Scale = (StartScale - StartScaleVariance) + StartScaleVariance * ((float)rand() / ((float)RAND_MAX / 2));
	newParticle->EndScale = (EndScale - EndScaleVariance) + EndScaleVariance * ((float)rand() / ((float)RAND_MAX / 2));
	newParticle->MaxAge = (ParticleLife - ParticleLifeVariance) + ParticleLifeVariance * ((float)rand() / ((float)RAND_MAX / 2));

	//working out the colour variances
	RGBA randomColourOffset;
	randomColourOffset.random();
	newParticle->Colour = (StartColour - StartColourVariance) + StartColourVariance * randomColourOffset * 2;

	//if the end colours are all -1, then programmer wants the end colour to be the same as the calculated start colour
	if (EndColour.r == -1 && EndColour.g == -1 && EndColour.b == -1) {

		newParticle->EndColour = newParticle->Colour;
		newParticle->EndColour.a = EndColour.a;
	}

	else
		newParticle->EndColour = (EndColour - EndColourVariance) + EndColourVariance * randomColourOffset;

	//if the alpha chanel is -1 then programmer wants end alpha to be same as calculated start alpha
	if (EndColour.a == -1)
		newParticle->EndColour.a = newParticle->Colour.a;

	//work out the direction for the particle by adjusting the starting angle
	float randomAngle = (Angle - AngleVariance) + AngleVariance * ((float)rand() / ((float)RAND_MAX / 2));
	//convert to radians
	float radians = (3.141592 * (randomAngle + 90)) / 180;
	//convert radian angle to direction vector
	newParticle->MoveVector = Vector3(-cosf(radians), sinf(radians), 0);
	float randMoveSpeed = (MoveSpeed - MoveVariance) + MoveVariance * ((float)rand() / ((float)RAND_MAX / 2));
	newParticle->MoveVector = newParticle->MoveVector * randMoveSpeed;
}

//Removes a particle from the system with;
//The particle to remove
void ParticleSystem::removeParticle(Particle * particleToRemove) {

	//get the index of the particle in the vector
	int pos = std::find(_activeParticles.begin(), _activeParticles.end(), particleToRemove) - _activeParticles.begin();

	//remove it from the vector
	_activeParticles.erase(_activeParticles.begin() + pos);

	//don't count the particle as removed if it was a trail
	if (particleToRemove->IsFake == false)
		_totalActiveParticles--;

	//release the memory
	delete particleToRemove;
}


//Draw the particles
void ParticleSystem::draw() {

	glPushMatrix();
	
	//set up draw routines ready
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, BlendFactor);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);

	//bind the texture
	glBindTexture(GL_TEXTURE_2D, _texture);

	//loop through all the particles
	for (int i = 0; i < (int)_activeParticles.size(); i++) {

		glPushMatrix();

		Particle loopedParticle = *_activeParticles[i];

		//translate to particle position
		glTranslatef(loopedParticle.Position.x, loopedParticle.Position.y, loopedParticle.Position.z);

		//scale according to particle size
		glScalef(loopedParticle.Scale, loopedParticle.Scale, loopedParticle.Scale);

		//set the colour according to the particle
		glColor4f(loopedParticle.Colour.r, loopedParticle.Colour.g, loopedParticle.Colour.b, loopedParticle.Colour.a);

		//draw the particle
		glBegin(GL_QUADS);

		glTexCoord2d(0, 0);
		glVertex3f(-ParticleSize, -ParticleSize, 0);
		glTexCoord2d(1, 0);
		glVertex3f(ParticleSize, -ParticleSize, 0);
		glTexCoord2d(1, 1);
		glVertex3f(ParticleSize, ParticleSize, 0);
		glTexCoord2d(0, 1);
		glVertex3f(-ParticleSize, ParticleSize, 0);

		glEnd();

		glColor4f(1, 1, 1, 1);
		glScalef(1, 1, 1);

		glPopMatrix();
	}

	//undo the gl changes we did
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

//===================================================================================
//Particle methods

//default constructor
Particle::Particle() {

	Scale = 1;
	EndScale = 1;
	Age = 0;
	IsFake = false;
}

//update particle with;
//The time since the last update
void Particle::update(float dt) {

	//trick to make it that the first update of the particle saves starting values
	if (Age == 0) {
		
		_startColour = Colour;
		_startScale = Scale;
	}

	Age += dt;

	Scale = _startScale + (EndScale - _startScale) * (Age / MaxAge);

	Position = Position + (MoveVector * dt);

	Colour = RGBA(_startColour, EndColour, (Age / MaxAge));
}
