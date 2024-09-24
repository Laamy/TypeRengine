#ifndef COUNTCOMPONENT_H
#define COUNTCOMPONENT_H

#include <SFML/Graphics.hpp>
#include "../IEntityComponent.h"

class CountComponent : public IEntityComponent {
public:
	std::uint32_t ObjectCount = 1;

public:
	CountComponent(std::uint32_t ObjectCount);
};

#endif