#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "ShaderClass.h"

//renders shapes
class ShapeRenderer
{
public:
	//constructor
	ShapeRenderer(Shader &shader, GLenum usage = GL_STATIC_DRAW);
	//destructor
	~ShapeRenderer();
	
	//draw shape method
	void DrawShape(glm::vec2 position, glm::vec2 size = glm::vec2(10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
	void DrawLine(glm::vec2 position, float size = 10.0f, float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	//shader object
	Shader shader;
	//VAO buffer for vertex data
	unsigned int quadVAO;
	
	//method for initializing data and storing it inside VAO
	void initRenderData(GLenum usage);
};


//renders the sprite
class SpriteRenderer
{
public:
	//constructor  - assigns shader and calls initRenderData
	//single image file
	SpriteRenderer(Shader &shader, GLenum usage = GL_STATIC_DRAW);
	//spritesheet
	SpriteRenderer(Shader &shader, glm::vec2 sprite_base, glm::vec2 sprite_distance, int sprite_column, int sprite_row, int 
	img_width,int img_height, GLenum usage = GL_STATIC_DRAW);
	
	//destructor
	~SpriteRenderer();
	
	//draw - does transformations and then draws the texture
	void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f), float rotate = 0.0f, glm::vec3 
	color = glm::vec3(1.0f));
	
	//Update current sprite			
	void SetSpriteLocation(int sprite_column, int sprite_row);
	
private:
			
	glm::vec2 distance = glm::vec2(0.0f,0.0f);
    	
    	//image dimensions
    	int width = 0;
    	int height = 0;
    	
    	//sprite size
    	glm::vec2 base = glm::vec2(0.0f,0.0f);
	
	unsigned int quadVAO;
	
	Shader shader;
	GLenum usage;
			
	void initRenderData();
	void initRenderData_SpriteSheet(float X_max, float X_min, float Y_max, float Y_min);
};
#endif
