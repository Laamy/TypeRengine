#include "Level.h"

SpatialMap* Level::GetLayer(LevelLayers layer)
{
	return &Layers[layer];
}

int Level::HashPosition(sf::Vector2f position)
{
	return SpatialMap::HashPosition(50, position);
}

void Level::Draw()
{
	// temp until i add camera2d
	sf::FloatRect cameraBounds;

	cameraBounds.top = cameraBounds.left = 0;
	cameraBounds.width = 800;
	cameraBounds.height = 600;

	for (std::size_t i = 0; i < LevelLayers::Count; ++i) {
		auto objects = Layers[i].GetObjectsInBounds(cameraBounds);
		for (class SolidActor* child : objects)
			child->Draw();
	}
}