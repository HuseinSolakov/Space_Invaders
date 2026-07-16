#include "ShaderClass.h"
#include <iostream>

//Activates the shader program for use
Shader &Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

//Creating shaders and linking them to shader program
void Shader::Compile(const char* vertexSource, const char* fragementSource, const char* geometrySource)
{
	//shader id's
	GLuint VertexShader, FragmentShader, GeometryShader;
	
	//Vertex Shader
	//create
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	//source
	glShaderSource(VertexShader, 1,&vertexSource, NULL);
	//compile
	glCompileShader(VertexShader);
	//handle errors
	checkCompileErrors(VertexShader, "VERTEX");

	//Fragement Shader
	//create
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//source
	glShaderSource(FragmentShader,1,&fragementSource, NULL);
	//compile
	glCompileShader(FragmentShader);
	//handle errors
	checkCompileErrors(FragmentShader, "FRAGMENT");

	//Geometry Shader
	//if geometry shader is used
	if(geometrySource != nullptr)
	{
		//create
		GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		//source
		glShaderSource(GeometryShader, 1,&geometrySource, NULL);
		//compile
		glCompileShader(GeometryShader);
		//handle erorrs
		checkCompileErrors(GeometryShader, "GEOMETRY");	
	}
	
	//shader program
	//create
	this->ID = glCreateProgram();
	//attach shaders
	glAttachShader(this->ID, VertexShader);
	glAttachShader(this->ID, FragmentShader);
	//attach geometry if used
	if(geometrySource != nullptr)
		glAttachShader(this->ID, GeometryShader);
	
	//link
	glLinkProgram(this->ID);
	//handle errors
	checkCompileErrors(this->ID, "PROGRAM");
	
	//delete shaders after linking
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	if(geometrySource != nullptr)
		glDeleteShader(GeometryShader);		
}

//uniform functions
//float
void Shader::SetFloat(const char* uniform_name, float value, bool useShader)
{
	if(useShader)
		this->Use();
		
	glUniform1f(glGetUniformLocation(this->ID, uniform_name), value);
}
//integer
void Shader::SetInteger(const char* uniform_name, int value, bool useShader)
{
	if(useShader)
		this->Use();
		
	glUniform1i(glGetUniformLocation(this->ID, uniform_name), value);
}

//vec2
void Shader::SetVector2f(const char* uniform_name, float x, float y, bool useShader)
{
	if(useShader)
		this->Use();
		
	glUniform2f(glGetUniformLocation(this->ID, uniform_name), x, y);
}

void Shader::SetVector2f(const char* uniform_name, const glm::vec2 &value, bool useShader)
{
	if(useShader)
		this->Use();
		
	glUniform2f(glGetUniformLocation(this->ID, uniform_name), value.x, value.y);
}


//vec3
void Shader::SetVector3f(const char* uniform_name, float x, float y, float z, bool useShader)
{
	if(useShader)
		this->Use();
		
	glUniform3f(glGetUniformLocation(this->ID, uniform_name), x, y, z);
}

void Shader::SetVector3f(const char* uniform_name, const glm::vec3 &value, bool useShader)
{
	if(useShader)
		this->Use();
		
	glUniform3f(glGetUniformLocation(this->ID, uniform_name), value.x, value.y, value.z);
}

//vec4
void Shader::SetVector4f(const char* uniform_name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, uniform_name), x, y, z, w);
}

void Shader::SetVector4f(const char* uniform_name, const glm::vec4 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, uniform_name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const char* uniform_name, const glm::mat4 &matrix, bool useShader)
{
	if(useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, uniform_name), 1, false, glm::value_ptr(matrix));
}

//Compile error handler
void Shader::checkCompileErrors(GLuint object, std::string type)
{
	int success;
	char infoLog[1024];
	
	//shaders
	if(type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		//if failure
		if(!success)
		{
			//get the log and write it inside infoLog
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			//display the error
			std::cout<<"ERROR::SHADER: Compile-time error: Type:"<<type<<"\n"<<infoLog<<std::endl;
		}
	}
	//program
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout<<"ERROR::SHADER: Link-time error: Type: "<<type<<"\n"<<infoLog<<std::endl;
		}
	}
}
