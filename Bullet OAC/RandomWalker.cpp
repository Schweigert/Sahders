#include "stdafx.h"
#include "RandomWalker.h"


RandomWalker::RandomWalker()
{
	this->vec3Force = glm::vec3(0, 0, 0);
}


RandomWalker::~RandomWalker()
{
}

glm::vec3 RandomWalker::GetResultingForce() {
	return vec3(0,0,0);
}

void RandomWalker::OnLoop(double dDeltaTime) {

}
