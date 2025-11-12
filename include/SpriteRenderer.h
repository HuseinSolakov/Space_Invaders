#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Texture.h"
#include "ShaderClass.h"

//renders the sprite
class SpriteRenderer
{
	public:
		//constructor
		SpriteRenderer(Shader &shader);
		SpriteRenderer(Shader &shader, int sprite_base, int sprite_distance, int sprite_column, int sprite_row, int img_width,int img_height);
		//destructor
		~SpriteRenderer();
		//does transformations and then draws the texture
		void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
		
	private:
		Shader shader;
		unsigned int quadVAO;
		
		void initRenderData();
		void initRenderData_SpriteSheet(float X_max, float X_min, float Y_max, float Y_min);
};
#endif
