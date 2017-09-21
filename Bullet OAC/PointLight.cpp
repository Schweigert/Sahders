#include "stdafx.h"
#include "PointLight.h"

#include "Constants.h"

int PointLight::numOfPointLights = 0;

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) :
	Light(ambient, diffuse, specular),
	constant(constant),
	linear(linear),
	quadratic(quadratic)
{
	this->iMyId = PointLight::numOfPointLights++;
	printf("%d\n", PointLight::numOfPointLights);
}


PointLight::~PointLight()
= default;

void PointLight::Initialize()
{
	std::string tag = "pointLights[" + std::to_string(this->iMyId) + "].";

	this->iShaderPosition = glGetUniformLocation(ptrApp->GetMainShaderId(),		(tag + "position").c_str());

	this->iShaderAmbientId = glGetUniformLocation(ptrApp->GetMainShaderId(),	(tag + "ambient").c_str());
	this->iShaderDiffuseId = glGetUniformLocation(ptrApp->GetMainShaderId(),	(tag + "diffuse").c_str());
	this->iShaderSpecularId = glGetUniformLocation(ptrApp->GetMainShaderId(),	(tag + "specular").c_str());

	this->iShaderConstant = glGetUniformLocation(ptrApp->GetMainShaderId(),		(tag + "constant").c_str());

	this->iShaderLinear = glGetUniformLocation(ptrApp->GetMainShaderId(),		(tag + "linear").c_str());
	this->iShaderQuadratic = glGetUniformLocation(ptrApp->GetMainShaderId(),	(tag + "quadratic").c_str());
}

void PointLight::UpdateShader()
{
	glUniform3fv(this->iShaderPosition, 1, &this->GetWorldPosition()[0]);

	glUniform3fv(this->iShaderAmbientId, 1, &this->ambient[0]);
	glUniform3fv(this->iShaderDiffuseId, 1, &this->diffuse[0]);
	glUniform3fv(this->iShaderSpecularId, 1, &this->specular[0]);

	glUniform1f(this->iShaderConstant, this->constant);

	glUniform1f(this->iShaderLinear, this->linear);
	glUniform1f(this->iShaderQuadratic, this->quadratic);


}
