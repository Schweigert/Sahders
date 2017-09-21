#include "stdafx.h"
#include "SpotLight.h"

#include "Constants.h"


SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, glm::vec3 direction) :
	Light(ambient, diffuse, specular),
	constant(constant),
	linear(linear),
	quadratic(quadratic),
	cutOff(cutOff),
	outerCutOff(outerCutOff),
	direction(direction)
{

}


SpotLight::~SpotLight()
= default;

void SpotLight::Initialize()
{
	this->iShaderPosition = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.position");
	this->iShaderDirection = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.direction");

	this->iShaderAmbientId = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.ambient");
	this->iShaderDiffuseId = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.diffuse");
	this->iShaderSpecularId = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.specular");

	this->iShaderConstant = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.constant");

	this->iShaderLinear = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.linear");
	this->iShaderQuadratic = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.quadratic");

	this->iShaderCutOff = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.cutOff");
	this->iShaderOuterCutOff = glGetUniformLocation(ptrApp->GetMainShaderId(), "spotLight.outerCutOff");
}

void SpotLight::UpdateShader()
{
	glUniform3fv(this->iShaderPosition, 1, &this->vTranslate[0]);
	glUniform3fv(this->iShaderPosition, 1, &this->direction[0]);

	glUniform3fv(this->iShaderAmbientId, 1, &this->ambient[0]);
	glUniform3fv(this->iShaderDiffuseId, 1, &this->diffuse[0]);
	glUniform3fv(this->iShaderSpecularId, 1, &this->specular[0]);

	glUniform1f(this->iShaderConstant, this->constant);

	glUniform1f(this->iShaderLinear, this->linear);
	glUniform1f(this->iShaderQuadratic, this->quadratic);

	glUniform1f(this->iShaderCutOff, this->cutOff);
	glUniform1f(this->iShaderOuterCutOff, this->outerCutOff);
}
