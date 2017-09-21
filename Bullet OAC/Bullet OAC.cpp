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
#include "Movie.h"


using namespace glm;

Application* ptrApp;

int main()
{
	ptrApp = new Application(1024, 720);

	std::vector<Entity*> arvores;

	// **********************************************************
	//                            BLACK GROUND
	// **********************************************************
	int groundSquareAmount = 16;
	int sizeGrid = 64;
	for (int i = 0; i < groundSquareAmount; ++i) {
		for (int k = 0; k < groundSquareAmount; ++k) {
			Model * ptrGround = new Model("objs/box.obj", "textures/grass_01.jpg");
			float sizeGrd = (float)sizeGrid / groundSquareAmount;
			ptrGround->setScale(vec3(sizeGrd, 0.5f, sizeGrd));
			ptrGround->setTranslate(vec3(i * sizeGrd - sizeGrid / 2.0f, -0.4, k *sizeGrd - sizeGrid / 2.0f));
			ptrGround->getMaterial()->SetSpecularColor(0.0, 0.0, 0.0);

			ptrGround->setStatic(true);

			ptrApp->AddEntity(ptrGround);
		}
	}

	/* Rua */
	for (int i = 0; i < 16; ++i) {
		Model* ptrRoad = new Model("objs/box.obj", "textures/dirt_01.jpg");
		ptrRoad->setScale(vec3(2.0f, 0.1f, 2.0f));
		ptrRoad->setTranslate(vec3(i * 2 - 16, 0, -2.5));
		ptrRoad->getMaterial()->SetSpecularColor(0.0, 0.0, 0.0);

		ptrRoad->setStatic(true);

		ptrApp->AddEntity(ptrRoad);
	}

	/* Calcada */
	for (int i = 0; i < 16; ++i) {
		Model* ptrRoad = new Model("objs/box.obj", "textures/concrete_01.jpg");
		ptrRoad->setScale(vec3(2.0f, 0.2f, 1.0f));
		ptrRoad->setTranslate(vec3(i * 2 - 16, 0, -1));

		ptrRoad->setStatic(true);

		ptrApp->AddEntity(ptrRoad);

		ptrRoad = new Model("objs/box.obj", "textures/concrete_01.jpg");
		ptrRoad->setScale(vec3(2.0f, 0.2f, 1.0f));
		ptrRoad->setTranslate(vec3(i * 2 - 16, 0, -4));

		ptrApp->AddEntity(ptrRoad);
	}

	// **********************************************************
	//                                LAMP
	// **********************************************************

	Model* lamp1 = new Model("objs/lamp.obj", "textures/concrete_01.jpg");
	lamp1->setScale(vec3(0.01f, 0.01f, 0.01f));
	lamp1->setTranslate(vec3(2, 0, -1));

	vec3 cor = vec3(1, 1, 0.25);

	PointLight* lighto = new PointLight(cor, cor, cor, 1.0f, 0.09f, 0.032f);
	lighto->setTranslate(vec3(2, 2, -1));
	ptrApp->AddLight(lighto);

	ptrApp->AddEntity(lamp1);

	// Lamp 2
	Model* lamp2 = new Model("objs/lamp.obj", "textures/concrete_01.jpg");
	lamp2->setScale(vec3(0.01f, 0.01f, 0.01f));
	lamp2->setTranslate(vec3(-6, 0, -1));

	cor = vec3(0.85, 0.85, 1);

	PointLight* lighto2 = new PointLight(cor, cor, cor, 1.0f, 0.09f, 0.032f);
	lighto2->setTranslate(vec3(-6, 2, -1));
	ptrApp->AddLight(lighto2);

	ptrApp->AddEntity(lamp2);

	// **********************************************************
	//                                ARVORES
	// **********************************************************
	Tree * ptrTree = new Tree();
	ptrTree->setTranslate(vec3(2.0f, 0.0f, 2.0f));
	ptrTree->setPivot(vec3(0.0f, 1.0f, 0.0f));
	ptrTree->setRotate(vec3(0.0f, 0.0f, 0.0f));
	ptrApp->AddEntity(ptrTree);
	arvores.push_back(ptrTree);

	Tree * ptrTree1 = new Tree();
	ptrTree1->setTranslate(vec3(2.9f, 0.0f, -5.2f));
	ptrApp->AddEntity(ptrTree1);
	arvores.push_back(ptrTree1);

	for (int i = 0; i < 48; ++i) {
		ptrTree = new Tree();
		ptrTree->setTranslate(vec3(Utilities::map(rand()%1000, 0, 1000, -32, 32), 0.0f, Utilities::map(rand() % 1000, 0, 1000, 5, 30)));
		ptrTree->setPivot(vec3(0.0f, 1.0f, 0.0f));
		ptrTree->setRotate(vec3(0.0f, 0.0f, 0.0f));
		ptrApp->AddEntity(ptrTree);

		arvores.push_back(ptrTree);

	}

	// **********************************************************
	//                                CASA
	// **********************************************************
	Barn * ptrBarn = new Barn();
	ptrBarn->setTranslate(vec3(5.2f, -0.2f, 10.0f));
	ptrBarn->setRotate(vec3(0, 90, 0));
	ptrBarn->Initialize();
	ptrApp->AddEntity(ptrBarn);
	arvores.push_back(ptrBarn);

	Model * ptrCasa = new Model("objs/casa.obj", "textures/casa.jpg");
	ptrCasa->setTranslate(vec3(-2.2f, -0.15f, 4.0f));
	ptrCasa->setRotate(vec3(0, 180, 0));
	ptrCasa->setScale(vec3(0.05f, 0.05f, 0.05f));
	ptrCasa->Initialize();
	ptrApp->AddEntity(ptrCasa);

	arvores.push_back(ptrCasa);

	// **********************************************************
	//                                RICK
	// **********************************************************

	Pessoa * rick = new Pessoa();
	rick->isMorty = false;
	rick->setTranslate(vec3(-3.0f, 0.f, -1.0f));
	rick->setRotate(vec3(0, -65, 0));
	rick->Initialize();

	rick->AbreFecha(2);
	ptrApp->AddEntity(rick);

	// **********************************************************
	//                                MORTY
	// **********************************************************
	Pessoa * morty = new Pessoa();
	morty->isMorty = true;
	morty->setTranslate(vec3(-2.0f, 0.f, -1.0f));
	morty->setRotate(vec3(0, 65, 0));
	morty->Initialize();
	ptrApp->AddEntity(morty);

	// **********************************************************
	//                                UFO
	// **********************************************************
	Interpolator * ufoInt = new PolinomialInterpolator(PolinomialInterpolator::Cubica);
	ufoInt->addAttribute(new AttrVector3(vec3(-6.2f, 5.0f, 15.0f), 0.4f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ufoInt->addAttribute(new AttrVector3(vec3(-6.2f, 0, 2.5f), 1.4f, PoseAttr::TRANSLATE)); // tempo: 1 seg

	for (int i = 0; i < 100; i += 2) {
		ufoInt->addAttribute(new AttrVector3(vec3(-6.2f, 0, 2.5f), 2 + i, PoseAttr::TRANSLATE)); // tempo: 1 seg
		ufoInt->addAttribute(new AttrVector3(vec3(-6.2f, 0.2, 2.5f), 3 + i, PoseAttr::TRANSLATE)); // tempo: 1 seg
	}

	Animator *  ufoAnim = new Animator(Animator::ONCE);
	ufoAnim->addInterpolator(ufoInt);

	Model* ufo = new Model("objs/ufo.obj", "textures/branco.jpg");
	ufo->setScale(vec3(.1f, .1f, .1f));
	ufo->Initialize();
	ufo->setAnimator(ufoAnim);
	ptrApp->AddEntity(ufo);

	ufoInt = new PolinomialInterpolator(PolinomialInterpolator::CubicaEQuadrada);
	ufoInt->addAttribute(new AttrVector3(vec3(-6.2f, 5.0f, 15.0f), 0.4f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ufoInt->addAttribute(new AttrVector3(vec3(-6.2f, 0, 2.5f), 1.4f, PoseAttr::TRANSLATE)); // tempo: 1 seg

	for (int i = 0; i < 100; i += 2) {
		ufoInt->addAttribute(new AttrVector3(vec3(-6.2f, 0, 2.5f), 2 + i, PoseAttr::TRANSLATE)); // tempo: 1 seg
		ufoInt->addAttribute(new AttrVector3(vec3(-6.2f, 0.5, 2.5f), 3 + i, PoseAttr::TRANSLATE)); // tempo: 1 seg
	}

	ufoAnim = new Animator(Animator::ONCE);
	ufoAnim->addInterpolator(ufoInt);

	cor = vec3(0.15, 0.15, 1);
	lighto2 = new PointLight(cor, cor, cor, 0.0, 0.5, 2.0);
	lighto2->setTranslate(vec3(0, 0, 0));

	lighto2->setAnimator(ufoAnim);
	ptrApp->AddLight(lighto2);

	ufo->AddEntity(lamp2);

	// **********************************************************
	//                                NUVEM
	// **********************************************************
	for (int i = 16; i >= 0; --i)
	{
		Cloud *bp = new Cloud(rand() % 3 < 1 ? "textures/cloud.png" : "textures/cloud2.png");
		float x = rand() % 32 - 16.0f;
		float xDest = min(x - rand() % 32, x - 10.0f);
		float y = rand() % 3 + 7.0f;
		float z = 10 + i * 2.0f;
		float sX = rand() % 5 + 1.0f;


		Interpolator * ptrCloudInter = new LinearInterpolator();
		ptrCloudInter->addAttribute(new AttrVector3(vec3(x, y, z), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
		ptrCloudInter->addAttribute(new AttrVector3(vec3(xDest, y, z), 40, PoseAttr::TRANSLATE)); // tempo: 1 seg


		Animator *  ptrCloudAni = new Animator(Animator::ONCE);
		ptrCloudAni->addInterpolator(ptrCloudInter);

		bp->setTranslate(vec3(x, y, z));
		bp->setSize(vec2(sX, sX));
		bp->setAnimator(ptrCloudAni);
		ptrApp->AddBillboard(bp);

	}

	// **********************************************************
	//                                GRASS
	// **********************************************************
	for (int i = 64; i >= 0; --i)
	{
		for (int j = 128; j >= 0; --j) {
			BillBoard *bp = new BillBoard("textures/grass.png");
			float s = (float)Utilities::map(rand() % 1000, 0, 1000, 0.3, 0.5);
			float dX = (rand() % 2) * 0.6f - 0.3f;
			float dZ = (rand() % 2) * 0.6f - 0.3f;
			bp->setTranslate(vec3(j / 2.0f - 32.0f + dX, s / 2.0f - 0.25, i / 2.0f + dZ));
			bp->SetSize(vec2(s, s));
			bp->SetShearable(true);
			ptrApp->AddBillboard(bp);
		}
	}

	// **********************************************************
	//                                CARRO
	// **********************************************************
	Carro *bp = new Carro();
	bp->setScale(vec3(1, 1, 1));
	ptrApp->AddEntity(bp);

	Interpolator * ptrCarroInter = new LinearInterpolator();
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(0, 0, -2), 4.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(16, 0, -2), 8, PoseAttr::TRANSLATE)); // tempo: 1 seg


	Animator *  ptrCarroAnimator = new Animator(Animator::ONCE);
	ptrCarroAnimator->addInterpolator(ptrCarroInter);

	bp->setAnimator(ptrCarroAnimator);

	/////////////////////////
	bp = new Carro();
	bp->setScale(vec3(1, 1, 1));
	ptrApp->AddEntity(bp);

	ptrCarroInter = new LinearInterpolator();
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 9.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(0, 0, -2), 11.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(16, 0, -2), 13, PoseAttr::TRANSLATE)); // tempo: 1 seg

	ptrCarroAnimator = new Animator(Animator::ONCE);
	ptrCarroAnimator->addInterpolator(ptrCarroInter);

	bp->setAnimator(ptrCarroAnimator);

	/////////////////////////
	bp = new Carro();
	bp->setScale(vec3(1, 1, 1));
	ptrApp->AddEntity(bp);

	ptrCarroInter = new LinearInterpolator();
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 12.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(0, 0, -2), 15.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(16, 0, -2), 18, PoseAttr::TRANSLATE)); // tempo: 1 seg

	ptrCarroAnimator = new Animator(Animator::ONCE);
	ptrCarroAnimator->addInterpolator(ptrCarroInter);

	bp->setAnimator(ptrCarroAnimator);

	/////////////////////////
	bp = new Carro();
	bp->setScale(vec3(1, 1, 1));
	ptrApp->AddEntity(bp);

	ptrCarroInter = new LinearInterpolator();
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 30.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(0, 0, -2), 32.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(16, 0, -2), 34, PoseAttr::TRANSLATE)); // tempo: 1 seg

	ptrCarroAnimator = new Animator(Animator::ONCE);
	ptrCarroAnimator->addInterpolator(ptrCarroInter);

	bp->setAnimator(ptrCarroAnimator);

	/////////////////////////
	bp = new Carro();
	bp->setScale(vec3(1, 1, 1));
	ptrApp->AddEntity(bp);

	ptrCarroInter = new LinearInterpolator();
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 42.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(0, 0, -2), 48.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(16, 0, -2), 54, PoseAttr::TRANSLATE)); // tempo: 1 seg

	ptrCarroAnimator = new Animator(Animator::ONCE);
	ptrCarroAnimator->addInterpolator(ptrCarroInter);

	bp->setAnimator(ptrCarroAnimator);

	/////////////////////////
	bp = new Carro();
	bp->setScale(vec3(1, 1, 1));
	ptrApp->AddEntity(bp);

	ptrCarroInter = new LinearInterpolator();
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 52.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(0, 0, -2), 54.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
	ptrCarroInter->addAttribute(new AttrVector3(vec3(16, 0, -2), 56, PoseAttr::TRANSLATE)); // tempo: 1 seg

	ptrCarroAnimator = new Animator(Animator::ONCE);
	ptrCarroAnimator->addInterpolator(ptrCarroInter);

	bp->setAnimator(ptrCarroAnimator);

	/////////////////////////
	for (int i = 0; i < 7; i++) {
		bp = new Carro();
		bp->setScale(vec3(1, 1, 1));
		ptrApp->AddEntity(bp);

		ptrCarroInter = new LinearInterpolator();
		ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg
		ptrCarroInter->addAttribute(new AttrVector3(vec3(-16, 0, -2), 62.0 + i*0.7, PoseAttr::TRANSLATE)); // tempo: 1 seg
		ptrCarroInter->addAttribute(new AttrVector3(vec3(0, 0, -2), 64. + i*0.7, PoseAttr::TRANSLATE)); // tempo: 1 seg
		ptrCarroInter->addAttribute(new AttrVector3(vec3(16, 0, -2), 66. + i*0.7, PoseAttr::TRANSLATE)); // tempo: 1 seg

		ptrCarroAnimator = new Animator(Animator::ONCE);
		ptrCarroAnimator->addInterpolator(ptrCarroInter);

		bp->setAnimator(ptrCarroAnimator);
	}


	// **********************************************************
	//                            RED LIGHT
	// **********************************************************
	cor = vec3(2, 0, 0);
	PointLight* red = new PointLight(cor, cor, cor, 2.0f, 0.09f, 1.032f);
	red->setTranslate(vec3(0, 100, -1));
	ptrApp->AddLight(red);

	ptrApp->AddTimerFunction([red](double dDeltaTime) {
		red->setTranslate(vec3(-2, 1, -1));
	}, 68);

	// **********************************************************
	//                            TESTING
	// **********************************************************
	Movie *movie = new Movie();
	movie->setTranslate(vec3(3, 1, 0));
	movie->SetSize(vec2(-5, -5));
	ptrApp->AddBillboard(movie);


	// **********************************************************
	//                            VACAS
	// **********************************************************
	for (int letra = 0; letra < 15; letra++) {
		Cow *cuboF = new Cow();
		cuboF->setTranslate(vec3(letra,-0.2, 8));
		cuboF->addWalk(0, 100, 0.5);
		cuboF->Initialize();
		ptrApp->AddEntity(cuboF);

		Vehicle* veh = new Vehicle(cuboF);
		veh->AddBehaviour(new WanderBehaviour());
		veh->AddBehaviour(new AvoidBoidBehaviour());
		veh->AddBehaviour(new AvoidObstacleBehaviour(arvores));
		veh->AddBehaviour(new StayWithin());

	}

	// **********************************************************
	//                            END SCENE
	// **********************************************************
	Camera* cam = new Camera();
	cam->setTranslate(vec3(-5, 1, 2.5));
	// **********************************************************
	//                            LIGHT
	// **********************************************************

	// LUZES
	PointLight* luzAzul = new PointLight(vec3(0, 0, 1), vec3(0, 0, 1), vec3(0, 0, 1), 2.0f, 0.09f, 1.032f);
	luzAzul->setTranslate(vec3(5, 1, -10));
	ptrApp->AddLight(luzAzul);

	PointLight* luzVerde = new PointLight(vec3(0, 1, 0), vec3(0, 1, 0), vec3(0, 1, 0), 2.0f, 0.09f, 1.032f);
	luzVerde->setTranslate(vec3(5, 1, -12));
	ptrApp->AddLight(luzVerde);

	PointLight* luzVermelha = new PointLight(vec3(5, 0, 0), vec3(5, 0, 0), vec3(5, 0, 0), 2.0f, 0.09f, 1.032f);
	luzVermelha->setTranslate(vec3(5, 0.5, -14));
	ptrApp->AddLight(luzVermelha);

	Interpolator * ptrLinearLight = new PolinomialInterpolator(PolinomialInterpolator::CubicaEQuadrada);
	ptrLinearLight->addAttribute(new AttrVector3(vec3(10, 3, -20), 0.0f, PoseAttr::TRANSLATE)); // tempo: 1 seg

	Animator *  ptrLightAnim = new Animator(Animator::ONCE);
	ptrLightAnim->addInterpolator(ptrLinearLight);

	vec3 lightPos = vec3(1, 5, -10);
	DirectionalLight* light = new DirectionalLight(vec3(0.92, 0.95, 1.0), vec3(0.92, 0.95, 1.0), vec3(0.92, 0.95, 1.0));
	//light->setAnimator(ptrLightAnim);
	light->setTranslate(lightPos);
	ptrApp->AddLight(light);


	ptrApp->ptrlMainLight = light;
	//ptrApp->ptrcMainCamera = cam;
	//ptrApp->ptrv3CameraPos = new vec3(2, 1, -5);
	ptrApp->InitApplication(); 

	printf("%d particulas\n", ptrApp->iParticleCount);

	return 0;
}


