#ifndef LEVEL_H
#define LEVEL_H

#include "Level/Spatial/SpatialMap.h"
#include "Level/LevelLayers.h"

class Level {
public:
	std::array<SpatialMap, LevelLayers::Count> Layers = {};

public:
	SpatialMap* GetLayer(LevelLayers layer);

	__forceinline int HashPosition(sf::Vector2f position);

	void Draw();
};

#endif // LEVEL_H