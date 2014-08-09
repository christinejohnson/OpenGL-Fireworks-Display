#include "stdafx.h"

#include "Texture.h"
#include "TGALoader.h"
#include <iostream>

//default constructor
Texture::Texture() { }

//Returns an opengl texture from the passed file, or -1 if not sucessful
GLuint Texture::loadTexture(char * fileName) {
	
	//create a string from the file name
	std::string temp;
	temp.assign(fileName);

	//load a TGA file if in this format
	if (Texture::hasEnding(fileName, ".tga"))
		return Texture::loadTGAFile(fileName);

	return -1;
}

//Loads a TGA image and binds it to a texture
GLuint Texture::loadTGAFile(char * fileName) {

	//load the TGA data
	TGATexture texData;
	LoadTGA(&texData, fileName);

	//generate a new texture
	GLuint newTexture;
	glGenTextures(1, &newTexture);
	glBindTexture(GL_TEXTURE_2D, newTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, texData.bpp / 8, texData.width, texData.height, 0, texData.type, GL_UNSIGNED_BYTE, texData.imageData);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_2D);

	free(texData.imageData);

	return newTexture;
}

//checks to see if a passed string has the other parameter as the ending
bool Texture::hasEnding (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}