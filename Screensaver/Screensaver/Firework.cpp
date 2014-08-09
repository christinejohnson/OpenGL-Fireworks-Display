#include "stdafx.h"

#include "Firework.h"

//Creates a Firework which subclasses a particle system with:
//the id of the texture to use in the firework
Firework::Firework(GLuint texture)
: ParticleSystem(texture, 20) {

	//load default values
	loadDefaultValues();
}

//Creates a Firework which subclasses a particle system with:
//the name of the texture to use in the firework
Firework::Firework(char * particleTexture)
: ParticleSystem(particleTexture, 20) {

	//load default values
	loadDefaultValues();
}

//Loads the default values for the firework
void Firework::loadDefaultValues() {

	ParticleSystem::loadDefaultValues();

	LaunchSpeed = 1.2f;
	LaunchVector = Vector3(0, 1, 0);

	SparkSpeed = 0.7f;
	SparkAngle = 180;
	SparkAngleVar = 20;

	ExplodeColour = RGBA(1, 0, 0, 1);
	ExplodeParticleCount = 60;
	ExplodeSpeed = 0.5f;
	ExplodeSpeedVar = 0.05f;
	ExplodeGravity = Vector3(0, -0.5f, 0);

	ShouldEmitTrail = false;
	UpdatesPerTrail = 1;
	_tailUpdatesCount = 0;
	TrailLifeModifier = 0.4f;

	_timeSinceLaunch = 0;
	State = kReady;

	//load default values for the ParticleSystem as it prepares for launch
	MaxParticles = 100;
	StartColour = RGBA(249 / (float)255, 216 / (float)255, 84 / (float)255, 1);
	StartColourVariance = RGBA(0, 40 / (float)255, 0, 0);
	EndColour = RGBA(-1, -1, -1, 0);
	EndColourVariance = RGBA(0, 0, 0, 0);
	EmissionRate = 0.005f;
	ParticleLife = 0.5f;
}

//Launches the firework along its move vector then explodes it. Uses:
//The time until the firework explodes
void Firework::launchWithDuration(float timeToExplosion) {

	//Only launch the firework if it is in the ready state
	if (State == kReady) {

		State = kLaunching;
		_timeSinceLaunch = 0;
		_launchDuration = timeToExplosion;

		//load values for launching. The particles spawned will represent the sparks 
		Angle = SparkAngle;
		AngleVariance = SparkAngleVar;
		Gravity = Vector3(0, -0.01f, 0);
		MoveSpeed = SparkSpeed;
		StartScale = 0.5f;
		EndScale = 0.1f;

		ShouldRegenParticles = true;
	}
}

//Updates the firework with;
//The time since the last update
void Firework::update(float dt) {

	if (State == kLaunching) {

		_timeSinceLaunch += dt;

		//move the origin of the particle system along the launch vector
		Origin = Origin + (LaunchVector * LaunchSpeed * dt);

		//If the firework has been launching for the target amount of time, then explode it
		if (_timeSinceLaunch > _launchDuration)
			explodePrep();
	}

	else if (State == kPrepExplode) {

		explode();
		State = kExploding;
	}

	else if (State == kExploding) {

		if (hasFinishedExplosion() == true)
			//mark it as finished
			State = kFinished;
	}

	//loop through all of the active particles that aren't trails, if the firework is exploding
	if (ShouldEmitTrail == true && State == kExploding) {

		if (_tailUpdatesCount == UpdatesPerTrail) {

			_tailUpdatesCount = 0;

			for (int i = 0; i < (int)_activeParticles.size(); i++) {

				Particle * loopedParticle = _activeParticles[i];

				//if not a trail then it is eligable to have a trail behind it
				if (loopedParticle->IsFake == false) {

					// create a new trail particle at its current position except with no velocity
					ParticleSystem::spawnParticle();
					Particle * newParticle = _activeParticles.back();
					//decrement the active particle count so its as if this paritcle doesn't exist (we don't want it to be counted since its a trail)
					_totalActiveParticles--;

					//make it a trail
					newParticle->IsFake = true;

					//copy its settings from the particle it is being created from, but make sure it doesn't change colour or move
					newParticle->Age = 0;
					newParticle->MaxAge = (loopedParticle->MaxAge * TrailLifeModifier) - loopedParticle->Age;
					newParticle->Colour = loopedParticle->Colour;
					newParticle->EndColour = loopedParticle->EndColour;
					newParticle->EndScale = loopedParticle->EndScale;
					newParticle->MoveVector = Vector3(0, 0, 0);
					newParticle->Position = loopedParticle->Position;
					newParticle->Scale = loopedParticle->Scale;
				}
			}
		}
		else
			_tailUpdatesCount++;
	}

	//update the base system
	ParticleSystem::update(dt);

	_previousState = State;
}

//Returns true if the firework has finished its 
bool Firework::hasFinishedExplosion() {

	//check if the firework has finally finished
	if (_totalActiveParticles == 0)
		return true;

	else
		return false;
}

//Prepares firework for explosion, setting the particle variables ready
void Firework::explodePrep() {

	State = kPrepExplode;
	_timeSinceLaunch = -1;

	//now load the new particle settings for the explosion state
	StartColour = ExplodeColour;
	StartColourVariance = RGBA(0, 0, 0, 0);
	EndColour = RGBA(-1, -1, -1, 0);
	MoveSpeed = ExplodeSpeed;
	MoveVariance = ExplodeSpeedVar;
	Angle = 0;
	AngleVariance = 360;
	Gravity = ExplodeGravity;
	ShouldRegenParticles = false;
	StartScale = 1.5;
	EndScale = 0.75f;
	ParticleLife = 1.3f;
}

//Explodes the firework, spawning the defined amount of particles
void Firework::explode() {

	//spawn the new particles to represent the explosion
	for (int i = 0; i < ExplodeParticleCount; i++) {

		ParticleSystem::spawnParticle();
	}
}