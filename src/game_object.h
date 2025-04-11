#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Provide a base class for renderable game objects
class GameObject {
public:
	virtual void draw() = 0;
	virtual void init() = 0;
	virtual ~GameObject() {}
};

#endif // !GAMEOBJECT_H
