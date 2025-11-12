#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad.h"

class Texture2D
{
  private:
	//texture dimensions
	GLuint img_Width, img_Height;
	//texture format
	GLuint InternalFormat;	//texture object format
	GLuint ImageFormat;	//image format
	
	//texture configuration
	GLuint Wrap_S;
	GLuint Wrap_T;
	
	GLuint FilterMin;
	GLuint FilterMax;
	
  public:
	//id
	GLuint ID;
	//spriteset dimensions
	int img_width, img_height;
	
	//constructor
	Texture2D();
	
	//Generates texture from image data
	void Generate(GLuint width, GLuint height, unsigned char* data);
	
	//Bind texture
	void Bind() const;
	
	//change texture/image format
	void setFormat(GLuint format);
};

#endif
