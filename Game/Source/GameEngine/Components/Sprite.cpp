#include <cstddef> //For offsetof function below
#include <GL/glew.h>
#include <GLM\detail\type_vec2.hpp>
#include <GLM\detail\type_vec3.hpp>
#include <math.h>
#include "Sprite.h"
#include "../Graphics/Texture.h"
#include "../ErrorContext.h"
#include "../ErrorHandling.h"
#include "../Vector3D.h"

SpriteComponent::SpriteComponent(Blz::string const imageFilePath, uint16 const imageWidth, uint16 const imageHeight)
{
	Blz::Err::ErrorContext ec("When initializing sprite with image", imageFilePath);

	this->width = static_cast<sfloat>(imageWidth);
	this->height = static_cast<sfloat>(imageHeight);

	Blz::Graphics::Texture texture(imageFilePath);
	this->texture = texture;

	ERRASSERT(texture.id != 0, "Texture did not load properly!");

	vertexData.resize(6);
}

SpriteComponent::SpriteComponent()
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::SetTargetPosition(sfloat screenPositionX, sfloat screenPositionY)
{
	sfloat halfWidth = this->width / 2;

	//Setting sprite origin at bottom middle of image by subtracting half width 
	this->vertexData.at(0).SetPosition(glm::vec3{ screenPositionX + (this->width - halfWidth), screenPositionY + this->height, 0.0f });//Top right corner
	this->vertexData.at(1).SetPosition(glm::vec3{ screenPositionX - halfWidth, screenPositionY + height, 0.0f });//Top left corner
	this->vertexData.at(2).SetPosition(glm::vec3{ screenPositionX - halfWidth, screenPositionY, 0.0f });//Bottom left corner
	this->vertexData.at(3).SetPosition(glm::vec3{ screenPositionX - halfWidth, screenPositionY, 0.0f });//Bottom left corner
	this->vertexData.at(4).SetPosition(glm::vec3{ screenPositionX + (this->width - halfWidth), screenPositionY, 0.0f });//Bottom right corner
	this->vertexData.at(5).SetPosition(glm::vec3{ screenPositionX + (this->width - halfWidth), screenPositionY + this->height, 0.0f });//Top right corner

	//Setting normal UV so only 1 image shows up on screen. 
	this->vertexData.at(0).SetUV(glm::vec2{ 1.0f, 1.0f });
	this->vertexData.at(1).SetUV(glm::vec2{ 0.0f, 1.0f });
	this->vertexData.at(2).SetUV(glm::vec2{ 0.0f, 0.0f });
	this->vertexData.at(3).SetUV(glm::vec2{ 0.0f, 0.0f });
	this->vertexData.at(4).SetUV(glm::vec2{ 1.0f, 0.0f });
	this->vertexData.at(5).SetUV(glm::vec2{ 1.0f, 1.0f });
}

Blz::Graphics::Texture SpriteComponent::GetTexture()
{
	return texture;
}

Blz::SmallVector<Vector3D> SpriteComponent::GetVertexData()
{
	return vertexData;
}
