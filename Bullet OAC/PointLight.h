#pragma once
#include "Light.h"
class PointLight :
	public Light
{
protected:
	float constant;
	float linear;
	float quadratic;

	unsigned int iShaderPosition,
		iShaderConstant,
		iShaderLinear,
		iShaderQuadratic;

	int iMyId;
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	~PointLight();

	static int numOfPointLights;

	virtual void Initialize() override;


	virtual void UpdateShader() override;

};

