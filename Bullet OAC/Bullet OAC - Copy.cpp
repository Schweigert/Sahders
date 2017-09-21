/************************************************************************/
/* File: TesteGL.cpp                                                    */
/* Autor: Gustavo Diel                                                  */
/* License: WTFLP                                                       */
/************************************************************************/

/* Basic Include */
#include "stdafx.h"

/* Standard Includes */
#include <Windows.h>
#include <stdlib.h>
#include <ctime>
#include <vector>

/* Utility Includes */
#include "Application.h"
#include "PerlinNoise.h"
#include "Animator.h"
#include "Utilities.hpp"
#include "BulletHandler.h"

/* Interpolates Includes */
#include "Interpolator.h"
#include "LinearInterpolator.h"
#include "BlockingInterpolator.hpp"
#include "PolinomialInterpolator.hpp"
#include "PolynomInterpolator.h"

/* OBJ Includes */
#include "Model.h"
#include "Fence.h"
#include "Tree.h"
#include "Barn.hpp"

/* Actor Includes */
#include "Carro.hpp"
#include "Robot.h"
#include "Cubo.h"
#include "Camera.h"
#include "Light.h"

/* Articulated Actors Includes */
#include "Headphone.h"
#include "Peao.h"
#include "Pessoa.h"
#include "BodyMember.h"
#include "Cow.h"
#include "Dama.h"

/* Physics Includes */
#include "Cloth.h"
#include "BillBoard.h"
#include "Cloud.h"

/* Particles Includes */
#include "ParticleSystem.h"
#include "ParticleSystemModel.h"
#include "Emitter.h"

/* Steering Behaviour */
#include "Vehicle.h"
#include "IBehaviour.h"
#include "SeekBehaviour.h"
#include "AvoidBoidBehaviour.h"
#include "AvoidObstacleBehaviour.h"
#include "WanderBehaviour.h"

/* GLM Includes */
#include <glm/glm.hpp>
#include "StayWithin.h"
#include "Dove.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"


using namespace glm;

Application* ptrApp;

int main()
{
	ptrApp = new Application(2560, 1440);

	std::vector<Entity*> arvores;

	// **********************************************************
	//                            BLACK GROUND
	// **********************************************************
	Model * ptrGround = new Model("objs/box.obj", "textures/preto.jpg");
	ptrGround->setScale(vec3(32, 1.0f, 32));
	ptrGround->setTranslate(vec3(0, -0.5f, 0));
	ptrGround->getMaterial()->SetSpecularColor(0.00, 0.00, 0.00);
	ptrGround->getMaterial()->SetAmbientColor(0.00, 0.00, 0.00);

	ptrGround->setStatic(true);
	ptrGround->ActivatePhysics(Entity::PLANE);

	ptrApp->AddEntity(ptrGround);

	Model * ptrWall = new Model("objs/box.obj", "textures/preto.jpg");
	ptrWall->setScale(vec3(1, 32.0f, 32));
	ptrWall->setTranslate(vec3(10, 8, 0));
	ptrWall->getMaterial()->SetSpecularColor(0.05, 0.05, 0.05);

	ptrWall->setStatic(true);
	ptrWall->ActivatePhysics(Entity::PLANE);

	ptrApp->AddEntity(ptrWall);


	// **********************************************************
	//                            DIEL PRODUCOES
	// **********************************************************
	Model *prod = new Model("objs/prod.obj", "textures/branco.jpg");
	prod->setTranslate(vec3(2, 100, 2));
	prod->setRotate(vec3(0, -90, 0));
	ptrApp->AddEntity(prod);

	prod->setMass(25.0f);
	prod->ActivatePhysics(Entity::BOX);

	for (auto letra : { 3,2,1,0 }) {
		char * nome;
		switch (letra)
		{
		case 0:
			nome = "objs/D.obj";
			break;
		case 1:
			nome = "objs/i.obj";
			break;
		case 2:
			nome = "objs/e.obj";
			break;
		case 3:
			nome = "objs/L.obj";
			break;
		default:
			nome = "objs/circle1cm.obj";
			break;
		}

		Model *cubo = new Model(nome, "textures/branco.jpg");
		cubo->setTranslate(vec3(2, 0.8, -10 + letra));
		cubo->setRotate(vec3(0, -90, 0));
		ptrApp->AddEntity(cubo);



		ptrApp->AddTimerFunction([cubo](double dDeltaTime) {
			cubo->setMass(18.0f);
			cubo->ActivatePhysics(Entity::BOX);
			cubo->getRigidBody()->setLinearVelocity(btVector3(0, 0, 10));
		}, 2.0f);

	}
	for (int i = 0; i < 1500; i++) {
		Model *prod = new Model("objs/circle.obj", "textures/branco.jpg");
		float x = 2 + Utilities::map(rand() % 1000, 0, 1000, -1, 1);
		float y = 150 + Utilities::map(rand() % 1000, 0, 1000, 0, 10);
		float z = Utilities::map(rand() % 1000, 0, 1000, 1, 2);
		prod->setTranslate(vec3(x, y, z));
		prod->setScale(vec3(0.05, 0.05, 0.05));
		ptrApp->AddEntity(prod);

		prod->setMass(5.0f);
		prod->ActivatePhysics(Entity::SPHERE);
	}

	prod = new Model("objs/ponies.obj", "textures/branco.jpg");
	prod->setTranslate(vec3(2, 200, 2));
	ptrApp->AddEntity(prod);
	prod->setScale(vec3(3.5, 3.5, 3.5));
	prod->setMass(25.0f);
	prod->ActivatePhysics(Entity::BOX);

	

	//prod = new Model("objs/casa.obj", "textures/casa.jpg");
	//prod->setTranslate(vec3(2, 20, 2));
	//ptrApp->AddEntity(prod);
	//prod->setScale(vec3(0.05, 0.05, 0.05));
	//prod->setMass(25.0f);
	//prod->ActivatePhysics(Entity::BOX);

	// **********************************************************
	//                            END SCENE
	// **********************************************************

	// **********************************************************
	//                            CAMERA
	// **********************************************************
	Interpolator * ptrLinearCamera = new PolinomialInterpolator(PolinomialInterpolator::CubicaEQuadrada);
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(3.0f, 1.0f, -4.0f), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(1.0f, 1.0f, -5.0f), 4.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(1.0f, 1.0f, -5.0f), 6.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(-2.0f, 1.0f, -5.0f), 11.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(-2.0f, 1.0f, -5.0f), 20.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(0.0f, 1.0f, -5.0f), 23.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg

	ptrLinearCamera->addAttribute(new AttrVector3(vec3(0.0f, 1.0f, -5.0f), 29.8f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(0.0f, 1.1f, -5.1f), 29.9f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(0.0f, 0.9f, -4.9f), 30.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrLinearCamera->addAttribute(new AttrVector3(vec3(0.0f, 1.0f, -5.0f), 30.1f, PoseAttr::TRANSLATE)); // tempo: 1 seg

	Animator *  ptrCameraAnim = new Animator(Animator::ONCE);
	ptrCameraAnim->addInterpolator(ptrLinearCamera);

	Camera* cam = new Camera();
	cam->setTranslate(vec3(-5, 1, 2.5));
	//cam->setAnimator(ptrCameraAnim);

	ptrApp->ptrv3LookAtPos = new vec3(2,0,2.5);

	ptrApp->AddEntity(cam);


	// **********************************************************
	//                            LIGHT
	// **********************************************************

	Interpolator * ptrLinearLight = new PolinomialInterpolator(PolinomialInterpolator::CubicaEQuadrada);
	ptrLinearLight->addAttribute(new AttrVector3(vec3(10, 3, -20), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg

	Animator *  ptrLightAnim = new Animator(Animator::ONCE);
	ptrLightAnim->addInterpolator(ptrLinearLight);

	vec3 lightPos = vec3(-1, 1, -2);
	DirectionalLight* light = new DirectionalLight(vec3(0.02, 0.02, 0.025), vec3(0.2, 0.2, 0.25), vec3(0.05, 0.05, 0.1));
	//light->setAnimator(ptrLightAnim);
	light->setTranslate(lightPos);
	ptrApp->AddLight(light);


	for (int i = 0; i < 2; i++) {
		vec3 cor(0, 0.5, 1);
		if (i == 0)
			cor = vec3(1, 0.5, 0);

		PointLight* lighto = new PointLight(cor, cor, cor, 1.0, 0.09, 0.032);
		lighto->setTranslate(vec3(0, 2, i * 4));
		ptrApp->AddLight(lighto);
	}

	ptrApp->AddTimerFunction([](double dDeltaTime) {
		for (int i = 0; i < ptrApp->getEntityCount(); ++i)
		{
			Entity* ent = ptrApp->GetEntity(i);
			if (ent->hasRigidBody()) {
				vec3 dist = vec3(0, 10, Utilities::map(rand() % 1000, 0, 1000, -20, -8));
				ent->getRigidBody()->setLinearVelocity(btVector3(dist.x, dist.y, dist.z));
			}
		}
	}, 15);

	ptrApp->ptrlMainLight = light;
	ptrApp->ptrcMainCamera = cam;
	//ptrApp->ptrv3CameraPos = new vec3(2, 1, -5);
	//ptrApp->ptraCameraTarget = cubo;//ptrPeao->getBase();

	ptrApp->InitApplication(); 

	printf("%d particulas\n", ptrApp->iParticleCount);
	int a;
	//scanf("%d", &a);
	return 0;
}


