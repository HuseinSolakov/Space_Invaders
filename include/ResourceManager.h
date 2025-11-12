//Static Singleton implementation of ResourceManager class

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "glad.h"

#include "Texture.h"
#include "ShaderClass.h"
class ResourceManager
{
	public:
		//image
		// int sprite_width, sprite_height, nrChannels;
		
		//resource storage
		static std::map<std::string, Shader>    Shaders;
		static std::map<std::string, Texture2D> Textures;
		
		//loads and generates shader program from a file
		static Shader LoadShader(const char* VertexFilePath, const char* FragmentFilePath, const char* GeometryFilePath, std::string);

		//retrieve a stored shader
		static Shader& GetShader(std::string name);
		
		//load/generate texture
		static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
		
		//retrieve a stored texture
		static Texture2D& GetTexture(std::string name);

		
		
		//de-allocate loaded resources
		static void Clear();
		
	private:
		// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    		ResourceManager() { }
    		// loads and generates a shader from file
		static Shader    loadShaderFromFile(const char* VertexFilePath, const char* FragmentFilePath, const char* GeometryFilePath = nullptr);
		// loads a single texture from file
		static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif
