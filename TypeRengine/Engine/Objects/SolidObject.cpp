#include "SolidObject.h"

#include "../GameEngine.h"

SolidObject::SolidObject() : SolidActor() {
	Shape = sf::RectangleShape();
}

void SolidObject::Draw() {
	ClientInstance* instance = ClientInstance::GetSingle();

	sf::RenderWindow* window = instance->GameEngine->Window;

	AABBShapeComponent* shape = GetShape();

	Shape.setPosition(shape->Position);
	Shape.setSize(shape->Size);

	Shape.setFillColor(sf::Color(255, 0, 0));

	window->draw(Shape);
}
