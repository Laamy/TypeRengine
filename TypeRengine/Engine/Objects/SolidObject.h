#ifndef SOLIDOBJECT_H
#define SOLIDOBJECT_H

#include "SolidActor.h"

class SolidObject : public SolidActor {
public:
	sf::RectangleShape Shape;

public:
	SolidObject() {
		Shape = sf::RectangleShape();
	}

public:
	virtual void Draw() override {
		ClientInstance* instance = ClientInstance::GetSingle();

		sf::RenderWindow* window = instance->GameEngine->Window;

		AABBShapeComponent* shape = GetShape();

		Shape.setPosition(shape->Position);
		Shape.setSize(shape->Size);

		Shape.setFillColor(sf::Color(255, 0, 0));

		window->draw(Shape);
	};
};

#endif // SOLIDOBJECT_H