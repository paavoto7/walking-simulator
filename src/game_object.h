#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "shader.h"

// Provide a base class for renderable game objects
class GameObject {
public:
	virtual void draw() = 0;
	virtual ~GameObject() {}
};

#endif // !GAMEOBJECT_H
