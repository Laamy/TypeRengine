#include "GameEngine.h"

void GameEngine::Start()
{
    DebugLogger::Debug("Window creation");

    this->Window = new sf::RenderWindow(sf::VideoMode(800, 600), "TypeRengine");

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
                this->Window->close();//close after event
                break;

            case sf::Event::Resized:
                this->Window->setSize(sf::Vector2u(event.size.width, event.size.height));//resize before event
                OnResized.Invoke(event.size);
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

    DebugLogger::Debug("Project ended");
}
