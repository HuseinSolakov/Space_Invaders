#include "Renderer.h"
#include <iostream>
//--- ShapeRenderer ---

//constructor
ShapeRenderer::ShapeRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

//destructor
ShapeRenderer::~ShapeRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

//draw
void ShapeRenderer::DrawShape
(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    this->shader.Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    
    model = glm::translate(model, glm::vec3(position, 0.0f));  
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
  
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("shapeColor", color);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);	//let user choose the type
    glBindVertexArray(0);
}

//initialize data
void ShapeRenderer::initRenderData()
{
	//configure VAO & VBO
	//VBO data
	unsigned int VBO;
	float vertices[] = 
	{
		//position
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,		
	};
	
	//gen VAO
	glGenVertexArrays(1, &this->quadVAO);
	//gen VBO
	glGenBuffers(1, &VBO);
	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//store the data inside the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//bind VAO
	glBindVertexArray(this->quadVAO);
	//Specify the shader attribute
	glEnableVertexAttribArray(0);		//location 0
	//configure the vertex attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	//Unbind Buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);		
}

//---- SpriteRenderer ---

//constructor
//for single file texture
SpriteRenderer::SpriteRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

//for sprite sheet
SpriteRenderer::SpriteRenderer
(Shader &shader, int sprite_base, glm::vec2 sprite_distance, int sprite_column, int sprite_row, int img_width,int img_height)
{
    base = sprite_base;
    distance = sprite_distance;
    
    width = img_width;
    height = img_height;

    SetSpriteLocation(sprite_column, sprite_row);
    
    this->shader = shader;
}

//destructor - delete VAO
SpriteRenderer::~SpriteRenderer()
{
        glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f)); 
  
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColor", color);
  
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

//sprite location
void SpriteRenderer::SetSpriteLocation(int sprite_column, int sprite_row)
{
    //find NDC pixel
    double NDC_pixel_x = 1.0 / width;
    double NDC_pixel_y = 1.0 / height;
    int current_sprite = 0;
       
    //1// Getting the sprite max min for X axis
    current_sprite = base * sprite_column;
    //if its not first sprite add distance
    if(sprite_column > 1)
	current_sprite += distance.x;
    
    //max min value x axis
    float X_sprite_max = NDC_pixel_x * current_sprite;
    float X_sprite_min = NDC_pixel_x * (current_sprite - base);
    //if its first sprite
    if(sprite_column < 1)
        X_sprite_min = 0.0;


    //2// Getting the max min value for  Y axis
    current_sprite = base * sprite_row;
    current_sprite += distance.y;
    
    float Y_sprite_max = NDC_pixel_y * current_sprite;
    float Y_sprite_min = NDC_pixel_y * (current_sprite - base);
    //if its first sprite
    if(sprite_row < 1)
        Y_sprite_min = 0.0;
        
   this->initRenderData_SpriteSheet(X_sprite_max, X_sprite_min, Y_sprite_max, Y_sprite_min);
}

//configure buffers for single image sprite 
void SpriteRenderer::initRenderData()
{
	//configure VAO & VBO
	//VBO data
	unsigned int VBO;
	float vertices[] = 
	{
		//position	texture (coordinates of picture)
		0.0f, 1.0f,	0.0f, 1.0f,
		1.0f, 0.0f,	1.0f, 0.0f,
		0.0f, 0.0f,	0.0f, 0.0f,
		
		0.0f, 1.0f,	0.0f, 1.0f,
		1.0f, 1.0f,	1.0f, 1.0f,
		1.0f, 0.0f,	1.0f, 0.0f,
		
	};
	
	//gen VAO
	glGenVertexArrays(1, &this->quadVAO);
	//gen VBO
	glGenBuffers(1, &VBO);
	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//store the data inside the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//bind VAO
	glBindVertexArray(this->quadVAO);
	//Specify the shader attribute
	glEnableVertexAttribArray(0);		//location 0
	//configure the vertex attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);

	//Unbind Buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);		
}

//configure buffers for sprite sheet
void SpriteRenderer::initRenderData_SpriteSheet(float X_max, float X_min, float Y_max, float Y_min)
{
	//configure VAO & VBO
	//VBO data
	unsigned int VBO;
	float vertices[] = 
	{
		//position	texture (coordinates of picture)
		0.0f, 1.0f,	X_min, Y_max,
		1.0f, 0.0f,	X_max, Y_min,
		0.0f, 0.0f,	X_min, Y_min,
		
		0.0f, 1.0f,	X_min, Y_max,
		1.0f, 1.0f,	X_max, Y_max,
		1.0f, 0.0f,	X_max, Y_min,
	};
	
	//gen VAO
	glGenVertexArrays(1, &this->quadVAO);
	//gen VBO
	glGenBuffers(1, &VBO);
	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//store the data inside the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//bind VAO
	glBindVertexArray(this->quadVAO);
	//Specify the shader attribute
	glEnableVertexAttribArray(0);		//location 0
	//configure the vertex attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);

	//Unbind Buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);		
}
