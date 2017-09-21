#include "stdafx.h"
#include "Entity.h"

#include <sstream>

#include <glm\glm.hpp>

#include "Constants.h"
#include "Utilities.hpp"
#include "Animator.h"
#include "Vehicle.h"

using namespace glm;
/**
\brief Construtor para a classe Entity
Cria uma entidade posicionada na origem, orientada sobre os eixos cartesianos e
em escala canonica.
*/
Entity::Entity()
{
	this->setTranslate(vec3(0.0f, 0.0f, 0.0f));
	this->setRotate(vec3(0.0f, 0.0f, 0.0f));
	this->setRotatePivot(vec3(0.0f, 0.0f, 0.0f));
	this->setScale(vec3(1.0f, 1.0f, 1.0f));
	this->setPivot(vec3(0.0f, 0.0f, 0.0f));
	this->vMaxValue = vec3( -9999.0f, -9999.0f, -9999.0f);
	this->vMinValue = -this->vMaxValue;
	std::ostringstream sstr;
	sstr << "Entity:" << ptrApp->getEntityCount();

	this->setName(sstr.str());
}
/**
\brief Construtor de clonagem para a classe Entity
Cria uma copia exata de um objeto entidade ja existente
\param clone Objeto a ser clonado
*/
Entity::Entity(const Entity& clone)
{
	this->setTranslate(clone.vTranslate);
	this->setRotate(clone.vRotate);
	this->setRotatePivot(clone.vRotatePivot);
	this->setScale(clone.vScale);
	this->setPivot(clone.vPivot);

}
/**
\brief Construtor de clonagem para a classe Entity a partir de ponteiros
Cria uma copia exata de um objeto entidade ja existente
\param ptrClone Ponteiro para o objeto a ser clonado
*/
Entity::Entity(Entity* ptrClone)
{
	this->setTranslate(ptrClone->vTranslate);
	this->setRotate(ptrClone->vRotate);
	this->setRotatePivot(ptrClone->vRotatePivot);
	this->setScale(ptrClone->vScale);
	this->setPivot(ptrClone->vPivot);
}


void Entity::AddEntity(Entity* ent)
{
	this->listOfEntities.push_back(ent);
	ent->setRoot(this);
}


/**
\brief Define a posicao espacial para o objeto (em coordenadas locais)
\param vec coordenadas do vetor translacao
*/
void Entity::setTranslate(vec3 vec)
{
	this->vTranslate = vec;	
}

void Entity::DelEntity(Entity* ent) {
	auto i = this->listOfEntities.begin();

	while (i != this->listOfEntities.end()) {
		// Do some stuff
		if (ent == *i) {
			ent->setRoot(NULL);
			i = this->listOfEntities.erase(i);
			break;
		}
		else
			++i;
	}
}

void Entity::setRoot(Entity* ent)
{
	this->ptrRoot = ent;
}


/**
\brief Define orientacao espacial para o objeto (em coordenadas locais)
\param vec coordenadas do vetor orientacao
*/
void Entity::setRotate(vec3 vec)
{
	this->vRotate = vec;
	qOrientation = quat(vec3(glm::radians(this->vRotate.x), glm::radians(this->vRotate.y), glm::radians(this->vRotate.z)));
}

void Entity::setOrientation(glm::quat ori)
{
	this->qOrientation = ori;
}

/**
\brief Define orientacao espacial para o objeto (em coordenadas locais)
\param vec coordenadas do vetor orientacao
*/
void Entity::setRotatePivot(vec3 vec)
{
	this->vRotatePivot = vec;
}
/**
\brief Define o tamanho do objeto (em coordenadas locais)
\param vec fatores de escala para o objeto
*/
void Entity::setScale(vec3 vec)
{
	this->vScale = vec;
}
/**
\brief Define a posição do pivot do objeto (em coordenadas locais)
\param vec coordenadas para o pivot do objeto
*/
void Entity::setPivot(vec3 vec)
{
	this->vPivot = vec;
}

/**
\brief Rotina de atualizacao dos parametros animaveis do objeto.<p>
Esta rotina deve ser sobrecarregada para cada tipo de entidade em particular.
*/
void Entity::OnLoop(double dDeltaTime)
{

	if (this->ptrAnimator) {
		this->ptrAnimator->OnLoop(dDeltaTime);
	}

	if (this->ptrVehicle) {
		this->ptrVehicle->OnLoop(dDeltaTime);
	}
}
/**
\brief Rotina de renderizacao de sombra do objeto em OPENGL.
*/
void Entity::OnPreRender(glm::mat4 _ModelMatrix) {

	m4ModelMatrix = glm::rotate(_ModelMatrix, glm::radians(this->vRotatePivot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m4ModelMatrix = glm::rotate(m4ModelMatrix, glm::radians(this->vRotatePivot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m4ModelMatrix = glm::rotate(m4ModelMatrix, glm::radians(this->vRotatePivot.x), glm::vec3(1.0f, 0.0f, 0.0f));

	m4ModelMatrix = glm::translate(m4ModelMatrix, this->vPivot);
	m4ModelMatrix = glm::translate(m4ModelMatrix, this->vTranslate) * glm::toMat4(this->qOrientation);

	/*m4ModelMatrix = glm::rotate(m4ModelMatrix, glm::radians(this->vRotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m4ModelMatrix = glm::rotate(m4ModelMatrix, glm::radians(this->vRotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m4ModelMatrix = glm::rotate(m4ModelMatrix, glm::radians(this->vRotate.x), glm::vec3(1.0f, 0.0f, 0.0f));*/

	m4ModelMatrix = glm::scale(m4ModelMatrix, this->vScale);
	m4ModelMatrix = glm::translate(m4ModelMatrix, this->vPivot * -1.0f);

	for (auto & listOfEntitie : this->listOfEntities) {
		listOfEntitie->OnPreRender(m4ModelMatrix);
	}
}

/**
\brief Rotina de renderizacao do objeto em OPENGL.
*/
void Entity::OnRender(glm::mat4 _ModelMatrix)
{
	for (auto & listOfEntitie : this->listOfEntities)
		listOfEntitie->OnRender(m4ModelMatrix);
}
/**
\brief Associa um objeto animador com este ator
\param ptrAnimator ponteiro do objeto animador a ser associado
*/
void Entity::setAnimator(Animator* ptrAnimator)
{
	if (ptrAnimator)
	{
		this->ptrAnimator = ptrAnimator;
		this->ptrAnimator->setParent(this);
	}
}
