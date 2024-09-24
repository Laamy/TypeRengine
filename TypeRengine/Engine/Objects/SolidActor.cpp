#include "SolidActor.h"

SolidActor::SolidActor() {
	// add the default main components
	Context.addComponent<AABBShapeComponent>(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
	Context.addComponent<CountComponent>(1);
}
