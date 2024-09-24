#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "SFML/Graphics.hpp"

#include "ConCurrent/LockableVector.hpp"

#include "Events/Event.h"

#include "../DebugLogger.h"

#define FUNC uint32_t

class GameEngine
{
public:
	sf::RenderWindow* Window;

public: // other
    LockableVector<class ServiceBase*> Services = LockableVector<class ServiceBase*>();

public: // events
	Event<sf::RenderWindow*> OnUpdate;

	// poll'd events
	Event<FUNC> OnClose;
	Event<sf::Event::SizeEvent> OnResized;
	Event<FUNC> OnGainedFocus;
	Event<FUNC> OnLostFocus;

	Event<FUNC> OnMouseEntered;
	Event<FUNC> OnMouseLeft;

	Event<sf::Uint32> OnTextEntered;
	Event<sf::Event::KeyEvent> OnKeyPressed;
	Event<sf::Event::KeyEvent> OnKeyReleased;

	Event<sf::Event::MouseWheelEvent> OnMouseWheelMoved;
	Event<sf::Event::MouseWheelScrollEvent> OnMouseWheelScrolled;
	Event<sf::Event::MouseButtonEvent> OnMouseButtonPressed;
	Event<sf::Event::MouseButtonEvent> OnMouseButtonReleased;
	Event<sf::Event::MouseMoveEvent> OnMouseMoved;

	Event<sf::Event::JoystickButtonEvent> OnJoystickButtonPressed;
	Event<sf::Event::JoystickButtonEvent> OnJoystickButtonReleased;
	Event<sf::Event::JoystickMoveEvent> OnJoystickMoved;
	Event<sf::Event::JoystickConnectEvent> OnJoystickConnected;
	Event<sf::Event::JoystickConnectEvent> OnJoystickDisconnected;

	Event<sf::Event::TouchEvent> OnTouchBegan;
	Event<sf::Event::TouchEvent> OnTouchMoved;
	Event<sf::Event::TouchEvent> OnTouchEnded;

	Event<sf::Event::SensorEvent> OnSensorChanged;
    
public:
	void Start();
};

#endif