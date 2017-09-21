#include "stdafx.h"
#include "DirectionalLight.h"

#include "Constants.h"

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(ambient, diffuse, specular)
{

}

DirectionalLight::~DirectionalLight()
= default;

void DirectionalLight::UpdateShader()
{
	glUniform3fv(this->iShaderDirectionId, 1, &this->vTranslate[0]);
	glUniform3fv(this->iShaderAmbientId, 1, &this->ambient[0]);
	glUniform3fv(this->iShaderDiffuseId, 1, &this->diffuse[0]);
	glUniform3fv(this->iShaderSpecularId, 1, &this->specular[0]);
}

void DirectionalLight::Initialize()
{
	this->iShaderDirectionId = glGetUniformLocation(ptrApp->GetMainShaderId(), "dirLight.direction");
	this->iShaderAmbientId = glGetUniformLocation(ptrApp->GetMainShaderId(), "dirLight.ambient");
	this->iShaderDiffuseId = glGetUniformLocation(ptrApp->GetMainShaderId(), "dirLight.diffuse");
	this->iShaderSpecularId = glGetUniformLocation(ptrApp->GetMainShaderId(), "dirLight.specular");
}
