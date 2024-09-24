#ifndef SOLIDOBJECT_H
#define SOLIDOBJECT_H

#include "SolidActor.h"

class SolidObject : public SolidActor {
public:
	sf::RectangleShape Shape;

public:
	SolidObject();

public:
	virtual void Draw() override;
};

#endif // SOLIDOBJECT_H