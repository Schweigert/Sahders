#pragma once
#include "Light.h"
class SpotLight :
	public Light
{

protected:
	unsigned int iShaderDirection,
		iShaderPosition,
		iShaderConstant,
		iShaderLinear,
		iShaderQuadratic,
		iShaderCutOff,
		iShaderOuterCutOff;

	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;

	glm::vec3 direction;

public:
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, glm::vec3 direction);
	~SpotLight();

	virtual void Initialize() override;


	virtual void UpdateShader() override;

};

