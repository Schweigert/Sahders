#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
protected:
	unsigned int iShaderDirectionId;

public:
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~DirectionalLight();

	virtual void UpdateShader() override;

	virtual void Initialize() override;

};

