#ifndef DazzleFireworkH
#define DazzleFireworkH

#include "Firework.h"

//A dazzle firework works just like a normal firework, except has short sparkles upon detonating
class DazzleFirework : public Firework {

	//===================================================================================
	//Variables
public:
	int MaxDazzleParticles;	//The max number of dazzle particles that can be active at a time
	float DazzleLifeSpan;	//The length of time a dazzle particle will last for
	float MaxDazzleTime;	//The max amount of time the firework can dazzle for before it stops

	Vector3 DazzlePosVar;	//The position variance dazzle particles spawn at
	RGBA DazzleColour;	//The colour which the dazzle particles will spawn with

private:
	float _timeSinceDazzleStarted;	//The length of time it has been since the firework started dazzling

	//===================================================================================
	//Methods
public:
	DazzleFirework::DazzleFirework(GLuint texture);
	DazzleFirework::DazzleFirework(char * particleTexture);
	void loadDefaultValues();
	void update(float dt);

protected:
	virtual void explode();
	virtual void replenishParticles(float dt);
};

#endif