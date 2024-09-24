#ifndef CAMERA2D_SERVICE_H
#define CAMERA2D_SERVICE_H

#include "../../Engine/Service/ServiceBase.h"

#include "../../Engine/Components/EntityContext.h"
#include "../../Engine/Components/Components/AABBShapeComponent.h"
#include "../../Engine/Components/Components/ZoomComponent.h"

#include "../../Engine/Components/FlagComponents.h"
#include "../../Engine/Components/FlagComponent.h"

#include "../../Macros.h"

class Camera2DService : public ServiceBase {
public: // types
	EntityContext Context = EntityContext(ClientInstance::GetSingle()->EnttRegistry);

	sf::View view = sf::View();

private: // ignore
	sf::Vector2f initMousePos;

private: // ignore
	void HookEvents();

public: // methods
	Camera2DService(GameEngine* engine);

	/// <summary>
	/// WARNING: Will move camera so the center is still the same as before resize
	/// </summary>
	inline void setSize(sf::Vector2f size);

	/// <summary>
	/// Set this Camera2D as the active View in RenderWindow
	/// </summary>
	__forceinline void SetActive();
	
	/// <summary>
	/// Get the camera bounds in world coordinates
	/// </summary>
	inline sf::FloatRect getCameraBounds();

	/// <summary>
	/// Convert pixel coordinates to world coordinates
	/// </summary>
	inline sf::Vector2f pixelToWorld(sf::Vector2f pixelPos);

	/// <summary>
	/// Get the center of the camera in world coordinates
	/// </summary>
	inline sf::Vector2f getOrigin();

public: // events

#pragma region Movement & Zooming
	__forceinline void OnMouseMoveEvent(sf::Event::MouseMoveEvent e);
	__forceinline void OnMouseButtonPressed(sf::Event::MouseButtonEvent e);
	__forceinline void OnMouseButtonReleased(sf::Event::MouseButtonEvent e);

	__forceinline void OnMouseWheelScrolled(sf::Event::MouseWheelScrollEvent e);
#pragma endregion

	__forceinline void OnResize(sf::Event::SizeEvent e);

	__forceinline void OnUpdate(sf::RenderWindow* window);

public: // utils
	DEFINE_GET_COMPONENT(Shape, AABBShapeComponent);
	DEFINE_GET_COMPONENT(Zoom, ZoomComponent);

	DEFINE_IS_COMPONENT(AutoResize, AutoResizeFlag);
	DEFINE_IS_COMPONENT(AllowZoom, AllowZoomFlag);
	DEFINE_IS_COMPONENT(AllowMove, AllowMoveFlag);
	DEFINE_IS_COMPONENT(Moving, isMovingFlag);
};

#endif // CAMERA2D_SERVICE_H