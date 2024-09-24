#ifndef CAMERA2D_SERVICE_H
#define CAMERA2D_SERVICE_H

#include <Windows.h>

#include "../../Engine/Service/ServiceBase.h"

#include "../../Engine/Components/EntityContext.h"
#include "../../Engine/Components/Components/AABBShapeComponent.h"
#include "../../Engine/Components/Components/ZoomComponent.h"

#include "../../Engine/Components/FlagComponents.h"
#include "../../Engine/Components/FlagComponent.h"

class Camera2DService : public ServiceBase {
public: // types
	EntityContext Context = EntityContext(ClientInstance::GetSingle()->EnttRegistry);

	sf::View view = sf::View();

private: // ignore
	sf::Vector2f initMousePos;

public: // methods
	Camera2DService(GameEngine* engine) {
		Init(engine);

		auto size = sf::Vector2u(800, 600); // TODO: get from engine (it errors cuz this is pre-init)
		Context.addComponent<AABBShapeComponent>(sf::Vector2f(0, 0), sf::Vector2f(size.x, size.y));
		Context.addComponent<ZoomComponent>(1, 5, 0);

		// set all of these to by default, TRUE
		Context.addComponent<FlagComponent<AutoResizeFlag>>();
		Context.addComponent<FlagComponent<AllowZoomFlag>>();
		Context.addComponent<FlagComponent<AllowMoveFlag>>();

		// do event hooks
		HookEvents();

		DebugLogger::Print(LogType::Info, "Initialized : Camera2D");
	}

	__forceinline void HookEvents() {
		Engine->OnMouseMoved.Hook([this](sf::Event::MouseMoveEvent e) { OnMouseMoveEvent(e); });
		Engine->OnMouseButtonPressed.Hook([this](sf::Event::MouseButtonEvent e) { OnMouseButtonPressed(e); });
		Engine->OnMouseButtonReleased.Hook([this](sf::Event::MouseButtonEvent e) { OnMouseButtonReleased(e); });
		Engine->OnMouseWheelScrolled.Hook([this](sf::Event::MouseWheelScrollEvent e) { OnMouseWheelScrolled(e); });
		
		Engine->OnResized.Hook([this](sf::Event::SizeEvent e) { OnResize(e); });
		Engine->OnUpdate.Hook([this](sf::RenderWindow* w) { OnUpdate(w); });
	}

	/// <summary>
	/// WARNING: Will move camera so the center is still the same as before resize
	/// </summary>
	inline void setSize(sf::Vector2f size) {
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

	__forceinline void SetActive() {
		auto shape = getShape();
		auto zoom = getZoom();

		view.reset(sf::FloatRect(shape->Position, shape->Size));
		view.zoom(zoom->Zoom);

		Engine->Window->setView(view);
	}

	inline sf::FloatRect getCameraBounds() {
		auto shape = getShape();
		auto zoom = getZoom();

		sf::Vector2f topLeft = Engine->Window->mapPixelToCoords(sf::Vector2i(0, 0));
		sf::Vector2f bottomRight = Engine->Window->mapPixelToCoords(sf::Vector2i(shape->Size.x, shape->Size.y));

		sf::Vector2f sizeInWorld = bottomRight - topLeft;

		sizeInWorld /= zoom->Zoom;

		return sf::FloatRect(topLeft, sizeInWorld);
	}

	inline sf::Vector2f PixelToWorld(sf::RenderWindow, sf::Vector2f pixelPos) {
		sf::Vector2f worldPos = Engine->Window->mapPixelToCoords(sf::Vector2i(pixelPos.x, pixelPos.y));
		
		return worldPos;
	}

	inline sf::Vector2f getOrigin() {
		auto shape = getShape();
		auto zoom = getZoom();

		sf::View tmp = sf::View(sf::FloatRect(shape->Position, shape->Size));
		tmp.zoom(zoom->Zoom);

		auto center = tmp.getCenter();

		return center;
	}

public: // events

#pragma region Movement & Zooming
	__forceinline void OnMouseMoveEvent(sf::Event::MouseMoveEvent e) {
		if (isAllowMove() && isMoving()) {
			sf::Vector2f curMousePos = sf::Vector2f(e.x, e.y);
			sf::Vector2f offset = curMousePos - initMousePos;

			auto shape = getShape();
			auto zoom = getZoom();

			shape->Position -= offset * zoom->Zoom;

			initMousePos = curMousePos;
		}
	}

	__forceinline void OnMouseButtonPressed(sf::Event::MouseButtonEvent e) {
		if (isAllowMove() && e.button == sf::Mouse::Button::Right) {
			Context.addComponent<FlagComponent<isMovingFlag>>();
			initMousePos = sf::Vector2f(e.x, e.y);
		}
	}

	__forceinline void OnMouseButtonReleased(sf::Event::MouseButtonEvent e) {
		if (isAllowMove() && e.button == sf::Mouse::Button::Right) {
			Context.removeComponent<FlagComponent<isMovingFlag>>();
		}
	}

	__forceinline void OnMouseWheelScrolled(sf::Event::MouseWheelScrollEvent e) {
		if (isAllowZoom()) {
			float zoomAmount = 0.1f; // TODO: store in component

			auto zoom = getZoom();

			// zoom in or out
			if (e.delta > 0)
				zoom->Zoom /= 1 + zoomAmount;
			else if (e.delta < 0)
				zoom->Zoom *= 1 + zoomAmount;

			// keep within bounds
			zoom->Zoom = std::clamp(zoom->Zoom, zoom->MinZoom, zoom->MaxZoom);
		}
	}
#pragma endregion

	__forceinline void OnResize(sf::Event::SizeEvent e) {
		if (isAutoResize()) {
			setSize(sf::Vector2f(e.width, e.height));
		}
	}

	__forceinline void OnUpdate(sf::RenderWindow* window) {
		SetActive(); // lmfao
	}

public: // utils
	__forceinline AABBShapeComponent* getShape() { return Context.tryGetComponent<AABBShapeComponent>(); }
	__forceinline ZoomComponent* getZoom() { return Context.tryGetComponent<ZoomComponent>(); }

	__forceinline bool isAutoResize() { return Context.hasComponent<FlagComponent<AutoResizeFlag>>(); }
	__forceinline bool isAllowZoom() { return Context.hasComponent<FlagComponent<AllowZoomFlag>>(); }
	__forceinline bool isAllowMove() { return Context.hasComponent<FlagComponent<AllowMoveFlag>>(); }
	__forceinline bool isMoving() { return Context.hasComponent<FlagComponent<isMovingFlag>>(); }
};

#endif // CAMERA2D_SERVICE_H