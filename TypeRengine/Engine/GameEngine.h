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
	Event<FUNC> OnResized;
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
	void Start()
	{
		DebugLogger::Debug("Test project start");

		this->Window = new sf::RenderWindow(sf::VideoMode(800, 600), "Test Window");

		this->Window->setFramerateLimit(0); // uncap rq
        this->Window->setVerticalSyncEnabled(false); // I FUCKING HATE THIS BRO

		while (this->Window->isOpen())
		{
			sf::Event event;

			// poll events rq
			while (this->Window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					OnClose.Invoke(NULL);
					this->Window->close();
				}

				switch (event.type)
				{
                    case sf::Event::Closed:
                        OnClose.Invoke(NULL);
                        this->Window->close();
                        break;

                    case sf::Event::Resized:
                        OnResized.Invoke(NULL);
                        break;

                    case sf::Event::LostFocus:
                        OnLostFocus.Invoke(NULL);
                        break;

                    case sf::Event::GainedFocus:
                        OnGainedFocus.Invoke(NULL);
                        break;

                    case sf::Event::TextEntered:
                        OnTextEntered.Invoke(event.text.unicode);
                        break;

                    case sf::Event::KeyPressed:
                        OnKeyPressed.Invoke(event.key);
                        break;

                    case sf::Event::KeyReleased:
                        OnKeyReleased.Invoke(event.key);
                        break;

                    case sf::Event::MouseWheelMoved:  // Deprecated
                        OnMouseWheelMoved.Invoke(event.mouseWheel);
                        break;

                    case sf::Event::MouseWheelScrolled:
                        OnMouseWheelScrolled.Invoke(event.mouseWheelScroll);
                        break;

                    case sf::Event::MouseButtonPressed:
                        OnMouseButtonPressed.Invoke(event.mouseButton);
                        break;

                    case sf::Event::MouseButtonReleased:
                        OnMouseButtonReleased.Invoke(event.mouseButton);
                        break;

                    case sf::Event::MouseMoved:
                        OnMouseMoved.Invoke(event.mouseMove);
                        break;

                    case sf::Event::MouseEntered:
                        OnMouseEntered.Invoke(NULL);
                        break;

                    case sf::Event::MouseLeft:
                        OnMouseLeft.Invoke(NULL);
                        break;

                    case sf::Event::JoystickButtonPressed:
                        OnJoystickButtonPressed.Invoke(event.joystickButton);
                        break;

                    case sf::Event::JoystickButtonReleased:
                        OnJoystickButtonReleased.Invoke(event.joystickButton);
                        break;

                    case sf::Event::JoystickMoved:
                        OnJoystickMoved.Invoke(event.joystickMove);
                        break;

                    case sf::Event::JoystickConnected:
                        OnJoystickConnected.Invoke(event.joystickConnect);
                        break;

                    case sf::Event::JoystickDisconnected:
                        OnJoystickDisconnected.Invoke(event.joystickConnect);
                        break;

                    case sf::Event::TouchBegan:
                        OnTouchBegan.Invoke(event.touch);
                        break;

                    case sf::Event::TouchMoved:
                        OnTouchMoved.Invoke(event.touch);
                        break;

                    case sf::Event::TouchEnded:
                        OnTouchEnded.Invoke(event.touch);
                        break;

                    case sf::Event::SensorChanged:
                        OnSensorChanged.Invoke(event.sensor);
                        break;

                    default:
                        break;
				}
			}

			// render
			OnUpdate.Invoke(this->Window);
		}

		DebugLogger::Debug("Test project ended");
	}
};

#endif