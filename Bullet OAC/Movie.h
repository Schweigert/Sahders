#pragma once
#include "Entity2D.h"

#include <string>

class Movie :
	public Entity2D
{
protected:

	unsigned int			uiBillboardCameraRightId;		//!< id da matriz de camera a direita para billboard
	unsigned int			uiBillboardCameraUpId;			//!< id da matriz de camera para cima para billboard
	unsigned int			uiBillboardViewProjMatrixId;	//!< id da matriz view x projection para billboard
	unsigned int			uiBillboardPositionId;			//!< id da posicao do billboard
	unsigned int			uiBillboardSizeId;				//!< id do tamanho do billboard
	unsigned int			uiBillboardStatusId;			//!< id da de uma info extra do billboard
	unsigned int			uiBillboardTextureId;			//!< id da textura do  billboard
	unsigned int			uiBillboardColorMod;
protected:
	unsigned int*			uiTextureVector;		//!< Id da textura do billboard
	unsigned int			uiVertexBufferId;		//!< id do buffer de vertices

	float					fDeltaShear;

	bool					bIsShearable;			//!< ele move com o vento ( grama e tals )

	glm::vec3				v3Position;				//!< posicao do billboard no mundo

	glm::vec2				v2Size;					//!< tamanho do billboard

	int numberFrames;

	unsigned int currentTexture = 0;
	float counter = 0;
public:
	Movie(std::string texture = "textures/pato/", int numberOfTextures = 50);
	~Movie();

	virtual void OnPreRender(glm::mat4) override;


	virtual void OnRender(glm::mat4) override;


	virtual void OnLoop(double) override;

	void SetSize(glm::vec2 size);

	virtual void setTranslate(glm::vec3 translate) override;

	virtual void Initialize() override;

};

