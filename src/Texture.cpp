#include <iostream>
#include "Texture.h"

//Constructor - initializing default values and creating the texture object
Texture2D::Texture2D() : img_Width(0), img_Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

//Load image data and setup texture
void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	this->img_Width = width;
	this->img_Height = height;
	
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->ImageFormat, GL_UNSIGNED_BYTE, data);
	
	//options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
	
	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0); 
}

//Bind the texture
void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

//Change the format
void Texture2D::setFormat(GLuint format)
{
	InternalFormat = format;
	ImageFormat = format;
}
