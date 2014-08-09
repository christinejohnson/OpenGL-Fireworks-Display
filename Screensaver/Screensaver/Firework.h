#ifndef FireworkH
#define FireworkH

#include "stdafx.h"

#include "ParticleSystem.h"

enum FireworkStates { kReady, kLaunching, kPrepExplode, kExploding, kFinished };

//Firework is a subclass of Particle system.
//It has 2 parts, 1) where it launches up in the air and 2) when it explodes out.
//Call the launch method to start the sequence
//After it is all finished the flag "Has Finished" will be set to true
class Firework : public ParticleSystem {

	//===================================================================================
	//Variables
public:
	//These LaunchVariables will move the origin of the particle system
	float LaunchSpeed;	//The speed which the launched firework moves at
	Vector3 LaunchVector;	//The direction the firework moves when its launching
	
	float SparkSpeed;	//The speed which the sparks move at when the rocket is launching
	float SparkAngle;	//The direction the sparks moves at when its rocket is launching, degrees
	float SparkAngleVar;	//The variance in angle in degrees of the sparks when the rocket is launching

	RGBA ExplodeColour;	//The colour of the firework when it explodes
	float ExplodeParticleCount;	//The number of particles that will be spawned on explosion
	float ExplodeSpeed;	//The speed the particles will move away from the center of the explosion
	float ExplodeSpeedVar;	//The variance in the speed which the firework explodes out at
	Vector3 ExplodeGravity;	//The gravity acting on particles from an exploded firework

	bool ShouldEmitTrail;	//Flag for weither the firework should have a trail or not when exploding
	int UpdatesPerTrail;	//The number of updates that take place before a trail is extended. Higher values mean better performance
	float TrailLifeModifier;	//The modifier to the length of a particle in the trail

	FireworkStates State;	//The state the firework is currently in

protected:
	float _timeSinceLaunch;	//If the firework is currently launching, this records the time since 
	float _launchDuration;	//The length of time a launch should last
	int _tailUpdatesCount;	//The number of updates taken place since the last trail update
	FireworkStates _previousState;	//The previous state the firework was in the last update call

	//===================================================================================
	//Methods
public:
	Firework::Firework(GLuint texture);
	Firework::Firework(char * particleTexture);
	virtual void loadDefaultValues();
	virtual void launchWithDuration(float timeToExplosion);
	virtual void update(float dt);

protected:
	virtual bool hasFinishedExplosion();
	virtual void explodePrep();
	virtual void explode();
};


#endif