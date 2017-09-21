/************************************************************************/
/* File: Joint.cpp                                                      */
/* Autor: Gustavo Diel                                                  */
/* License: WTFLP                                                       */
/************************************************************************/

/* Basic Includes */
#include "stdafx.h"
#include "Joint.h"

/* Standard Includes */
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

/* GLM Includes */
#include <glm/gtc/matrix_transform.hpp>

/* Utility Includes */
#include "Constants.h"
#include "Application.h"

/**
\brief Construtor para a classe Joint
Cria uma entidade posicionada na origem, orientada sobre os eixos cartesianos e
em escala canonica.
*/
Joint::Joint() : Entity()
{}
/**
\brief Construtor de clonagem para a classe Joint
Cria uma copia exata de um objeto entidade ja existente
\param clone Objeto a ser clonado
*/
Joint::Joint(const Joint& clone) : Entity(clone)
{}
/**
\brief Construtor de clonagem para a classe Joint a partir de ponteiros
Cria uma copia exata de um objeto entidade ja existente
\param ptrClone Ponteiro para o objeto a ser clonado
*/
Joint::Joint(Joint* ptrClone) : Entity(ptrClone)
{}
/**
\brief Desaloca os objetos dinamicos desta classe
*/
Joint::~Joint()
{
	for (vector<Entity*>::iterator itJoint = this->listOfJoints.begin(); itJoint != this->listOfJoints.end(); itJoint++)
		delete (*itJoint);
}
/**
\brief Associa uma nova junta como filha desta junta
\param ptrJoint ponteiro para a nova junta-filha a ser associada
*/
void Joint::Attach(Entity* ptrJoint)
{
	if (ptrJoint) {
		this->listOfJoints.push_back(ptrJoint);
		ptrJoint->setRoot(this);
	}
}

/**
\brief Desassocia uma nova junta como filha desta junta
\param ptrJoint ponteiro para a ser desassociado
*/
void Joint::Dettach(int indx)
{
	vector<Entity*>::iterator erased = listOfJoints.erase(listOfJoints.begin() + indx);
	(*erased)->setTranslate(this->getTranslate());
	ptrApp->AddEntity(*erased);
}
/**
\brief Desassocia uma nova junta como filha desta junta
\param ptrJoint ponteiro para a ser desassociado
*/
int Joint::GetIndexOf(Entity* joint) {
	for (int i = 0; i < this->listOfJoints.size(); ++i)
		if (this->listOfJoints.at(i) == joint)
			return i;
	return -1;
}	

/**
\brief Rotina para rendering de todas as primitivas graficas que compoem esta junta
*/
void Joint::OnPreRender(glm::mat4 _m4ModelMatrix)
{

	m4JointModelMatrix = glm::rotate(_m4ModelMatrix, glm::radians(this->vRotatePivot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m4JointModelMatrix = glm::rotate(m4JointModelMatrix, glm::radians(this->vRotatePivot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m4JointModelMatrix = glm::rotate(m4JointModelMatrix, glm::radians(this->vRotatePivot.x), glm::vec3(1.0f, 0.0f, 0.0f));

	m4JointModelMatrix = glm::translate(m4JointModelMatrix, this->vPivot);

	m4JointModelMatrix = glm::translate(m4JointModelMatrix, this->vTranslate) * glm::toMat4(qOrientation);
	

	for (vector<Entity*>::iterator itJoint = this->listOfJoints.begin(); itJoint != this->listOfJoints.end(); itJoint++)
		(*itJoint)->OnPreRender(m4JointModelMatrix);

	m4ModelMatrix = glm::scale(m4JointModelMatrix, this->vScale);
	m4ModelMatrix = glm::translate(m4ModelMatrix, this->vPivot * -1.0f);

	for (vector<Entity*>::iterator itEntity = this->listOfEntities.begin(); itEntity != this->listOfEntities.end(); itEntity++)
		(*itEntity)->OnPreRender(m4ModelMatrix);

}

/**
\brief Rotina para rendering de todas as primitivas graficas que compoem esta junta
*/
void Joint::OnRender(glm::mat4 m4ModelMatrix)
{
	for (vector<Entity*>::iterator itJoint = this->listOfJoints.begin(); itJoint != this->listOfJoints.end(); itJoint++)
		(*itJoint)->OnRender(m4JointModelMatrix);


	for (vector<Entity*>::iterator itEntity = this->listOfEntities.begin(); itEntity != this->listOfEntities.end(); itEntity++)
		(*itEntity)->OnRender(m4ModelMatrix);

}

/**
\brief Executa o processo de animação
\param dDeltaTime tempo transcorrido deste a ultima chamada desta função
*/
void Joint::OnLoop(double dDeltaTime)
{
	for (auto & listOfJoint : this->listOfJoints)
		listOfJoint->OnLoop(dDeltaTime);

	Entity::OnLoop(dDeltaTime); // chama a mesma rotina no objeto-base
}