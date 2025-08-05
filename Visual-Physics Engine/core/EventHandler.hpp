//
//  EventHandler.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <functional>
#include <unordered_map>

#include <SFML/Graphics.hpp>


class Scene;


class EventHandler {
public:
    enum class Listener {
        MouseScrolled
    };

public:
    
    friend class ZoomHandler;

public:
    EventHandler(sf::RenderWindow& window, Scene& scene);

    void pollEvents();
    void handleEvent(const sf::Event& event);

    void subscribe(Listener listener, std::function<void()> callback);

private:
    void windowClosed();
    
    void ctrlMouseWheelScrolled(const sf::Event::MouseWheelScrolled& event);
    void shiftMouseWheelScrolled(const sf::Event::MouseWheelScrolled& event);
    void trackpadZoomed(float zoomFactor);
    void mouseWheelScrolled(const sf::Event::MouseWheelScrolled& event);

private:
    Scene& m_scene;
    sf::RenderWindow& m_window;

    std::vector<std::function<void()>> m_mouseScrolledCallbacks;
};


#endif // EVENTHANDLER_HPP
