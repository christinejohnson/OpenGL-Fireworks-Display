// Screensaver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gl\glut.h>
#include <gl\GL.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>

#include "CoordSystems.h"
#include "Camera.h"
#include "Texture.h"
#include "ParticleSystem.h"
#include "Firework.h"
#include "DazzleFirework.h"

using namespace std;

//prototypes
void RenderScene();
void NormalKeysPressed(unsigned char key, int x, int y);
void SpecialKeysPressed(int key, int x, int y);
void update(int i);
void SpawnFirework();

	Camera * _gameCamera;	
	GLuint _backgroundTexture;

	GLuint _particleTexture;
	vector<Firework*> _fireworks;

	int _maxFireworks = 4;	//The max amount of fireworks to be running at once
	int _activeFireworks = 0;	//Do not change this yourself
	float _spawnChance = 0.01f;	//The percentage spawn chance per frame of a firework going off it there is < maxfireworks on screen

	//  currentTime - previousTime is the time elapsed
	//  between every call of the update function
	//	used to calculate frames per second for accurate updating
	float currentTime = 0;
	float previousTime = 0;

int main(int argc, char **argv) {

	srand((unsigned)time(0)); 

	//init a window etc
	glutInit(&argc, argv);
	glutInitWindowPosition(-1, -1);
	//make the window fill the screen
	glutInitWindowSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Screensaver");

	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	//set up game camera
	_gameCamera = new Camera(kPerspective, 800, 90);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	//set up key listeners
	glutKeyboardFunc(NormalKeysPressed);
	glutSpecialFunc(SpecialKeysPressed);

	//load texture for the fireworks
	_particleTexture = Texture::loadTexture("ParticleTrans.tga");

	//load background texture
	_backgroundTexture = Texture::loadTexture("BackgroundDark.tga");

	//set up update loop
	glutTimerFunc(1, update, 1);

	//the draw and main loop for opengl
	glutDisplayFunc(RenderScene);
	glutMainLoop();

	return 0;
}

void NormalKeysPressed(unsigned char key, int x, int y) {

	float moveSpeed = 0.1;

	switch (key) {

			//w
		case 119: case 87:

			_gameCamera->modifyPositionBy(Vector3(0, 0, moveSpeed));
			break;

			//a
		case 65: case 97:

			_gameCamera->modifyPositionBy(Vector3(-moveSpeed * 2, 0, 0));
			break;

			//s
		case 83: case 115:

			_gameCamera->modifyPositionBy(Vector3(0, 0, -moveSpeed));
			break;

			//d
		case 68: case 100:

			_gameCamera->modifyPositionBy(Vector3(moveSpeed * 2, 0, 0));
			break;

			//escape
		case 27:

			exit(0);
			break;
	}

	glutPostRedisplay();
}

void SpecialKeysPressed(int key, int x, int y) {

	float moveSpeed = 0.1f;

	switch (key) {

		case GLUT_KEY_UP:

			_gameCamera->modifyRotationBy(Vector3(-moveSpeed, 0, 0));
			break;

		case GLUT_KEY_DOWN:

			_gameCamera->modifyRotationBy(Vector3(moveSpeed, 0, 0));
			break;

		case GLUT_KEY_LEFT:

			_gameCamera->modifyRotationBy(Vector3(0, -moveSpeed, 0));
			break;

		case GLUT_KEY_RIGHT:

			_gameCamera->modifyRotationBy(Vector3(0, moveSpeed, 0));
			break;
	}

	glutPostRedisplay();
}

void update(int i) {

	//work out the frames per second
	currentTime = glutGet(GLUT_ELAPSED_TIME);

	float dt = currentTime - previousTime;
	//convert to seconds
	dt /= 1000;

	previousTime = currentTime;

	//update the fireworks
	for (int i = 0; i < (int)_fireworks.size(); i++) {
	
		Firework * loopedFirework = _fireworks[i];

		//only update the firework if it hasn't finished its sequence
		if (loopedFirework->State != kFinished)
			loopedFirework->update(dt);

		//if firework is finished, remove it 
		else {

			//get the index of the particle in the vector
			int pos = std::find(_fireworks.begin(), _fireworks.end(), loopedFirework) - _fireworks.begin();

			//remove it from the vector
			_fireworks.erase(_fireworks.begin() + pos);

			_activeFireworks--;

			//release the memory
			delete loopedFirework;

			//reduce the for loop iteration to avoid invalid index
			i--;
		}
	}

	//if there are less fireworks than the max, roll for a chance for the firework spawning
	if (_activeFireworks < _maxFireworks) {

		if (((float)rand() / (float)RAND_MAX /100) <= _spawnChance) {

			SpawnFirework();
		}
	}

	//reschedule the update and redraw scene at 60fps
	glutTimerFunc(16, update, 1);
	glutPostRedisplay();
}

void SpawnFirework() {

	Firework * newFirework;

	//make 1 in 4 fireworks a dazzle firework
	if ((int)rand() % 4 == 1)
		newFirework = new DazzleFirework(_particleTexture);

	//and the rest just normal fireworks
	else
		newFirework = new Firework(_particleTexture);

	//place it at a random position around the point (-3, -3, -7)
	Vector3 pos = Vector3(-3, -3, -7);
	Vector3 randOffset;
	randOffset.random(5);
	//giving the variation in position added weight on the x axis so the fireworks spread out more
	randOffset.x *= 1.5f;
	pos = pos + randOffset;
	newFirework->Origin = pos;

	//make it a random colour
	RGBA randColour;
	randColour.random();
	randColour.a = 1;
	newFirework->ExplodeColour = randColour;

	//other poperties I like
	newFirework->ShouldEmitTrail = true;

	//make it a rare chance that some of the fireworks have different settings
	if ((int) rand() % 3 == 1) {

		newFirework->BlendFactor = GL_ONE_MINUS_SRC_COLOR;
		newFirework->TrailLifeModifier = 0.6;
		newFirework->ExplodeSpeed = 0.65f;
	}

	//make it launch for a random time
	float launchTime = (float)rand() / ((float)RAND_MAX / 1.5) + 1.7f;

	//add to list
	_fireworks.push_back(newFirework);

	//launch the firework
	newFirework->launchWithDuration(launchTime);

	_activeFireworks++;
}

void DrawBackground() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	_gameCamera->initView(kOrthagraphic);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _backgroundTexture);

	glPushMatrix();

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0);

    glTexCoord2f(0.0f,1);
    glVertex2f(0.0f, 1);

    glTexCoord2f(1, 1);
    glVertex2f(1, 1);

    glTexCoord2f(1, 0.0f);
    glVertex2f(1, 0);

	glEnd();
	glPopMatrix();

	glFlush();
}

void RenderScene() {

	glDisable(GL_LIGHTING);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawBackground();

	glMatrixMode(GL_PROJECTION);
	//reset view
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	//change to perspective
	_gameCamera->initView(kPerspective);

	//set up view to camera
	_gameCamera->moveGLView();

	//draw the fireworks
	for (int i = 0; i < (int)_fireworks.size(); i++) {
	
		Firework * loopedFirework = _fireworks[i];
		loopedFirework->draw();
	}

	glutSwapBuffers();
}