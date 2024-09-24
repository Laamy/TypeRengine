#ifndef AABBSHAPECOMPONENT_H
#define AABBSHAPECOMPONENT_H

#include <SFML/Graphics.hpp>
#include "../IEntityComponent.h"

class AABBShapeComponent : public IEntityComponent {
public:
	sf::Vector2f Position;
	sf::Vector2f Size; // upper - position

public:
	AABBShapeComponent(sf::Vector2f Position, sf::Vector2f Size);
};

#endif // AABBSHAPECOMPONENT_H