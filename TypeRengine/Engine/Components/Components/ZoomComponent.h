#ifndef ZOOMCOMPONENT_H
#define ZOOMCOMPONENT_H

#include <SFML/Graphics.hpp>
#include "../IEntityComponent.h"

class ZoomComponent : public IEntityComponent {
public:
	float Zoom = 1;
	float MaxZoom = 5;
	float MinZoom = 0;

public:
	ZoomComponent(float Zoom, float MaxZoom, float MinZoom)
		: Zoom(Zoom), MaxZoom(MaxZoom), MinZoom(MinZoom) {}

	ZoomComponent() = default;
};

#endif // ZOOMCOMPONENT_H