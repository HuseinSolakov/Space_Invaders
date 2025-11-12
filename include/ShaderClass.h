#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <string>
#include "glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
    public:
        //state
        GLuint ID;
        
        //constructor
        Shader() { }
        //sets the current shader as active
        Shader &Use();
        
        void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); //geometry optional
	
	//utility functions
	void    SetFloat    (const char* uniform_name, float	value, bool useShader = false);
    	void    SetInteger  (const char* uniform_name, int 	value, bool useShader = false);
   	void    SetVector2f (const char* uniform_name, float	x, float y, bool useShader = false);
   	void    SetVector2f (const char* uniform_name, const	glm::vec2 &value, bool useShader = false);
  	void    SetVector3f (const char* uniform_name, float	x, float y, float z, bool useShader = false);
   	void    SetVector3f (const char* uniform_name, const	glm::vec3 &value, bool useShader = false);
   	void    SetVector4f (const char* uniform_name, float	x, float y, float z, float w, bool useShader = false);
  	void    SetVector4f (const char* uniform_name, const	glm::vec4 &value,  bool useShader = false);
   	void    SetMatrix4  (const char* uniform_name, const	glm::mat4 &matrix, bool useShader = false);

	private:
	//compilation/linking check
	void checkCompileErrors(GLuint object, std::string type);
};

#endif
