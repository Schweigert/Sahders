#pragma once
#include "Entity.h"

class Light :
	public Entity
{
protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	unsigned int iShaderAmbientId;
	unsigned int iShaderDiffuseId;
	unsigned int iShaderSpecularId;

public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : ambient(ambient), diffuse(diffuse), specular(specular) {};
	
	virtual void Initialize() override = 0;

	virtual void UpdateShader() = 0;
};

