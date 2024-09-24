#include "Camera2DService.h"

// ignore
void Camera2DService::HookEvents() {
	Engine->OnMouseMoved.Hook([this](sf::Event::MouseMoveEvent e) { OnMouseMoveEvent(e); });
	Engine->OnMouseButtonPressed.Hook([this](sf::Event::MouseButtonEvent e) { OnMouseButtonPressed(e); });
	Engine->OnMouseButtonReleased.Hook([this](sf::Event::MouseButtonEvent e) { OnMouseButtonReleased(e); });
	Engine->OnMouseWheelScrolled.Hook([this](sf::Event::MouseWheelScrollEvent e) { OnMouseWheelScrolled(e); });

	Engine->OnResized.Hook([this](sf::Event::SizeEvent e) { OnResize(e); });
	Engine->OnUpdate.Hook([this](sf::RenderWindow* w) { OnUpdate(w); });
}

// methods

Camera2DService::Camera2DService(GameEngine* engine) {
	Init(engine);

	auto size = sf::Vector2u(800, 600); // TODO: get from engine (it errors cuz this is pre-init)
	Context.addComponent<AABBShapeComponent>(sf::Vector2f(0, 0), sf::Vector2f(size.x, size.y));
	Context.addComponent<ZoomComponent>(
		0.3f,	  // default zoom
		0.5f,	  // max zoom
		0.2f,	  // min zoom
		0.1f	  // step amount
	);

	// set all of these to by default, TRUE
	Context.addComponent<FlagComponent<AutoResizeFlag>>();
	Context.addComponent<FlagComponent<AllowZoomFlag>>();
	Context.addComponent<FlagComponent<AllowMoveFlag>>();

	// do event hooks
	HookEvents();

	DebugLogger::Print(LogType::Info, "Initialized : Camera2D");
}

void Camera2DService::setSize(sf::Vector2f size) {
	auto shape = getShape();
	auto zoom = getZoom();

	sf::View tmp = sf::View(sf::FloatRect(shape->Position, shape->Size));
	auto center = tmp.getCenter();

	shape->Size = size;

	tmp.reset(sf::FloatRect(shape->Position, shape->Size));
	tmp.zoom(zoom->Zoom);
	tmp.setCenter(center);

	shape->Position = sf::Vector2f(
		view.getCenter().x - size.x / 2,
		view.getCenter().y - size.y / 2
	);
}

void Camera2DService::SetActive() {
	auto shape = getShape();
	auto zoom = getZoom();

	view.reset(sf::FloatRect(shape->Position, shape->Size));
	view.zoom(zoom->Zoom);

	Engine->Window->setView(view);
}

sf::FloatRect Camera2DService::getCameraBounds() {
	auto shape = getShape();
	auto zoom = getZoom();

	sf::Vector2f topLeft = Engine->Window->mapPixelToCoords(sf::Vector2i(0, 0));
	sf::Vector2f bottomRight = Engine->Window->mapPixelToCoords(sf::Vector2i(shape->Size.x, shape->Size.y));

	sf::Vector2f sizeInWorld = bottomRight - topLeft;

	sizeInWorld /= zoom->Zoom;

	return sf::FloatRect(topLeft, sizeInWorld);
}

sf::Vector2f Camera2DService::pixelToWorld(sf::Vector2f pixelPos) {
	sf::Vector2f worldPos = Engine->Window->mapPixelToCoords(sf::Vector2i(pixelPos.x, pixelPos.y));

	return worldPos;
}

sf::Vector2f Camera2DService::getOrigin() {
	auto shape = getShape();
	auto zoom = getZoom();

	sf::View tmp = sf::View(sf::FloatRect(shape->Position, shape->Size));
	tmp.zoom(zoom->Zoom);

	auto center = tmp.getCenter();

	return center;
}

#pragma region Events

void Camera2DService::OnMouseMoveEvent(sf::Event::MouseMoveEvent e) {
	if (isAllowMove() && isMoving()) {
		sf::Vector2f curMousePos = sf::Vector2f(e.x, e.y);
		sf::Vector2f offset = curMousePos - initMousePos;

		auto shape = getShape();
		auto zoom = getZoom();

		shape->Position -= offset * zoom->Zoom;

		initMousePos = curMousePos;
	}
}

void Camera2DService::OnMouseButtonPressed(sf::Event::MouseButtonEvent e) {
	if (isAllowMove() && e.button == sf::Mouse::Button::Right) {
		Context.addComponent<FlagComponent<isMovingFlag>>();
		initMousePos = sf::Vector2f(e.x, e.y);
	}
}

void Camera2DService::OnMouseButtonReleased(sf::Event::MouseButtonEvent e) {
	if (isAllowMove() && e.button == sf::Mouse::Button::Right) {
		Context.removeComponent<FlagComponent<isMovingFlag>>();
	}
}

void Camera2DService::OnMouseWheelScrolled(sf::Event::MouseWheelScrollEvent e) {
	if (isAllowZoom()) {
		auto zoom = getZoom();

		float zoomAmount = zoom->StepAmount; // TODO: store in component

		// zoom in or out
		if (e.delta > 0)
			zoom->Zoom /= 1 + zoomAmount;
		else if (e.delta < 0)
			zoom->Zoom *= 1 + zoomAmount;

		// keep within bounds
		zoom->Zoom = std::clamp(zoom->Zoom, zoom->MinZoom, zoom->MaxZoom);
	}
}

void Camera2DService::OnResize(sf::Event::SizeEvent e) {
	if (isAutoResize()) {
		setSize(sf::Vector2f(e.width, e.height));
	}
}

void Camera2DService::OnUpdate(sf::RenderWindow* window) {
	SetActive(); // lmfao
}

#pragma endregion
