#include "stdafx.h"
#include "Movie.h"


#include "Constants.h"
#include "Application.h"
#include "TextureLoader.h"
#include "Controller.h"

Movie::Movie(std::string texture, int numberOfTextures)
{
	numberFrames = numberOfTextures;
	this->uiTextureVector = (unsigned int*)malloc(sizeof(unsigned int) * numberOfTextures);
	for (int i = 0; i < numberOfTextures; ++i)
	{
		this->uiTextureVector[i] = ptrTextureLoader->LoadTexture(texture + "(" + std::to_string(i + 1) + ").png");
		glBindTexture(GL_TEXTURE_2D, this->uiTextureVector[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	// The VBO containing the 4 vertices of the particles.
	const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};
	glGenBuffers(1, &uiVertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, uiVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	this->setTranslate(vec3(0, 0, 0));
}


Movie::~Movie()
= default;

void Movie::OnPreRender(glm::mat4)
{

}

void Movie::OnRender(glm::mat4)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->uiTextureVector[this->currentTexture]);
	glUniform1i(ptrApp->GetBillboardTextureId(), 0);

	glm::mat4 ViewMatrix = ptrController->getViewMatrix();
	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
	glm::mat4 ViewProjectionMatrix = ptrController->getProjectionMatrix() * ViewMatrix;

	glUniform3f(uiBillboardCameraRightId, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(uiBillboardCameraUpId, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniform3f(uiBillboardPositionId, this->v3Position.x, this->v3Position.y, this->v3Position.z);
	glUniform2f(uiBillboardSizeId, this->v2Size.x, this->v2Size.y);

	glUniform1f(uiBillboardColorMod, 1.0);

	glUniformMatrix4fv(uiBillboardViewProjMatrixId, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, uiVertexBufferId);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
}

void Movie::OnLoop(double dDeltaTime)
{
	counter += (float)dDeltaTime;
	// 30 fps = 0.33333333
	// 60 fps = 0.01666666
	if (counter - 0.03 > 0) {
		counter = 0;
		currentTexture++;
		currentTexture = currentTexture % numberFrames;
	}
}

void Movie::SetSize(glm::vec2 size)
{
	this->v2Size = size;
}

void Movie::setTranslate(glm::vec3 translate)
{
	this->v3Position = translate;
}

void Movie::Initialize()
{
	uiBillboardCameraRightId = ptrApp->GetBillboardCameraRightId();
	uiBillboardCameraUpId = ptrApp->GetBillboardCameraUpId();
	uiBillboardViewProjMatrixId = ptrApp->GetBillboardViewProjMatrixId();
	uiBillboardPositionId = ptrApp->GetBillboardPositionId();
	uiBillboardSizeId = ptrApp->GetBillboardSizeId();
	uiBillboardStatusId = ptrApp->GetBillboardStatusId();
	uiBillboardTextureId = ptrApp->GetBillboardTextureId();
	uiBillboardColorMod = ptrApp->GetBillboardColorMod();
}