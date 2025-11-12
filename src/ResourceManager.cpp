#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"

//instantiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader>    ResourceManager::Shaders;

//Load/Create shader program
Shader ResourceManager::LoadShader(const char* VertexFilePath, const char* FragmentFilePath, const char* GeometryFilePath, std::string name)
{
	Shaders[name] = loadShaderFromFile(VertexFilePath, FragmentFilePath, GeometryFilePath);
	return Shaders[name];
}
//Get the shader program
Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

//Load/Create texture
Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

//Get Texture via key
Texture2D& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

//Delete the shaders and textures
void ResourceManager::Clear()
{
	//delete all shaders
	for(auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	
	//delete all textures
	for(auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

//Retrieve source code and create shader program
Shader ResourceManager::loadShaderFromFile(const char* VertexFilePath, const char* FragmentFilePath, const char* GeometryFilePath)
{
	// 1. retrieve the shaders source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	
	try
	{
		//open the files in input mode
		std::ifstream vertexFile(VertexFilePath);
		std::ifstream fragmentFile(FragmentFilePath);
		//string stream
		std::stringstream vertexStream, fragmentStream;
	        
	        // read file's buffer contents into streams
	        vertexStream   << vertexFile.rdbuf();
	        fragmentStream << fragmentFile.rdbuf();
	        
	        //close file handlers
	        vertexFile.close();
	        fragmentFile.close();
	        
	        //convert stream into string
	        vertexCode   = vertexStream.str();
	        fragmentCode = fragmentStream.str();
	        
	        //if geometry shader is used
	        if(GeometryFilePath != nullptr)
		{
			std::ifstream geometryFile(GeometryFilePath);
			std::stringstream geometryStream;
			//read content to string stream
			geometryStream << geometryFile.rdbuf();
			//close file
			geometryFile.close();
			//convert stream into string
			geometryCode = geometryStream.str();
		}
	} catch (std::exception e) {std::cout<< "ERROR:SHADER: Failed to read shader files" << std::endl;}
	
	//convert the string into c string
	const char* vertex   =   vertexCode.c_str();
	const char* fragment = fragmentCode.c_str();
	const char* geometry = geometryCode.c_str();

	// 2. create shader object from source code
	Shader shader_program;
	shader_program.Compile(vertex, fragment, GeometryFilePath != nullptr ? geometry : nullptr);
	
	return shader_program;
}

//Load texture image and Generate the texture
Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha )
{
	//create texture object
	Texture2D texture;
	
	//modify the format if we are using RGBA (for .png)
	if(alpha)
	  texture.setFormat(GL_RGBA);
	
	//load image
	int nrChannels;
	unsigned char* data = stbi_load(file, &texture.img_width, &texture.img_height, &nrChannels, 0);
	
	//now generate texture
	texture.Generate(texture.img_width, texture.img_height, data);
	
	//free image data
	stbi_image_free(data);
	return texture;
}
